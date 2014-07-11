#include "../config.h"

/*
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or (at
 *  your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 *  USA.
 *
 */

/*
 * This file contains the scheduler and associated functions
 */

#include <cilk.h>
#include <cilk-internal.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <RD_and_OM.h>
#include <assert.h>

#define BATCH_ASSERT(args...) CILK_ASSERT(args)

// Declare early for use in Closure_steal. invoke-batch.c can't be
// included yet because it depends on some deque manipulation
// functions. The includes are kind of a mess.
static void invoke_batch_slow(CilkWorkerState *const _cilk_ws,
							  BatchFrame *_cilk_frame);

FILE_IDENTITY(ident,
			  "$HeadURL: https://bradley.csail.mit.edu/svn/repos/cilk/5.4.3/runtime/sched.c $ $LastChangedBy: bradley $ $Rev: 1698 $ $Date: 2004-10-22 22:10:46 -0400 (Fri, 22 Oct 2004) $");

#define CLOSURE_HEAD(cl)  (cl->cache->head)
#define CLOSURE_EXCEPTION(cl)  (cl->cache->exception)
#define CLOSURE_TAIL(cl)  (cl->cache->tail)
#define CLOSURE_STACK(cl) (cl->cache->stack)
#define CLOSURE_ABORT(cl) (cl->abort_status)

#define NOBODY (-1)   /* invalid processor number */

int global_node_count = 0;

/*
 * SCHEDULER LOCK DIAGRAM
 *
 * locks must be grabbed in the following order:
 *
 * ReadyDeque lock
 * Closure lock
 *
 * Closure locks must be acquired downwards
 * (parent first, then children)
 *
 * See also PROTOCOLS for a more verbose description.
 */

static inline void Cilk_switch2batch(CilkWorkerState *const ws)
{
	ws->batch_id = USE_SHARED(current_batch_id);
	ws->current_cache = &ws->ds_cache;
	ws->current_deque_pool = USE_PARAMETER(ds_deques);
}

static inline void Cilk_switch2core(CilkWorkerState *const ws)
{
	ws->batch_id = 0;
	ws->current_cache = &ws->cache;
	ws->current_deque_pool = USE_PARAMETER(deques);
}

/*************************************************************
 * Basic operations on closures
 *************************************************************/
static inline  void Closure_checkmagic(CilkWorkerState *const UNUSED(ws),
									   Closure *UNUSED(t))
{
	CILK_ASSERT(ws, t->magic == CILK_CLOSURE_MAGIC);
}

/* assert that t be locked by ourselves */
static inline void Closure_assert_ownership(CilkWorkerState *const UNUSED(ws),
											Closure *UNUSED(t))
{
	CILK_ASSERT(ws, t->mutex_owner == ws->self);
}

/* assert that t be not locked */
static inline void Closure_assert_alienation(CilkWorkerState *const UNUSED(ws),
											 Closure *UNUSED(t))
{
	CILK_ASSERT(ws, t->mutex_owner != ws->self);
}

static inline void Closure_clean(CilkContext *const context,
								 Closure *t)
{
	Cilk_mutex_destroy(context, &t->mutex);
}

static inline void Closure_destroy(CilkWorkerState *const ws, Closure *t)
{
	Closure_checkmagic(ws, t);

	WHEN_CILK_DEBUG(t->magic = ~CILK_CLOSURE_MAGIC);
	CILK_ASSERT(ws, ! (t->malloced));
	Closure_clean(ws->context, t);
	Cilk_internal_free(ws, t, sizeof(Closure));
}

static inline void Closure_destroy_malloc(CilkWorkerState *const ws,
										  Closure *t)
{
	Closure_checkmagic(ws, t);

	WHEN_CILK_DEBUG(t->magic = ~CILK_CLOSURE_MAGIC);
	CILK_ASSERT(ws, (t->malloced));
	Closure_clean(ws->context, t);
	Cilk_free(t);
}

static inline void Closure_lock(CilkWorkerState *const ws, Closure *t)
{
	Closure_checkmagic(ws, t);
	Cilk_mutex_wait(ws->context, ws, &(t->mutex));
	WHEN_CILK_DEBUG(t->mutex_owner = ws->self);
}

static inline void Closure_unlock(CilkWorkerState *const ws, Closure *t)
{
	Closure_checkmagic(ws, t);
	Closure_assert_ownership(ws, t);
	WHEN_CILK_DEBUG(t->mutex_owner = -1);
	Cilk_mutex_signal(ws->context, &(t->mutex));
}

/***********************************************************
 *  Internal random number generator.
 ***********************************************************/
static unsigned int rts_rand(CilkWorkerState *const ws)
{
	ws->rand_next = ws->rand_next * 1103515245 + 12345;
	return (ws->rand_next >> 16);
}

static void rts_srand(CilkWorkerState *const ws, unsigned int seed)
{
	ws->rand_next = seed;
}

/*********************************************************
 * Management of ReadyDeques
 *********************************************************/

/* initialize the deques */
static void create_deques(CilkContext *const context)
{
	int i;

	INIT_PARAMETER1(deques,
					Cilk_malloc_fixed(USE_PARAMETER1(active_size) *
									  sizeof(ReadyDeque)));
	CILK_CHECK(USE_PARAMETER1(deques),
			   (context, NULL, "failed to allocate deques\n"));
	INIT_PARAMETER1(ds_deques,
					Cilk_malloc_fixed(USE_PARAMETER1(active_size) *
									  sizeof(ReadyDeque)));
	CILK_CHECK(USE_PARAMETER1(ds_deques),
			   (context, NULL, "failed to allocate deques\n"));

	for(i = 0; i < USE_PARAMETER1(active_size); ++i) {
		Cilk_mutex_init(context, &USE_PARAMETER1(deques)[i].mutex);
		Cilk_mutex_init(context, &USE_PARAMETER1(ds_deques)[i].mutex);
	}

}

static void init_deques(CilkContext *const context)
{
	int i;

	for (i = 0; i < USE_PARAMETER1(active_size); ++i) {
		USE_PARAMETER1(deques)[i].top = (Closure *) NULL;
		USE_PARAMETER1(deques)[i].bottom = (Closure *) NULL;
		WHEN_CILK_DEBUG(USE_PARAMETER1(deques)[i].mutex_owner = NOBODY);

		USE_PARAMETER1(ds_deques)[i].top = (Closure *) NULL;
		USE_PARAMETER1(ds_deques)[i].bottom = (Closure *) NULL;
		WHEN_CILK_DEBUG(USE_PARAMETER1(ds_deques)[i].mutex_owner = NOBODY);
	}
}

static void free_deques(CilkContext *const context)
{
	int i;

	for(i = 0; i < USE_PARAMETER1(active_size); ++i) {
		Cilk_mutex_destroy(context, &USE_PARAMETER1(deques)[i].mutex);
		Cilk_mutex_destroy(context, &USE_PARAMETER1(ds_deques)[i].mutex);
	}

	Cilk_free(USE_PARAMETER1(deques));
	Cilk_free(USE_PARAMETER1(ds_deques));
}

/* assert that pn's deque be locked by ourselves */
static inline void deque_assert_ownership(CilkWorkerState *const UNUSED(ws),
										  int UNUSED(pn),
										  ReadyDeque *const deque_pool)
{
	CILK_ASSERT(ws, deque_pool[pn].mutex_owner == ws->self);
}

static inline void deque_assert_alienation(CilkWorkerState *const UNUSED(ws),
										   int UNUSED(pn),
										   ReadyDeque *const deque_pool)
{
	CILK_ASSERT(ws, deque_pool[pn].mutex_owner != ws->self);
}

static inline void deque_lock(CilkWorkerState *const ws, int pn,
							  ReadyDeque *const deque_pool)
{
	Cilk_mutex_wait(ws->context, ws, &deque_pool[pn].mutex);
	WHEN_CILK_DEBUG(deque_pool[pn].mutex_owner = ws->self);
}

static inline void deque_unlock(CilkWorkerState *const ws, int pn,
								ReadyDeque *const deque_pool)
{
	WHEN_CILK_DEBUG(deque_pool[pn].mutex_owner = NOBODY);
	Cilk_mutex_signal(ws->context, &deque_pool[pn].mutex);
}

/*
 * functions that add/remove elements from the top/bottom
 * of deques
 */
static Closure *deque_xtract_top(CilkWorkerState *const ws, int pn,
								 ReadyDeque *const deque_pool)
{
	Closure *cl;

	deque_assert_ownership(ws, pn, deque_pool);

	cl = deque_pool[pn].top;

	if (cl) {
		CILK_ASSERT(ws, cl->owner_ready_deque == pn);
		deque_pool[pn].top = cl->next_ready;
		if (cl == deque_pool[pn].bottom) {
			CILK_ASSERT(ws, cl->next_ready == (Closure *) NULL);
			deque_pool[pn].bottom = (Closure *) NULL;
		} else {
			CILK_ASSERT(ws, cl->next_ready);
			(cl->next_ready)->prev_ready = (Closure *) NULL;
		}
		WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
	} else {
		CILK_ASSERT(ws, deque_pool[pn].bottom == (Closure *) NULL);
	}

	return cl;
}

static Closure *deque_peek_top(CilkWorkerState *const ws, int pn,
							   ReadyDeque *const deque_pool)
{
	Closure *cl;

	deque_assert_ownership(ws, pn, deque_pool);

	cl = deque_pool[pn].top;

	if (cl) {
		CILK_ASSERT(ws, cl->owner_ready_deque == pn);
	} else {
		CILK_ASSERT(ws, deque_pool[pn].bottom == (Closure *) NULL);
	}

	return cl;
}

static Closure *deque_xtract_bottom(CilkWorkerState *const ws, int pn,
									ReadyDeque *const deque_pool)
{
	Closure *cl;

	deque_assert_ownership(ws, pn, deque_pool);

	cl = deque_pool[pn].bottom;
	if (cl) {
		CILK_ASSERT(ws, cl->owner_ready_deque == pn);
		deque_pool[pn].bottom = cl->prev_ready;
		if (cl == deque_pool[pn].top) {
			CILK_ASSERT(ws, cl->prev_ready == (Closure *) NULL);
			deque_pool[pn].top = (Closure *) NULL;
		} else {
			CILK_ASSERT(ws, cl->prev_ready);
			(cl->prev_ready)->next_ready = (Closure *) NULL;
		}

		WHEN_CILK_DEBUG(cl->owner_ready_deque = NOBODY);
	} else {
		CILK_ASSERT(ws, deque_pool[pn].top == (Closure *) NULL);
	}

	return cl;
}

static Closure *deque_peek_bottom(CilkWorkerState *const ws, int pn,
								  ReadyDeque *const deque_pool)
{
	Closure *cl;

	deque_assert_ownership(ws, pn, deque_pool);

	cl = deque_pool[pn].bottom;
	if (cl) {
		CILK_ASSERT(ws, cl->owner_ready_deque == pn);
	} else {
		CILK_ASSERT(ws, deque_pool[pn].top == (Closure *) NULL);
	}

	return cl;
}

#if 0
/*
 * Unused code, which may be useful if you hack the scheduler.
 */
static void deque_add_top(CilkWorkerState *const ws, Closure *cl, int pn,
						  ReadyDeque *const deque_pool)
{
	deque_assert_ownership(ws, pn, deque_pool);
	CILK_ASSERT(ws, cl->owner_ready_deque == NOBODY);

	cl->next_ready = deque_pool[pn].top;
	cl->prev_ready = (Closure *)NULL;
	deque_pool[pn].top = cl;
	WHEN_CILK_DEBUG(cl->owner_ready_deque = pn);

	if (deque_pool[pn].bottom) {
		CILK_ASSERT(ws, cl->next_ready);
		(cl->next_ready)->prev_ready = cl;
	} else {
		deque_pool[pn].bottom = cl;
	}
}
#endif

static void deque_add_bottom(CilkWorkerState *const ws, Closure *cl, int pn,
							 ReadyDeque *const deque_pool)
{
	deque_assert_ownership(ws, pn, deque_pool);
	CILK_ASSERT(ws, cl->owner_ready_deque == NOBODY);

	cl->prev_ready = deque_pool[pn].bottom;
	cl->next_ready = (Closure *)NULL;
	deque_pool[pn].bottom = cl;
	WHEN_CILK_DEBUG(cl->owner_ready_deque = pn);

	if (deque_pool[pn].top) {
		CILK_ASSERT(ws, cl->prev_ready);
		(cl->prev_ready)->next_ready = cl;
	} else {
		deque_pool[pn].top = cl;
	}
}

static inline void deque_assert_is_bottom(CilkWorkerState *const ws,
										  Closure *UNUSED(t),
										  ReadyDeque *const deque_pool)
{
	deque_assert_ownership(ws, ws->self, deque_pool);
	CILK_ASSERT(ws, t == deque_peek_bottom(ws, ws->self, deque_pool));
}

/* Remove closure for frame f from bottom of pn's deque and free them */
void Cilk_remove_and_free_closure_and_frame(CilkWorkerState *const ws,
											CilkStackFrame *f, int pn,
											ReadyDeque *const deque_pool)
{
	Closure *t;

	deque_lock(ws, pn, deque_pool);
	t = deque_xtract_bottom(ws, pn, deque_pool);
	CILK_ASSERT(ws, t->frame == f);
	deque_unlock(ws, pn, deque_pool);
	Cilk_free(f);
	Closure_destroy_malloc(ws, t);
}


/**************************************************
 * Management of CilkProcInfo's
 **************************************************/
/*
 * Returns a pointer to the CilkProcInfoT structure describing entry <entry>
 * of procedure whose signature is p.
 */
static inline CilkProcInfo *get_CilkProcInfo(CilkWorkerState *const UNUSED(ws),
											 CilkProcInfo * p, int entry)
{
	CILK_ASSERT(ws, entry > 0);
	return p + entry;
}

/*
 * Returns a pointer to the slow version for a procedure
 * whose signature is p.
 */
static inline void (*get_proc_slow(CilkProcInfo *p)) () {
	return p[0].inlet;
}

/********************************************
 * Closure management
 ********************************************/
static inline int Closure_at_top_of_stack(Closure *cl)
{
	return (CLOSURE_HEAD(cl) + 1 == CLOSURE_TAIL(cl));
}

static inline int Closure_has_children(Closure *cl)
{
	return (cl->join_counter != 0);
}

static inline void Closure_init(CilkContext *const context,
								CilkWorkerState *const UNUSED(ws), Closure *new)
{
	Cilk_mutex_init(context, &new->mutex);

	new->cp = (Cilk_time) 0;
	new->work = (Cilk_time) 0;

	new->complete_inlets = (struct InletClosure *) NULL;
	new->incomplete_inlets = (struct InletClosure *) NULL;
	new->next_ready = (Closure *) NULL;
	new->prev_ready = (Closure *) NULL;
	CLOSURE_ABORT(new) = NO_ABORT;
	new->return_size = 0;
	new->return_value = (void *)0;

	WHEN_CILK_DEBUG({
			new->magic = CILK_CLOSURE_MAGIC;
			new->owner_ready_deque = NOBODY;
			new->mutex_owner = NOBODY;
		});

	Cilk_event(ws, EVENT_CLOSURE_CREATE);
}

static Closure *Closure_create(CilkWorkerState *const ws)
{
	Closure *new = Cilk_internal_malloc(ws, sizeof(Closure));

	CILK_CHECK(new, (ws->context, ws, "can't allocate closure\n"));
	WHEN_CILK_DEBUG(new->malloced = 0;);

	Closure_init(ws->context,ws, new);

	return new;
}

Closure *Cilk_Closure_create_malloc(CilkContext *const context, CilkWorkerState *const ws)
{
	Closure *new = Cilk_malloc(sizeof(Closure));

	CILK_CHECK(new, (context, ws, "can't allocate closure\n"));
	WHEN_CILK_DEBUG(new->malloced = 1;);

	Closure_init(context, ws, new);

	return new;
}

/*
 * if (entry >= 0)
 *      get the inlet according to the entry
 * else
 *      get the inlet from the frame
 */
static struct InletClosure
*make_incomplete_inlet_closure(CilkWorkerState *const ws,
							   Closure *parent,
							   int entry)
{
	struct InletClosure *p;
	CilkStackFrame *f;
	CilkProcInfo *info;

	p = (struct InletClosure *)
		Cilk_internal_malloc(ws, sizeof(struct InletClosure));

	f = parent->frame;

	if (entry >= 0) {
		/* get the special exit closure */
		info = get_CilkProcInfo(ws, f->sig, entry);
	} else {
		info = get_CilkProcInfo(ws, f->sig, f->entry);
	}
	/* if no index is statically specified, get it from the frame */
	if (info->index == -1)
		p->receiver = f->receiver;
	else
		p->receiver = ((char *)f) + info->index;

	/* take care of inlets */
	if (info->inlet != NULL) {
		/* get arguments of inlet out of frame and into closure */
		p->inlet = info->inlet;
		p->argsize = info->argsize;
		p->inlet_args = Cilk_internal_malloc(ws, p->argsize);

		memcpy((void *) p->inlet_args,
			   (char *) f + info->argindex, p->argsize);
	} else {
		/*
		 * in the non-inlet case, we use the inlet_args field
		 * to hold the return value
		 */
		p->inlet = NULL;
		p->argsize = info->size;
		p->inlet_args = Cilk_internal_malloc(ws, p->argsize);
	}

	return p;
}

/*
 * only the scheduler is allowed to alter the closure
 * tree.  Consequently, these operations are private
 */
static void Closure_add_child(CilkWorkerState *const ws,
							  Closure *parent,
							  Closure *child)
{
	struct InletClosure *p;

	Closure_checkmagic(ws, parent);
	Closure_assert_ownership(ws, parent);
	Closure_checkmagic(ws, child);
	Closure_assert_alienation(ws, child);

	p = make_incomplete_inlet_closure(ws, parent, -1);

	/*
	 * semantic overloading: the inlet closure also contains
	 * the list of children.
	 */
	p->this = child;

	/* splice into list of unreturned children in parent */
	p->next = parent->incomplete_inlets;
	parent->incomplete_inlets = p;
}

static struct InletClosure *Closure_remove_child(CilkWorkerState *const ws,
												 Closure *parent,
												 Closure *child)
{
	struct InletClosure **q;
	struct InletClosure *p;

	Closure_checkmagic(ws, parent);
	Closure_checkmagic(ws, child);
	Closure_assert_ownership(ws, parent);

	for (q = &parent->incomplete_inlets; *q; q = &((*q)->next)) {
		p = *q;
		if (p->this == child) {
			*q = p->next;
			goto found;
		}
	}

	Cilk_die_internal(ws->context, ws, "BUG in Closure_remove_child\n");
	p = NULL;

found:
	return p;
}

/*******************************************
 * Exceptions
 *******************************************/
/*
 * Cilk supports a clever way to send an interruption/exception to a
 * running worker, without interrupting its normal way of working. The
 * idea is as follows.
 *
 * The local worker operates at the bottom of the frame stack, using a
 * stack pointer T (tail).  The exception mechanism maintains another
 * pointer E (exception).  The worker polls the value of E when
 * decrementing T.  Whenever T becomes less or equal than E, the
 * worker calls the exception handler of the runtime system, that does
 * all the necessary bookkeeping.
 *
 * Exceptions can be sent only to running closures.  The exception
 * pointer is always greater or equal than the head pointer (the index
 * of the first non-stolen frame on the stack)
 */

/* value of E bigger than any possible value of T */
#ifdef __GNUC__
#define EXCEPTION_INFINITY (volatile CilkStackFrame**)(-1LL)
#else
#if defined(__mips)
/* must be mipspro */
/* for o64 ABI (uses the LP memory model) */
#define EXCEPTION_INFINITY (volatile CilkStackFrame**)(-1L)
#else
#error ohops
#endif
#endif

static inline void increment_exception_pointer(CilkWorkerState *const ws,
											   Closure *cl)
{
	Closure_assert_ownership(ws, cl);

	CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

	if (CLOSURE_EXCEPTION(cl) != EXCEPTION_INFINITY) {
		++CLOSURE_EXCEPTION(cl);

		/* make sure the exception is visible, before we continue */
		Cilk_fence();
	}
}

static inline void decrement_exception_pointer(CilkWorkerState *const ws,
											   Closure *cl)
{
	Closure_assert_ownership(ws, cl);

	CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

	if (CLOSURE_EXCEPTION(cl) != EXCEPTION_INFINITY)
		--CLOSURE_EXCEPTION(cl);
}

static inline void reset_exception_pointer(CilkWorkerState *const ws,
										   Closure *cl)
{
	Closure_assert_ownership(ws, cl);

	CLOSURE_EXCEPTION(cl) = CLOSURE_HEAD(cl);
}

static inline void signal_immediate_exception(CilkWorkerState *const ws,
											  Closure *cl)
{
	Closure_assert_ownership(ws, cl);

	CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);

	CLOSURE_EXCEPTION(cl) = EXCEPTION_INFINITY;

	/* make sure the exception is visible, before we continue */
	Cilk_fence();
}

/********************************************************
 * Abort-related stuff
 ********************************************************/

/*
 * Abort is implemented by adding an abort_status in the closure
 * structure.
 *    NO_ABORT:         the closure is not aborted;
 *    ALMOST_NO_ABORT:  the closure is being aborted as parent;
 *    ABORT_ALL:        the closure is being aborted as child;
 *
 * When abort happens, we want to make sure that already spawned
 * but not returned subcomputations stop working, and "return" as
 * quickly as possible.  This is handled in the exception handler.
 * Also,  the inlets of a closure that is being aborted, either as
 * parent or child, should not be executed.  We handle this in
 * function apply_inlets.
 *
 * Abort is signaled either in running slow code, or in apply_inlets.
 * To signal an abort, the process that discovers the abort grabs
 * the parent lock and recursively goes down the tree to signal
 * it's children by setting their abort_status to ABORT_ALL, and
 * signal immediate exception to all running children.  The parent
 * is set to ALMOST_NO_ABORT, so that the complete inlets would not
 * be executed.
 *
 * Before we signal an abort from the parent, we check to see that, if
 * the parent is running and the worker is not on the top of the stack,
 * then, we promote the frame second to the top to be a complete closure,
 * put the parent back to the ready deque, and signal abort from the parent.
 * In this way, the parent will start execution at the right entry point.
 *
 * The stealing protocol does not steal a closure if it is being
 * aborted.  Inlets are also not applied to closures that are aborting.
 *
 * Once the abort is signaled, we let the normal protocol to handle
 * the abort:
 *
 *    We let the exception handler do the work to abort running closure.
 *    (see comments in exception_handler).
 *
 *    For a ready closure, we check whether it's being aborted before
 *    set up to execute it.  If it is being aborted, we free the last
 *    frame and let it return.
 *
 *    For a returning closure, if it is being aborted, we do not append
 *    the corresponding inlet to the complete inlet list, but discard
 *    the return value.
 *
 *    For a suspended closure, we do the same provably_good_steal.
 *    If it is being aborted, we will discover it we it's state is ready.
 *
 *
 */

static inline int get_frame_size(CilkStackFrame *f) {
	return (f->sig)[0].index; /* compiler put the frame size here. */
}

static inline int Closure_being_aborted(CilkWorkerState *const ws, Closure *cl)
{
	Closure_assert_ownership(ws, cl);
	return (CLOSURE_ABORT(cl) != NO_ABORT);
}

static inline void signal_abort(Closure *cl, enum AbortStatus s)
{
	if (s < CLOSURE_ABORT(cl))
		CLOSURE_ABORT(cl) = s;
}

static inline void reset_abort(Closure *cl)
{
	CLOSURE_ABORT(cl) = NO_ABORT;
}

static inline void maybe_reset_abort(Closure *cl)
{
	if (CLOSURE_ABORT(cl) != ABORT_ALL)
		reset_abort(cl);
}


static void recursively_signal_abort(CilkWorkerState *const ws,
									 Closure *cl, enum AbortStatus s)
{
	struct InletClosure *p;

	Closure_assert_ownership(ws, cl);
	signal_abort(cl, s);
	if (cl->status == CLOSURE_RUNNING)
		signal_immediate_exception(ws, cl);

	/* walk the list of children */
	for (p = cl->incomplete_inlets; p; p = p->next) {
		Closure_lock(ws, p->this);
		recursively_signal_abort(ws, p->this, ABORT_ALL);
		Closure_unlock(ws, p->this);
	}
}


static Closure *abort_ready_closure(CilkWorkerState *const ws, Closure *cl)
{
	/*
	 * abort a ready closure. There are two cases: if it has
	 * children, suspend it.  Else free the frame associated
	 * with the closure and make the closure return
	 */
	CILK_ASSERT(ws, CLOSURE_ABORT(cl) == ABORT_ALL);

	if (Closure_has_children(cl)) {
		cl->status = CLOSURE_SUSPENDED;
		Cilk_event(ws, EVENT_ABORT_READY_SUSPEND);
		return (Closure *)0;
	} else {
		Cilk_destroy_frame(ws, cl->frame, get_frame_size(cl->frame));
		Cilk_event(ws, EVENT_ABORT_READY_RETURN);
		cl->status = CLOSURE_RETURNING;
		return cl;
	}
}

/***********************************************************
 *         Work-stealing and related functions
 ***********************************************************/
/*
 * For stealing we use a Dekker-like protocol, that achieves
 * mutual exclusion using shared memory.
 *
 * Thief:
 *   lock
 *   E++          ; signal intention to steal, sending an exception
 *   (implicit MEMBAR)
 *   If (H >= T)  ; if we can steal
 *     E--        ; retract the exception
 *     give up
 *   steal (that is, H++, etc)
 *   unlock
 *
 * The victim does the normal exception-handling mechanism:
 *
 *   T--
 *   --MEMBAR-- ensure T write occurs before E is read.
 *   if (E>=T)
 *
 *
 */

/*
 * Do the thief part of Dekker's protocol.  Return 1 upon success,
 * 0 otherwise.  The protocol fails when the victim already popped
 * T so that E=T.
 */
static int do_dekker_on(CilkWorkerState *const ws, Closure *cl)
{
	Closure_assert_ownership(ws, cl);

	increment_exception_pointer(ws, cl);
	Cilk_membar_StoreLoad();

	if ((CLOSURE_HEAD(cl) + 1) >= CLOSURE_TAIL(cl)) {
		decrement_exception_pointer(ws, cl);
		return 0;
	}

	return 1;
}

static void Closure_make_ready(Closure *cl)
{
	cl->cache = (CilkClosureCache *)NULL;
	cl->status = CLOSURE_READY;
}

/*
 * promote the child frame of parent to a full closure.
 * Detach the parent and return it.
 *
 * Assumptions: the parent is running on victim, and we own
 * the locks of both parent and deque[victim].
 * The child keeps running on the same cache of the parent.
 * The parent's join counter is incremented.
 *
 * In order to promote a child frame to a closure,
 * the parent's frame must be the last in its ready queue.
 *
 * Returns the child.
 */
static Closure *promote_child(CilkWorkerState *const ws,
							  Closure *parent, int victim,
							  ReadyDeque *const deque_pool)
{
	Closure *child = Closure_create(ws);

	Closure_assert_ownership(ws, parent);
	deque_assert_ownership(ws, victim, deque_pool);
	CILK_ASSERT(ws, parent->status == CLOSURE_RUNNING);
	CILK_ASSERT(ws, parent->owner_ready_deque == victim);
	CILK_ASSERT(ws, parent->next_ready == (Closure *) NULL);

	/* can't promote until we are sure nobody works on the frame */
	CILK_ASSERT(ws, CLOSURE_HEAD(parent) <= CLOSURE_EXCEPTION(parent));

	/* update the various fields */
	child->parent = parent;
	child->join_counter = 0;
	child->cache = parent->cache;
	child->status = CLOSURE_RUNNING;

	/* setup the frame of the child closure */
	CLOSURE_HEAD(child)++;
	child->frame = (CilkStackFrame*)*CLOSURE_HEAD(child);

	/* insert the closure on the victim processor's deque */
	deque_add_bottom(ws, child, victim, deque_pool);

	/* at this point the child can be freely executed */
	return child;
}

/*
 * Finishes the promotion process.  The child is already fully promoted
 * and requires no more work (we only use the given pointer to identify
 * the child).  This function does some more work on the parent to make
 * the promotion complete.
 */
void finish_promote(CilkWorkerState *const ws,
					Closure *parent, Closure *child)
{
	Closure_assert_ownership(ws, parent);
	Closure_assert_alienation(ws, child);

	/* the parent is still locked; we still need to update it */
	/* join counter update */
	parent->join_counter++;

	/* register this child */
	Closure_add_child(ws, parent, child);

	/* Make the parent ready (also invalidates the cache) */
	Closure_make_ready(parent);

	return;
}

/*
 * stealing protocol.  Tries to steal from the victim; returns a
 * stolen closure, or NULL if none.
 */
static Closure *Closure_steal(CilkWorkerState *const ws, int victim,
							  ReadyDeque *const deque_pool)
{
	Closure *res = (Closure *) NULL;
	Closure *cl, *child;

	Cilk_event(ws, EVENT_STEAL_ATTEMPT);

	deque_lock(ws, victim, deque_pool);

	cl = deque_peek_top(ws, victim, deque_pool);

	if (cl) {
		Closure_lock(ws, cl);

		/* do not steal aborting closures */
		if (Closure_being_aborted(ws, cl)) {
			Cilk_event(ws, EVENT_STEAL_ABORT);
			goto give_up;
		}


		switch (cl->status) {
		case CLOSURE_READY:
			Cilk_event(ws, EVENT_STEAL);
			res = deque_xtract_top(ws, victim, deque_pool);
			CILK_ASSERT(ws, cl == res);

			/* it is ok to swap these two */
			Closure_unlock(ws, cl);
			deque_unlock(ws, victim, deque_pool);
			break;

		case CLOSURE_RUNNING:
			/* send the exception to the worker */
			if (do_dekker_on(ws, cl)) {
				/*
				 * if we could send the exception, promote
				 * the child to a full closure, and steal
				 * the parent
				 */
				child = promote_child(ws, cl, victim, deque_pool);

				/* detach the parent */
				res = deque_xtract_top(ws, victim, deque_pool);
				CILK_ASSERT(ws, cl == res);
				deque_unlock(ws, victim, deque_pool);

				/*
				 * at this point, more steals can happen
				 * from the victim.
				 */
				finish_promote(ws, cl, child);
				Closure_unlock(ws, cl);
				Cilk_event(ws, EVENT_STEAL);
			} else {
				Cilk_event(ws, EVENT_STEAL_NO_DEKKER);
				goto give_up;
			}
			break;

		case CLOSURE_SUSPENDED:
			Cilk_die_internal(ws->context, ws, "Bug: suspended closure in ready deque\n");
			break;

		case CLOSURE_RETURNING:
			/* ok, let it leave alone */
			Cilk_event(ws, EVENT_STEAL_RETURNING);

			/*
			 * MUST unlock the closure before the queue;
			 * see rule D in the file PROTOCOLS
			 */
		give_up:
			Closure_unlock(ws, cl);
			deque_unlock(ws, victim, deque_pool);
			break;

		default:
			Cilk_die_internal(ws->context, ws, "Bug: unknown closure status\n");
		}

	} else {
		deque_unlock(ws, victim, deque_pool);
		Cilk_event(ws, EVENT_STEAL_EMPTY_DEQUE);
	}
#if CILK_STATS
	if (res) {
		if (ws->batch_id) {
			USE_SHARED(batch_steals_success)[ws->self]++;
			if (res->frame->sig->inlet != invoke_batch_slow) {
				USE_SHARED(batch_steals_real)[ws->self]++;
			}
		}
		USE_SHARED(num_steals)[ws->self]++;
	} else {
		if (ws->batch_id) {
			USE_SHARED(batch_steals_fail)[ws->self]++;
		}
		USE_SHARED(num_steals_fail)[ws->self]++;
	}
#endif

	return res;
}

/********************************************************
 * Functions that take care of remote argument sending.
 ********************************************************/

/*
 * signal_abort_from_inlet:
 *
 * Signal all closures that need to be aborted from the parent <cl>.
 * If the parent is running and not working on the top of the
 * stackframe, then, the current work is a spawn and should be aborted
 * as well.  In this case, we promote the child frame to a closure,
 * detach the parent, put the parent on the ready deque, and abort from
 * the parent.
 */
static void signal_abort_from_inlet(CilkWorkerState *const ws, Closure *cl)
{
	Closure *child;
	Closure_assert_ownership(ws, cl);

	if (cl->status == CLOSURE_RUNNING &&
		!Closure_at_top_of_stack(cl)) {
		/*
		 * if closure is running and the frame is not on the top,
		 * then, poll_inlets is called in exception_handler.  So,
		 * the worker "ws->self" should own ready deque.
		 */
		deque_assert_ownership(ws, ws->self, ws->current_deque_pool);

		/* promote the child frame, and detach the parent from deque */
		child = promote_child(ws, cl, ws->self, ws->current_deque_pool);
		finish_promote(ws, cl, child);

		/*
		 * Now, the stackframe the exception handler is working on
		 * belongs to the child, not the parent.  Therefore, the
		 * exception handler should get the child from the deque
		 * after calling poll_inlets
		 */
	}

	/* do not abort the frame at the top of the stack */
	Cilk_enter_state(ws, STATE_ABORT_RECURSIVE);
	recursively_signal_abort(ws, cl, ALMOST_NO_ABORT);
	Cilk_exit_state(ws, STATE_ABORT_RECURSIVE);
}

/*
 * apply_inlet:
 *
 * Call inlet function only if the closure is not being aborted.  Use
 * abort_flag to check if abort happens when inlet function is
 * executed.  If abort occurs, signal all closures that need to be
 * aborted.
 */
static void apply_inlet(CilkWorkerState *const ws,
						Closure *cl, struct InletClosure *i)
{
	void *receiver = i->receiver;
	int argsize = i->argsize;
	void (*inlet) (CilkWorkerState *const, void *, void *, void *) = i->inlet;
	CilkStackFrame *frame = cl->frame;
	void *inlet_args = i->inlet_args;

	Closure_assert_ownership(ws, cl);

	/*
	 * do not apply inlets to aborting closures --- these inlets
	 * are supposed to be killed
	 */
	if (!Closure_being_aborted(ws, cl)) {
		ws->abort_flag = 0;

		if (inlet) {
			/* run the inlet */
			inlet(ws, frame, inlet_args, receiver);
		} else {
			/*
			 * default inlet: copy just-returned value into frame
			 */
			if (argsize) {
				memcpy(receiver, inlet_args, argsize);
			}

			/*
			 * TODO: make a little stub that does the memcpy.
			 * (Probably the resulting code will be messier than
			 *  just doing the check here)
			 */
		}

		if (ws->abort_flag)
			signal_abort_from_inlet(ws, cl);
	}

	/* free the arguments to the inlet, and the inlet structure itself */
	Cilk_internal_free(ws, inlet_args, argsize);
	Cilk_internal_free(ws, i, sizeof(struct InletClosure));
}

/* walk the list of inlets and call apply_inlets on all of them */
static void poll_inlets(CilkWorkerState *const ws, Closure *cl)
{
	struct InletClosure *i;

	Closure_assert_ownership(ws, cl);
	if (cl->status == CLOSURE_RUNNING &&
		!Closure_at_top_of_stack(cl))
		/*
		 * If we get here, poll_inlets has been called by
		 * the exception handler
		 */
		deque_assert_ownership(ws, ws->self, ws->current_deque_pool);

	Cilk_event(ws, EVENT_POLL_INLETS);

	for (i = cl->complete_inlets; i; i = cl->complete_inlets) {
		cl->complete_inlets = i->next;

		/*
		 * apply_inlets may change the ready deque. See comments in
		 * apply_inlet for details
		 */

		apply_inlet(ws, cl, i);
	}

	/*
	 * Reset the abort status after all incoming inlets have been
	 * executed/killed
	 */
	if (CLOSURE_ABORT(cl) == ALMOST_NO_ABORT)
		reset_abort(cl);
}

static void complete_and_enque_inlet(CilkWorkerState *const ws,
									 Closure *parent, Closure *child,
									 struct InletClosure *i)
{
	Closure_assert_ownership(ws, parent);
	Cilk_event(ws, EVENT_RETURN_ENQUEUE);

	/*
	 * Install value child is returning into inlet structure.
	 */
	CILK_COMPLAIN
		(child->return_size <= i->argsize,
		 (ws->context, ws,
		  "Cilk runtime system: invalid size of a return-ed value.\n"
		  "Either some internal Cilk data structure is corrupted,\n"
		  "or you have inconsistent prototypes across different files.\n"
		  "(E.g., cilk int foo(...)  and  cilk double foo(...))\n"));

	if (child->return_size) {
		CILK_ASSERT(ws, i->inlet_args != NULL);
		memcpy(i->inlet_args, child->return_value, child->return_size);
		Cilk_internal_free(ws, child->return_value, child->return_size);
		WHEN_CILK_DEBUG(child->return_value = NULL);
		WHEN_CILK_DEBUG(child->return_size = 0);
	}

	if (Closure_being_aborted(ws, child)) {
		Cilk_internal_free(ws, i->inlet_args, i->argsize);
		Cilk_internal_free(ws, i, sizeof(struct InletClosure));
	} else {
		i->next = parent->complete_inlets;
		parent->complete_inlets = i;
	}
}

static Closure *provably_good_steal_maybe(CilkWorkerState *const ws, Closure *parent)
{
	Closure *res = (Closure *) NULL;

	Closure_assert_ownership(ws, parent);

	if (!Closure_has_children(parent) &&
		parent->status == CLOSURE_SUSPENDED) {
		/* do a provably-good steal; this is *really* simple */
		res = parent;

		poll_inlets(ws, parent);

		Cilk_event(ws, EVENT_PROVABLY_GOOD_STEAL);

		/* debugging stuff */
		CILK_ASSERT(ws, parent->owner_ready_deque == NOBODY);
		Closure_make_ready(parent);
	}

	return res;
}

/*
 * Return protocol.
 *
 * This protocol deposits the return value of a subcomputation (child)
 * into the appropriate place (parent).  This function returns a
 * closure to be executed next, or NULL if none.
 * The child must not be locked by ourselves, and be in no deque.
 */
static Closure *Closure_return(CilkWorkerState *const ws, Closure *child)
{
	Closure *parent;
	Closure *res = (Closure *) NULL;
	struct InletClosure *inlet;

	CILK_ASSERT(ws, child);
	CILK_ASSERT(ws, child->join_counter == 0);
	CILK_ASSERT(ws, child->status == CLOSURE_RETURNING);
	CILK_ASSERT(ws, child->owner_ready_deque == NOBODY);
	Closure_assert_alienation(ws, child);

	parent = child->parent;
	CILK_ASSERT(ws, parent);

	/*
	 * at this point the status is as follows: the child is in
	 * no deque and unlocked.  However, the child is still
	 * in the children list of the parent.  Since the child
	 * is not locked, we can safely grab the parent's lock.
	 */
	Closure_lock(ws, parent);

	CILK_ASSERT(ws, parent->status != CLOSURE_RETURNING);
	CILK_ASSERT(ws, parent->frame);
	CILK_ASSERT(ws, parent->frame->magic == CILK_STACKFRAME_MAGIC);

	inlet = Closure_remove_child(ws, parent, child);

	/*
	 * the lock is not really necessary, since if rule D
	 * is obeyed no one can have a pointer to child at this
	 * point.  But, better be paranoid...
	 */
	Closure_lock(ws, child);

	complete_and_enque_inlet(ws, parent, child, inlet);

	/* tell the parent to poll this inlet (it may be aborting) */
	if (parent->status == CLOSURE_RUNNING)
		signal_immediate_exception(ws, parent);
	else {
		/* run them yourself */
		poll_inlets(ws, parent);
	}

	/* update critical path and work */
	WHEN_CILK_TIMING({
			if (ws->cp_hack > parent->cp)
				parent->cp = ws->cp_hack;
			parent->work += ws->work_hack;
		});

	/*
	 * the two fences ensure dag consistency (Backer)
	 */
	CILK_ASSERT(ws, parent->join_counter > 0);
	Cilk_fence();
	--parent->join_counter;
	Cilk_fence();

	res = provably_good_steal_maybe(ws, parent);

	Closure_unlock(ws, parent);

	/* now the child is no longer needed */
	Closure_unlock(ws, child);
	Closure_destroy(ws, child);

	return res;
}

/*
 * suspend protocol
 */
static void Closure_suspend(CilkWorkerState *const ws, Closure *cl,
							ReadyDeque *const deque_pool)
{
	Closure *cl1;

	Closure_checkmagic(ws, cl);
	Closure_assert_ownership(ws, cl);
	deque_assert_ownership(ws, ws->self, deque_pool);

	CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);
	cl->status = CLOSURE_SUSPENDED;

	Cilk_event(ws, EVENT_SUSPEND);
	cl1 = deque_xtract_bottom(ws, ws->self, deque_pool);
	USE_UNUSED(cl1); /* prevent warning when ASSERT is defined as nothing */

	CILK_ASSERT(ws, cl == cl1);
}

/*************************************************************
 * Various interactions with the cilk2c output
 *************************************************************/
/* destruction of a slow frame */
void Cilk_destroy_frame(CilkWorkerState *const ws,
						CilkStackFrame *f, size_t size)
{
	WHEN_CILK_ALLOCA(
		{
			if (f->alloca_h)
				Cilk_unalloca_internal(ws, f);
		});

	Cilk_internal_free(ws, f, size);
}

/* at a slow sync; return 0 if the sync succeeds, and 1 if suspended */
/* Unfortunately, the macro which calls this function, CILK2C_SYNC,
 * is generated by the cilk2c translator, so I can't change this
 * function to take a ReadyDeque argument without changing cilk2c,
 * which would be a huge pain. */
int Cilk_sync(CilkWorkerState *const ws)
{
	Closure *t;

	int res = 0;

	Cilk_event(ws, EVENT_CILK_SYNC);

	deque_lock(ws, ws->self, ws->current_deque_pool);

	t = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);

	Closure_lock(ws, t);
	CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
	CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

	/* assert we are really at the top of the stack */
	CILK_ASSERT(ws, Closure_at_top_of_stack(t));

	poll_inlets(ws, t);
	deque_assert_is_bottom(ws, t, ws->current_deque_pool);

	maybe_reset_abort(t);

	if (Closure_has_children(t)) {
		Closure_suspend(ws, t, ws->current_deque_pool);
		res = 1;
	}

	Closure_unlock(ws, t);

	deque_unlock(ws, ws->self, ws->current_deque_pool);
	return res;
}

#if CILK_TIMING
/* update work and CP after a slow sync */
void Cilk_after_sync_slow_cp(CilkWorkerState *const ws,
							 Cilk_time *work, Cilk_time *cp)
{
	Closure *t;

	deque_lock(ws, ws->self, ws->current_deque_pool);
	t = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);

	Closure_lock(ws, t);

	CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
	CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

	/* assert we are really at the top of the stack */
	CILK_ASSERT(ws, Closure_at_top_of_stack(t));

	*work += t->work;
	if (t->cp > *cp)
		*cp = t->cp;
	t->cp = 0;
	t->work = 0;

	Closure_unlock(ws, t);
	deque_unlock(ws, ws->self, ws->current_deque_pool);
}
#endif

static void move_result_into_closure(CilkWorkerState *const ws,
									 Closure *t, void *resultp,
									 int size)
{
	Closure_assert_ownership(ws, t);
	t->return_size = size;

	if (size) {
		t->return_value = Cilk_internal_malloc(ws, size);
		memcpy (t->return_value, (void *)resultp, size);
	} else {
		WHEN_CILK_DEBUG(t->return_value = NULL);
	}
}

/*
 * This is the exception handler.
 *
 * The handler returns 1 if the worker must return to the runtime
 * system, 0 otherwise.  This is what it does:
 *
 * We need to call poll_inlets first.
 * But poll_inlets may change the closure that owns the stack that
 * is currently running.  So, we need to get the closure from the
 * ready deque.  Then, we follow the protocol below:
 *
 *   reset exception pointer
 *   if ( H>=T )
 *       it is a steal
 *       change state into CLOSURE_RETURNING
 *       res = 1
 *
 *   else if (current frame is being aborted)
 *       res = 1
 *       signal another exception for parent frame
 *       if (current frame is the last frame)
 *            do a sync
 *            {suspend if needed; o.w. destroy frame and return}
 *       else
 *            destroy current frame
 *
 *   else { not steal, not abort}
 *       reset abort pointer to NO_ABORT
 *
 *   return res;
 */
int Cilk_exception_handler(CilkWorkerState *const ws, void *resultp, int size)
{
	Closure *t, *t1;

	int res = 0;

	Cilk_event(ws, EVENT_EXCEPTION);

	deque_lock(ws, ws->self, ws->current_deque_pool);
	t = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);

	CILK_ASSERT(ws, t);
	Closure_lock(ws, t);
	poll_inlets(ws, t);

	t1 = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);
	CILK_ASSERT(ws, t1);

	/*
	 * poll_inlets may change the bottom of the ready deque.
	 * (see comments at signal_abort_from_inlet)
	 */
	if (t != t1) {
		Closure_unlock(ws, t);
		t = t1;
		Closure_lock(ws, t);
	}

	reset_exception_pointer(ws, t);

	CILK_ASSERT(ws, t->status == CLOSURE_RUNNING ||
				t->status == CLOSURE_RETURNING);

	if (CLOSURE_HEAD(t) >= CLOSURE_TAIL(t)) {
		Cilk_event(ws, EVENT_EXCEPTION_STEAL);
		res = 1;

		if (t->status == CLOSURE_RUNNING) {
			t->status = CLOSURE_RETURNING;
			move_result_into_closure(ws, t, resultp, size);
		}
	} else {
		CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);
		if (Closure_being_aborted(ws, t)) {
			CilkStackFrame *f;
			res = 1;

			Cilk_event(ws, EVENT_EXCEPTION_ABORT);
			signal_immediate_exception(ws, t);

			if (Closure_at_top_of_stack(t)) {
				if (Closure_has_children(t)) {
					Cilk_event(ws, EVENT_EXCEPTION_ABORT_SUSPEND);
					/* pretend to sync */
					maybe_reset_abort(t);
					Closure_suspend(ws, t, ws->current_deque_pool);
					goto skip_free;
				} else {
					/* pretend we are returning */
					t->status = CLOSURE_RETURNING;
					Cilk_event(ws, EVENT_EXCEPTION_ABORT_RETURN);
				}
			}

			f = (CilkStackFrame *) CLOSURE_TAIL(t)[-1];
			Cilk_destroy_frame(ws, f, get_frame_size(f));
		skip_free: ;
		} else {
			/* frame not being aborted */
			maybe_reset_abort(t);
			Cilk_event(ws, EVENT_EXCEPTION_OTHER);
		}
	}

	Closure_unlock(ws, t);
	deque_unlock(ws, ws->self, ws->current_deque_pool);
	return res;

}
/*
 * before a return in the slow code.  This function marks the
 * closure as returning, so that it won't be stolen.
 */
void Cilk_set_result(CilkWorkerState *const ws, void *resultp, int size)
{
	Closure *t;

	Cilk_event(ws, EVENT_RETURN_SLOW);

	deque_lock(ws, ws->self, ws->current_deque_pool);
	t = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);
	Closure_lock(ws, t);

	CILK_ASSERT(ws, t->status == CLOSURE_RUNNING);
	CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

	/*
	 * this must happen after a sync; hence, either there is
	 * no abort at all, or the parent is trying to kill us
	 */
	CILK_ASSERT(ws, CLOSURE_ABORT(t) == NO_ABORT ||
				CLOSURE_ABORT(t) == ABORT_ALL);

	t->status = CLOSURE_RETURNING;
	t->frame = (CilkStackFrame *) NULL;

	move_result_into_closure(ws, t, resultp, size);

	Closure_unlock(ws, t);
	deque_unlock(ws, ws->self, ws->current_deque_pool);
}


void Cilk_abort_slow(CilkWorkerState *const ws)
{
	Closure *cl;

	deque_lock(ws, ws->self, ws->current_deque_pool);
	cl = deque_peek_bottom(ws, ws->self, ws->current_deque_pool);
	Closure_lock(ws, cl);

	CILK_ASSERT(ws, cl->status == CLOSURE_RUNNING);
	CILK_ASSERT(ws, Closure_at_top_of_stack(cl));
	Cilk_event(ws, EVENT_ABORT_SLOW);

	Cilk_enter_state(ws, STATE_ABORT_RECURSIVE);
	recursively_signal_abort(ws, cl, ALMOST_NO_ABORT);
	Cilk_exit_state(ws, STATE_ABORT_RECURSIVE);

	poll_inlets(ws, cl);

	Closure_unlock(ws, cl);
	deque_unlock(ws, ws->self, ws->current_deque_pool);
}

void Cilk_abort_standalone(CilkWorkerState *const ws)
{
	Cilk_event(ws, EVENT_ABORT_STANDALONE);
	ws->abort_flag = 1;
}

/*********************************************************
 * Execution
 *********************************************************/
static Closure *setup_for_execution(CilkWorkerState *const ws, Closure *t)
{
	Closure_assert_ownership(ws, t);

	CILK_ASSERT(ws, t->frame->magic == CILK_STACKFRAME_MAGIC);

	t->status = CLOSURE_RUNNING;
	t->cache = ws->current_cache;
	CLOSURE_STACK(t)[0] = t->frame;	/* push the first frame on the stack */
	CLOSURE_HEAD(t) = (volatile CilkStackFrame**)CLOSURE_STACK(t);
	CLOSURE_TAIL(t) = (volatile CilkStackFrame**)CLOSURE_STACK(t)+1;
	reset_exception_pointer(ws, t);
	reset_abort(t);

	return t;
}

static Closure *return_value(CilkWorkerState *const ws, Closure *t)
{
	Closure *res = NULL;

	Cilk_enter_state(ws, STATE_RETURNING);

	CILK_ASSERT(ws, t->status == CLOSURE_RETURNING);

	res = Closure_return(ws, t);

	Cilk_exit_state(ws, STATE_RETURNING);
	return res;
}

/*
 * execute the closure. If the closure is returning, take care of the
 * result.
 */
static Closure *do_what_it_says(CilkWorkerState *const ws, Closure *t,
								ReadyDeque *const deque_pool)
{
	Closure *res = NULL;
	CilkStackFrame *f;

	/* t must not be locked */
	Closure_assert_alienation(ws, t);
	Closure_lock(ws, t);

	switch (t->status) {
	case CLOSURE_READY:
		if (Closure_being_aborted(ws, t)) {
			res = abort_ready_closure(ws, t);
			Closure_unlock(ws, t);
		} else {
			/* just execute it */
			setup_for_execution(ws, t);
			f = t->frame;
			CILK_ASSERT(ws, f);
			CILK_ASSERT(ws, f->sig);
			CILK_ASSERT(ws, f->magic == CILK_STACKFRAME_MAGIC);
			poll_inlets(ws, t);
			Closure_unlock(ws, t);
			/*
			 * MUST unlock the closure before locking the queue
			 * (rule A in file PROTOCOLS)
			 */
			deque_lock(ws, ws->self, deque_pool);
			deque_add_bottom(ws, t, ws->self, deque_pool);
			deque_unlock(ws, ws->self, deque_pool);

			/* now execute it */
			if (ws->batch_id) {
				Cilk_enter_state(ws, STATE_BATCH_WORKING);
			} else {
				Cilk_enter_state(ws, STATE_WORKING);
			}
			(get_proc_slow(f->sig)) (ws, f);
			if (ws->batch_id) {
				Cilk_exit_state(ws, STATE_BATCH_WORKING);
			} else {
				Cilk_exit_state(ws, STATE_WORKING);
			}

		}

		break;

	case CLOSURE_RETURNING:
		/*
		 * the return protocol assumes t is not locked,
		 * and everybody will respect the fact that t is
		 * returning
		 */
		Closure_unlock(ws, t);
		res = return_value(ws, t);
		break;

	default:
		Cilk_die_internal(ws->context, ws,
						  "BUG in do_what_it_says(), t->status = %p\n",
						  t->status);
		break;
	}

	return res;
}

/*********************************************************
 * The scheduler itself
 *********************************************************/
void Cilk_scheduler(CilkWorkerState *const ws, Closure *t)
{
	int victim;

	CILK_ASSERT(ws, ws->self >= 0);
	rts_srand(ws, ws->self * 162347);

	Cilk_enter_state(ws, STATE_TOTAL);

	while (!USE_SHARED(done)) {

		if (!t) {
			/* try to get work from our local queue */
			deque_lock(ws, ws->self, ws->current_deque_pool);
			t = deque_xtract_bottom(ws, ws->self, ws->current_deque_pool);
			deque_unlock(ws, ws->self, ws->current_deque_pool);
		}

		/* otherwise, steal */
		while (!t && !USE_SHARED(done)) {

			Cilk_enter_state(ws, STATE_STEALING);

			// Decide where to steal from
			if ((rts_rand(ws) % 99)+1 <= USE_PARAMETER(dsprob)) {
				Cilk_switch2batch(ws);
			} else {
				Cilk_switch2core(ws);
			}
			victim = rts_rand(ws) % USE_PARAMETER(active_size);

			if (victim != ws->self) {
				t = Closure_steal(ws, victim, ws->current_deque_pool);
			}

			if (!t && USE_PARAMETER(options->yieldslice) &&
				!USE_SHARED(done)) {
				Cilk_lower_priority(ws);
			}

			Cilk_exit_state(ws, STATE_STEALING);
		} // End Stealing

		if (USE_PARAMETER(options->yieldslice))
			Cilk_raise_priority(ws);

		if (t && !USE_SHARED(done)) {
			t = do_what_it_says(ws, t, ws->current_deque_pool);
		}

		/*
		 * if provably-good steals happened, t will contain
		 * the next closure to execute
		 */
	}

	Cilk_exit_state(ws, STATE_TOTAL);

	return;
}

/*********************************************************
 * Batch scheduler for data structure operations
 *********************************************************/
#include "invoke-batch.c"

static inline int batch_done_yet(CilkWorkerState *const ws, int batch_id)
{
	// Don't need to check the status, but it might reduce contention.
	//  return USE_SHARED(pending_batch).array[ws->self].status == DS_DONE
	//    &&
	return USE_SHARED(current_batch_id) > batch_id;
}

#define MUSEC_IN_SEC 1000000
#define ELAPSED(begin, diff)								\
	(double)(diff.tv_sec * MUSEC_IN_SEC + diff.tv_usec -	\
			 (begin.tv_sec * MUSEC_IN_SEC + begin.tv_usec))

// Return 1 if the batch is actually done.
static inline int batch_sleep(CilkWorkerState *const ws,
                              unsigned int num_microseconds, int batch_id)
{
	struct timeval begin, diff;
	gettimeofday(&begin, NULL);
	gettimeofday(&diff, NULL);
	while(ELAPSED(begin,diff) < num_microseconds) {

		if (batch_done_yet(ws, batch_id)) return 1;

		gettimeofday(&diff, NULL);

	}
	return 0;
}

void batch_scheduler(CilkWorkerState *const ws, unsigned int batch_id,
                     Closure* t)
{
	int victim, should_steal;
	int done = 0;
	Closure* batch_closure = USE_PARAMETER(invoke_batch);

	Cilk_enter_state(ws, STATE_BATCH_SCHEDULING);

	CILK_ASSERT(ws, ws->self >= 0);
	CILK_ASSERT(ws, batch_id >= 0);

	while (!batch_done_yet(ws, ws->batch_id)) {
		if (!t) {
			/* try to get work from the local ds queue */
			deque_lock(ws, ws->self, USE_PARAMETER(ds_deques));
			t = deque_xtract_bottom(ws, ws->self, USE_PARAMETER(ds_deques));
			deque_unlock(ws, ws->self, USE_PARAMETER(ds_deques));
		}

		if(!t && batch_done_yet(ws, ws->batch_id)) {
			break;
		}

		while (!t && !batch_done_yet(ws, ws->batch_id)) {
			// Otherwise, steal

			should_steal = (rts_rand(ws) % 99) + 1;
			if (should_steal <= USE_PARAMETER(batchprob)) {
				Cilk_enter_state(ws, STATE_BATCH_STEALING);

				if (should_steal <= USE_PARAMETER(bias)) {
					victim = USE_SHARED(batch_owner);
				} else {
					victim = rts_rand(ws) % USE_PARAMETER(active_size);
				}

				if (victim != ws->self && victim != -1
					&& USE_SHARED(pending_batch).array[victim].status
					== DS_IN_PROGRESS) {

					t = Closure_steal(ws, victim, USE_PARAMETER(ds_deques));

					if (!t && USE_PARAMETER(options->yieldslice) &&
						USE_SHARED(current_batch_id) == batch_id) {
						Cilk_lower_priority(ws);
					}
				}
				Cilk_exit_state(ws, STATE_BATCH_STEALING);
			}

			if (batch_done_yet(ws, batch_id)) {
				done = 1;
				break;
			} else if (!t) {
				//        nanosleep(&USE_PARAMETER(sleeptime), NULL);
				if (batch_sleep(ws, USE_PARAMETER(sleeptime), batch_id)) break;
			}
		}
		if (!t && done) break;

		if (USE_PARAMETER(options->yieldslice))
			Cilk_raise_priority(ws);

		if (t) {
			Cilk_exit_state(ws, STATE_BATCH_SCHEDULING);
			t = do_what_it_says(ws, t, USE_PARAMETER(ds_deques));
			Cilk_enter_state(ws, STATE_BATCH_SCHEDULING);
		}
	}

	Cilk_exit_state(ws, STATE_BATCH_SCHEDULING);

	return;

}

void Cilk_batchify(CilkWorkerState *const ws,
				   InternalBatchOperation op, void *ds,
				   void *data, size_t data_size, void *indv_result)
{
	unsigned int num_ops, batch_id;
	int num_spots = USE_PARAMETER(batchvals);
	Batch* pending = &USE_SHARED(pending_batch);
	void* work_array = USE_SHARED(batch_work_array);

	BatchRecord *record = &pending->array[ws->self];
	record->operation = op;
	record->args = data;
	record->size = data_size;
	record->result = indv_result;

	Cilk_fence();
	asm volatile ("" : : : "memory");

	record->status = DS_WAITING;

	int* status = (int*) &record->status;
	Cilk_switch2batch(ws); // done in batch_scheduler

	do {

		// We can get rid of one of these.
		ws->batch_id = USE_SHARED(current_batch_id);
		batch_id = USE_SHARED(current_batch_id);

		if (*status == DS_WAITING &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			__sync_bool_compare_and_swap(&USE_SHARED(batch_lock), 0, 1)) {

			USE_SHARED(batch_owner) = ws->self;
			ws->batch_id = USE_SHARED(current_batch_id);
			pending->batch_no = ws->batch_id;

			// Resize if necessary.
			if (data_size > pending->data_size) {

				Cilk_free(USE_SHARED(batch_work_array));
				USE_SHARED(batch_work_array) =
					Cilk_malloc(USE_PARAMETER(active_size)
								* data_size
								* USE_PARAMETER(batchvals));
				work_array = USE_SHARED(batch_work_array);
				pending->data_size = data_size;
			}


			num_ops = compact(ws, pending, work_array, record);

			Closure* t = USE_PARAMETER(invoke_batch);
			BatchFrame* f = USE_SHARED(batch_frame);

			// Only need these for the slow version.
			f->args->ds = ds;
			f->args->work_array = (void*)work_array;
			f->args->num_ops = num_ops;
			f->args->op = op;

			/* deque_lock(ws, ws->self, USE_PARAMETER(ds_deques)); */
			/* Closure_lock(ws, t); */
			/* setup_for_execution(ws, t); */
			/* Closure_unlock(ws, t); */
			/* deque_add_bottom(ws, t, ws->self, USE_PARAMETER(ds_deques)); */
			/* deque_unlock(ws, ws->self, USE_PARAMETER(ds_deques)); */

			reset_batch_closure(ws->context);

			batch_scheduler(ws, ws->batch_id, t);
			/* do_what_it_says(ws, t, USE_PARAMETER(ds_deques)); */
			//      invoke_batch(ws, ds, op, num_ops);

		} else {
			ws->batch_id = USE_SHARED(current_batch_id);
			batch_scheduler(ws, batch_id, NULL);
		}
	} while (*status != DS_DONE);

	ws->batch_id = 0;

	Cilk_switch2core(ws); // done in batch_scheduler
	return;

}

// Do the collect, but just call the operation - don't use invoke_batch_slow
void Cilk_batchify_sequential(CilkWorkerState * const ws,
							  CilkBatchSeqOperation op, void *dataStruct,
							  void *data, size_t dataSize, void *indvResult)
{
	unsigned int i;
	Batch* pending = &USE_SHARED(pending_batch);
	int* work_array = USE_SHARED(batch_work_array);

	BatchRecord* record = &pending->array[ws->self];
	record->status = DS_WAITING;
	record->args = (int*)data;

	int* status = (int*)&record->status;
	//  Cilk_switch2batch(ws);

	do {
		ws->batch_id = USE_SHARED(current_batch_id);
		if (*status == DS_WAITING &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			__sync_bool_compare_and_swap(&USE_SHARED(batch_lock), 0, 1)) {

			ws->batch_id = USE_SHARED(current_batch_id);
			i = compact(ws, pending, work_array, NULL);

			op(pending, dataStruct, (void*)work_array, i, NULL);

			Cilk_terminate_batch(ws);
			//      USE_SHARED(batch_lock) = 0;
			break;
		} else {
			//      nanosleep(&USE_PARAMETER(sleeptime), NULL);
			if (batch_sleep(ws, USE_PARAMETER(sleeptime), ws->batch_id)) break;
		}
	} while (*status != DS_DONE);

	//  Cilk_switch2core(ws);
	return;

}

void Cilk_batchify_raw(CilkWorkerState *const ws,
					   CilkBatchSeqOperation op, void *dataStruct,
					   void *data, size_t dataSize, void *indvResult)
{
	unsigned int i;
	Batch* pending = &USE_SHARED(pending_batch);
	int* work_array = USE_SHARED(batch_work_array);

	BatchRecord* record = &pending->array[ws->self];
	record->status = DS_WAITING;

	// Memcpy is slower than a simple array insertion. I'm not quite
	//sure why this is so. Maybe the compiler can do some extra optimization?
	///  __builtin_memcpy(work_array + sizeof(helper)*ws->self, data, sizeof(helper));
	work_array[ws->self] = *(int*)data;
	int* status = (int*)&record->status;

	Cilk_switch2batch(ws);

	do {
		ws->batch_id = USE_SHARED(current_batch_id);
		if (*status == DS_WAITING &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			0 == USE_SHARED(batch_lock) &&
			__sync_bool_compare_and_swap(&USE_SHARED(batch_lock), 0, 1)) {

			ws->batch_id = USE_SHARED(current_batch_id);

			Batch* pending = &USE_SHARED(pending_batch);

			for (i = 0; i < USE_PARAMETER(active_size); i++) {
				if (pending->array[i].status == DS_WAITING)
					pending->array[i].status = DS_IN_PROGRESS;
			}

			op(pending, dataStruct, (void*)work_array, USE_PARAMETER(active_size), NULL);

			Cilk_terminate_batch(ws);

			//      USE_SHARED(batch_lock) = 0;
			break;
		} else {
			//      nanosleep(&USE_PARAMETER(sleeptime), NULL);
			if (batch_sleep(ws, USE_PARAMETER(sleeptime), ws->batch_id)) break;
		}
	} while (*status != DS_DONE);

	Cilk_switch2core(ws);

	return;
}

/*!
**************************************************
* Order Maintenance functions for race detection *
**************************************************/

#define ENGLISH_ID 10
#define HEBREW_ID 11
unsigned int INT_BIT_SIZE = 64;
unsigned long MAX_NUMBER = ~0;


//! Race_Detect Struct
/*! This struct is to be utilized as if it is the memory location
  of a particular variable in a program.  To change the value of
  the pointer, race_detect_Read or rd_write must be called and the
  necessary changes to the real variable they care about are handled
  internally, through the member data.
*/
struct RD_Memory_Struct_s {

	Cilk_mutex mutex; //Lock for atomicity
	void * data; //The memory location where the read/write occurs
	size_t size; //Size of 'data' data type
	OM_Node * left_r; //leftmost node that is reading
	OM_Node * right_r; //rightmost node that is reading
	OM_Node * left_w; //leftmost node that is writing
	OM_Node * right_w; //rightmost node that is writing

};

/// Initializes the paramers of a top list
Top_List * Init_top_list ()
{
	/// Malloc data for all necessary nodes and for the list itself
	Top_List * list = Cilk_malloc(sizeof(*list));
	list->head = Cilk_malloc(sizeof(* (list->head)));
	list->tail = Cilk_malloc(sizeof(* (list->tail)));

	/// Assign appropriate vals to list
	list->size  				= 	0;

	/// Assign T (in bender's paper), which governs how dense the list can be
	/// before rebalancing. As of now, we just pick an arbitrary val in [1,2]
	list->overflow_constant 	=	1.3;

	/// Assign appropriate vals to head and tail node tags
	list->head->tag = 0;
	list->tail->tag = ~0;

	/// Assign correct vals to head and tail pointers
	list->head->prev =  list->tail->next = NULL;

	/// Prev node for tail is head
	list->tail->prev = list->head;

	/// Next node for head is tail
	list->head->next = list->tail;

	return list;
}	

/// Initializes the parameters of a bottom list
Bottom_List * Init_bottom_list ()
{
	Bottom_List * list;

	//MAX_NUMBER = ~0;
	;//printf("Max # in init: %u\n", MAX_NUMBER);

	list = Cilk_malloc(sizeof(*list));
	list->head = Cilk_malloc(sizeof(* (list->head)));
	list->tail = Cilk_malloc(sizeof(* (list->tail)));

	// Set up list
	list->head->next_e = list->head->next_h = list->tail;
	list->tail->prev_e = list->tail->prev_h = list->head;

	list->size_e = list->size_h = list->Reorder_flag_h = list->Reorder_flag_e = 0;

	// Set up nodes
	list->head->tag_e = list->head->tag_h = 0;
	list->tail->tag_e = list->tail->tag_h = MAX_NUMBER;
	;//printf("Debug: list->tail->tag_e & id: %u & %i\n", list->tail->tag_e, list->tail->id);

	return list;

}
/// Rebalances the list according to Bender's algorithm around pivot
void top_list_rebalance(Top_List * list, Bottom_List *pivot)
{
	Bottom_List *lList = pivot, *rList = pivot;
	double overflow_density, overflow_threshold;
	unsigned long enclosing_tag_range, num_elements_in_sublist = 2;
	double i = -1;

	/// We assume l/rList are not NULL since the list will have at least 3 elements
	do	/// Check if range is in overflow
	{
		/// Move overflow list head and tail outward
		if (lList->prev){
			num_elements_in_sublist++;
			lList = lList->prev;
		}
		if (rList->next){
			num_elements_in_sublist++;
			rList = rList->next;
		}
		/// Calculate overflow_density
		enclosing_tag_range = rList->tag - lList->tag;

		i = ceil( log2((double)enclosing_tag_range) );
		overflow_threshold = pow(list->overflow_constant, -1.0 * i); 

		overflow_density = (num_elements_in_sublist/ (double)enclosing_tag_range ) ;
	}
	while ( (enclosing_tag_range == 0 || overflow_density > overflow_threshold ) && (lList != list->head || rList != list->tail));
	//printf("Debug: Rebalancing of tag range of %ul and num of elements %ul with density %f15 \n", enclosing_tag_range,num_elements_in_sublist, overflow_density);
	/// rebalance subsection of top_list
	long skip_size =  (unsigned long)((enclosing_tag_range - 1 ) / (num_elements_in_sublist)) ;
	/// Debug: not needed in final
	assert(skip_size>0);
	tag_range_relabel(lList, rList, skip_size );

	return;
}		/* -----  end of function top_list_rebalance(Top_List * list, Bottom_List *pivot)  ----- */



/// Inserts a Bottom_List into a Top_List
/// Bottom_List y is inserted after Bottom_List x in Top_List list
/// param ID the Eng or Heb structure
void Insert_top_list(Top_List * list, Bottom_List * x, Bottom_List *y)
{
	///Debug: Double check that all vals are not null
	//assert(y != NULL && x != NULL && list != NULL);

	y->tag = ((x->next->tag >> 1) + (x->tag >> 1));

	/// correct for adding two odd numbers
	if (x->next->tag & x->tag & 0x1 == 0x1)
		y->tag++;

	if (x->next->tag - x->tag <= 1)
	{
		top_list_rebalance(list, x);
		/// Dont assign pointers, call insert again to put y after x
		Insert_top_list(list, x, y);
	}
	else{
		/// Reassign  prev/next pointers
		y->prev = x;
		y->next = x->next;
		x->next = y;
		/// Update the next node after y's prev  reference (change it to y)
		y->next->prev = y;
	}

	/// IMPORTANT NOTE: size is only updated once since the list referes to the same nodes
	(list->size)++;
	return ;
}

/// This is called in the initialization of cilk
/// Add the first node to the OM_DS
void OM_DS_add_first_node(Bottom_List * om_ds, OM_Node * node, const int ID) {
#ifdef OM_IS_LL
	/// Debug message
	;//printf("Debug: appending node\n");

	/// Enter if ds and x are not NULL
	;// (debug) if (om_ds && node){

		if ( om_ds->size_e == 0 && om_ds->size_h == 0)
		{
			/// Assign head and tail to new node
			om_ds->tail = om_ds->head = node;

			/// Ensure node has no next
			node->next_e = node->next_h = NULL;

			/// Assign unique node id
			node->id =global_node_count++;

			/// Assign the node' data structure reference to om_ds passed in
			node->ds_e = node->ds_h = om_ds;

			/// Increment size of linked list
			om_ds->size_e = 1;
			om_ds->size_h = 1;
		}
		else {
			/// Debug code
			/// If linked list has nodes already, exit. Don't let this be called
			/// incorrectly and let code continue
			printf("List is non-empty, dont call add first node\n");
			exit(0);
		}
		;/*DEBUG}
	else {
		printf("Debug: appending null node or to null ds\n");
		exit(0);
	}
		 */
#elif defined OM_IS_BENDER

	/// Debug message
	;//printf("Debug: appending node\n");

	/// Enter if ds and x are not NULL
	;//DEBUG iff (om_ds && node){

		switch(ID) {

		case ENGLISH_ID:

			if (om_ds->size_e == 0) {

				/// Change head->next to be this node
				om_ds->head->next_e = node;

				/// Change node->prev to be the head
				node->prev_e = om_ds->head;

				/// Change node->next to be tail
				node->next_e = om_ds->tail;

				/// Change tail->prev to be this node
				om_ds->tail->prev_e = node;

				/// Assign unique node id
				node->id =global_node_count++;

				/// Assign tag
				node->tag_e = (om_ds->tail->tag_e >> 1);

				/// Increment size of linked list
				om_ds->size_e++;

				/// Assign node to this ds
				node->ds_e = om_ds;
			}
			else {
				/// Debug code
				/// If linked list has nodes already, exit. Don't let this be called
				/// incorrectly and let code continue
				printf("List is non-empty, dont call add first node\n");
				exit(0);
			}
			break;

		case HEBREW_ID:

			if (om_ds->size_h == 0) {
				/// Change head->next to be this node
				om_ds->head->next_h = node;

				/// Change node->prev to be the head
				node->prev_h = om_ds->head;

				/// Change node->next to be tail
				node->next_h = om_ds->tail;

				/// Change tail->prev to be this node
				om_ds->tail->prev_h = node;

				/// Assign unique node id
				node->id =global_node_count++;

				/// Assign tag
				node->tag_h = (om_ds->tail->tag_h >> 1);

				/// Increment size of linked list
				om_ds->size_h++;

				/// Assign node to this ds
				node->ds_h = om_ds;
			}
			else 	{
				/// Debug code
				/// If linked list has nodes already, exit. Don't let this be called
				/// incorrectly and let code continue
				printf("List is non-empty, dont call add first node\n");
				exit(0);
			}
			break;
		}
		;/*DEBUG}
	else
		printf("Debug: appending null node or to null ds\n");
		 */
#endif
}


/// Allocate memory and set variables
void OM_DS_init(CilkContext *const context){
	Bottom_List * bottom_list;

	/// Define CILK running parameters

	// If Batchify works
//#define BATCHIFY_WORKING

	if (context->Cilk_global_state){
		/// Debug message
		;//printf("Debug: OM_DS_init\n");

		/// Allocate heap memory for top list
		context->Cilk_global_state->OM_DS = Init_top_list();

		/// Make an initial bottom list for the top_list
		bottom_list = Init_bottom_list();

		/// Add first node to top_list for eng & heb
		Insert_top_list(context->Cilk_global_state->OM_DS,
						context->Cilk_global_state->OM_DS->head,
						bottom_list);
	}
}


/// Frees the nodes of the linked list
/// Since both linked lists contain the same nodes, it is sufficient
/// to move through the english list only.
void OM_LL_free_nodes_internal(CilkContext *const context){
	/// Create tmp nodes
	OM_Node  * node, *nextNode;

	/// Debug message
	;//printf("DEBUG:LL free nodes\n");

	/// Assign node to head of english list
	node = context->Cilk_global_state->OM_DS->head->next->head;

	/// Move through only Bottom_List, freeing each node.
	while(node != NULL){
		nextNode = node->next_e;
		Cilk_free(node);
		node = nextNode;
	}

	/// If LL, then top list will have head & tail bot_lists as well as 
	/// the one functioning linked list
	Cilk_free(context->Cilk_global_state->OM_DS->head);
	Cilk_free(context->Cilk_global_state->OM_DS->head->next);
	Cilk_free(context->Cilk_global_state->OM_DS->tail);

}


/// Frees nodes of OM_DS
void OM_free_nodes_internal(CilkContext *const context) {

	Top_List * top = context->Cilk_global_state->OM_DS; //!< Is freed in function that calls this
	Bottom_List * current = top->head, * next;
	OM_Node * node, * nextNode;

	/// Iterate through the Bottom_Lists
	while(current != top->tail) {

		next = current->next;

		/// Iterate through Nodes in Bottom_Lists
		node = current->head;
		while(node != current->tail){
			nextNode = node->next_e;
			Cilk_free(node); //!< Free the node
			node = nextNode;
		}
		Cilk_free(node); //!< Free tail node
	
		Cilk_free(current); //!< Free the Bot_list
		current = next;
	}
	Cilk_free(current); //!< Free the tail Bot_list

}



/// Frees the OM_DS in the context and all of the nodes within.
void OM_DS_free_and_free_nodes(CilkContext *const context){
#ifdef OM_IS_LL
	/// Free nodes of linked list
	OM_LL_free_nodes_internal(context);
#elif defined OM_IS_BENDER
	/// Free nodes of OM_DS
	OM_free_nodes_internal(context);
#else 
	printf("No ds defined so failure to free.\n");
	exit(10);
#endif
	///Debug message
	;//printf("Debug: free OMDS\n");

	/// Finally, free the Top_List
	Cilk_free(context->Cilk_global_state->OM_DS);
}


/**** START EXP SECTION ****/
/*
struct _cilk_insertPar_frame{CilkStackFrame header;
	struct{
		void*dataStruct;
		void*data;
		size_t size;
		void*result;
	}scope0;
	struct{InsertRecord*irArray;
		OM_DS*ds;
		InsertRecord*ir;
		OM_Node*x;
		OM_Node*y;
		int ID;
	}scope1;
};
struct _cilk_insertPar_args{
	void*dataStruct;
	void*data;
	size_t size;
	void*result;
};
static void _cilk_insertPar_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insertPar_frame*_cilk_frame);
static CilkProcInfo _cilk_insertPar_sig[]={{0,sizeof(struct _cilk_insertPar_frame),_cilk_insertPar_slow,0,0}};

void insertPar (CilkWorkerState*const _cilk_ws,void*dataStruct,void*data,size_t size,void*result){struct _cilk_insertPar_frame*_cilk_frame;
	{ _cilk_frame = Cilk_cilk2c_init_frame(_cilk_ws, sizeof(struct _cilk_insertPar_frame), _cilk_insertPar_sig);
	};
	{ Cilk_cilk2c_start_thread_fast_cp(_cilk_ws, &(_cilk_frame->header));
		Cilk_cilk2c_event_new_thread_maybe(_cilk_ws);
		;//ignore OM_DS stuff OM_DS_new_thread_start(_cilk_ws, &(_cilk_frame->header));
	};

	{
		InsertRecord *irArray= (InsertRecord *)data;
		OM_DS *ds= (OM_DS *) dataStruct;

		int i = 0;
		for (; i < size ; i++)
		{
			InsertRecord *ir= &irArray[i];
			OM_Node *x= ir->x;
			OM_Node*y=ir->y;
			int ID= ir->ID;

			if (!(x && y&& ds)) {
				printf("Some nod,skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n", x->id, x, y->id, y, ds->tail->id, ds->tail);

				{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header));
						Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
					};
					return;
				}
			};

			switch (ID) {
			case HEBREW_ID:

				if (!(x->next_h))
					ds->tail = y;

				y->next_h = x->next_h;

				if (!(__sync_bool_compare_and_swap(&(x->next_h),x->next_h, y)))
				{
					printf("Exiting, atomic insert failed");

					exit(0);

				}
				break;

			case ENGLISH_ID:

				if (!(x->next_e))
					ds->tail = y;

				y->next_e = x->next_e;

				if (!(__sync_bool_compare_and_swap(&(x->next_e),x->next_e, y)))
				{
					printf("Exiting, atomic insert failed");
					exit(0);
				}
				break;
			}

			ds->size++;
		} //end for

		{{ Cilk_cilk2c_before_return_fast_cp(_cilk_ws, &(_cilk_frame->header));
				Cilk_cilk2c_before_return_fast( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
			};
			return;
		}}}

static void _cilk_insertPar_slow(CilkWorkerState*const _cilk_ws,struct _cilk_insertPar_frame*_cilk_frame){CilkWorkerState*ws;
	void*dataStruct;
	void*data;
	size_t size;
	void*result;
	{ Cilk_cilk2c_start_thread_slow_cp(_cilk_ws, &(_cilk_frame->header));
		Cilk_cilk2c_start_thread_slow(_cilk_ws, &(_cilk_frame->header));
		;// ignore order maintenance functions OM_DS_new_thread_start(_cilk_ws, &(_cilk_frame->header));
	};
	switch (_cilk_frame->header.entry) {}
	dataStruct=_cilk_frame->scope0.dataStruct;
	data=_cilk_frame->scope0.data;
	size=_cilk_frame->scope0.size;
	result=_cilk_frame->scope0.result;

	{
		InsertRecord *irArray= (InsertRecord *)data;

		OM_DS *ds= (OM_DS *) dataStruct;
		int i = 0;
		for (; i < size ; i++)
		{ //start for
			InsertRecord *ir= &irArray[i];

			OM_Node *x= ir->x;
			OM_Node*y=ir->y;
			int ID= ir->ID;

			if (!(x && y&& ds)) {
				printf("Some node or ds is null,skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n", x->id, x, y->id, y, ds->tail->id, ds->tail);

				{{ Cilk_set_result(_cilk_ws, (void *)0, 0);
					};
					{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header));
						Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
					};
					return;
				}
			};

			switch (ID) {
			case HEBREW_ID:

				if (!(x->next_h))
					ds->tail = y;

				y->next_h = x->next_h;

				if (!(__sync_bool_compare_and_swap(&(x->next_h),x->next_h, y)))
				{
					printf("Exiting, atomic insert failed");
					exit(0);
				}
				break;

			case ENGLISH_ID:

				if (!(x->next_e))
					ds->tail = y;

				y->next_e = x->next_e;

				if (!(__sync_bool_compare_and_swap(&(x->next_e),x->next_e, y)))
				{
					printf("Exiting, atomic insert failed");
					exit(0);
				}
				break;
			}

			ds->size++;
		} //end for

		{{ Cilk_set_result(_cilk_ws, (void *)0, 0);
			};
			{ Cilk_cilk2c_before_return_slow_cp(_cilk_ws, &(_cilk_frame->header));
				Cilk_cilk2c_before_return_slow( _cilk_ws, &(_cilk_frame->header), sizeof(*_cilk_frame));
			};
			return;
		}}}

static void _cilk_insertPar_import(CilkWorkerState*const _cilk_ws,void*_cilk_procargs_v)

{(void)_cilk_ws;
	(void)_cilk_procargs_v;
	insertPar(_cilk_ws,((struct _cilk_insertPar_args*)_cilk_procargs_v)->dataStruct,((struct _cilk_insertPar_args*)_cilk_procargs_v)->data,((struct _cilk_insertPar_args*)_cilk_procargs_v)->size,((struct _cilk_insertPar_args*)_cilk_procargs_v)->result);

}
void mt_insertPar(CilkContext*const context,void*dataStruct,void*data,size_t size,void*result)
{struct _cilk_insertPar_args*_cilk_procargs;
	_cilk_procargs=(struct _cilk_insertPar_args*)Cilk_malloc_fixed(sizeof(struct _cilk_insertPar_args));
	_cilk_procargs->dataStruct=dataStruct;
	_cilk_procargs->data=data;
	_cilk_procargs->size=size;
	_cilk_procargs->result=result;
	Cilk_start(context,_cilk_insertPar_import,_cilk_procargs,0);
	Cilk_free(_cilk_procargs);

}
*/
/*** END EXP SECTION ****/


/// Prints the bottom List
void printList(Bottom_List * list, const int ID) {
    OM_Node * n;
    if (list && list->head)
        n = list->head;
    else
		exit(10);
	if( ID == HEBREW_ID)
		printf("Hebrew format: ==ID(tag)->== Head:");
	else
		printf("English format: ==ID(tag)->== Head:");

    while (n != NULL){
		if (ID == HEBREW_ID) {
			printf("%d(%lu)->", n->id, n->tag_h);
        	n = n->next_h;
		}
		else {
			printf("%d(%lu)->", n->id, n->tag_e);
			n = n->next_e;
		}
    }
    printf("Tail\n");
}

/// Prints the Top List
void print_top_list(Top_List *list){

	Bottom_List * current = list->head;
	printf("HEAD%u{ %lu}->", current->id, current->tag);

	while(current != NULL)
	{
		printf("#%u{%lu}->", current->id, current->tag);
		current = current->next;
	}
	printf("Tail\n\n\n");

}
/*Does one insert into the data structure x is held in*/
int single_node_insert_batch_helper(OM_Node *x, OM_Node * y, const int ID)
{
	Bottom_List * ds;

	switch(ID){
	case HEBREW_ID:

		/// Retrieve the data structure known node
		ds = x->ds_h;
	
		/// Update the ds y is in 
		y->ds_h = ds;

		///Debug: if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 0 ;
		}
		
		/// This is the procedure:
		y->next_h = x->next_h;
		x->next_h->prev_h = y;
		x->next_h = y;
		y->prev_h = x;
		
		/// Assign y's tag
		y->tag_h = ((y->next_h->tag_h >> 1) + (y->prev_h->tag_h >> 1));
		
		/// Update flag as necessary
		if(ds->size_h < (INT_BIT_SIZE >> 1))
			ds->Reorder_flag_h = 0;
		else
			ds->Reorder_flag_h = 1;

		ds->size_h++;

		if(ds->size_h == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

		break;

	case ENGLISH_ID:

		/// Retrieve the data structure known node
		ds = x->ds_e;
	
		/// Update the ds y is in 
		y->ds_e = ds;

		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 1;
		}

		/// This is the procedure:
		y->next_e = x->next_e;
		x->next_e->prev_e = y;
		x->next_e = y;
		y->prev_e = x;
		
		/// Assign y's tag
		y->tag_e = ((y->next_e->tag_e >> 1) + (y->prev_e->tag_e >> 1));

		/// Update flag as necessary
		if(ds->size_e < (INT_BIT_SIZE >> 1))
			ds->Reorder_flag_e = 0;
		else
			ds->Reorder_flag_e = 1;

		ds->size_e++;

		if(ds->size_e == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

	}

	/// Specific number for checks
	return 100;
}

/*For now just leave this as a C procedure*/
void insert_node_batch_op(CilkWorkerState *const _cilk_ws,
                                       void *data_struct, void *data,
                                       size_t num_elem, void *result)

{
	Top_List *list = data_struct;
	InsertRecord *ir, *irArray = (InsertRecord * )data;
	int rebalance = 0;
	while (num_elem > 0)
	{
		ir = &irArray[--num_elem];
		/// We need to do a rebalance if this returns 1
		if (single_node_insert_batch_helper(ir->x, ir->y, ir->ID) == 1)
		{
			if (ir->ID == ENGLISH_ID)
				Split_and_add_to_top(_cilk_ws, list, ir->x->ds_e);
			else if (ir->ID == HEBREW_ID)
				Split_and_add_to_top(_cilk_ws, list, ir->x->ds_h);
		}
	}

	if (rebalance)
			Rebalance_bottom_lists(_cilk_ws, list);
	return;
}
#ifdef OM_IS_LL
void OM_DS_insert(CilkWorkerState *const ws, OM_Node * x, OM_Node * y, const int ID){

	Bottom_List * ds;
	if (ID == HEBREW_ID)
		ds = y->ds_h = x->ds_h;
	else
		ds = y->ds_e = x->ds_e;

#ifdef BATCHIFY_WORKING
	/// Batchify insert
	/// Create the insert record, which will be passed to the operation function
	/// that we reference in Cilk_batchify(...,  &callback operation, ....);
	InsertRecord * ir = (InsertRecord * ) Cilk_malloc(sizeof(InsertRecord));
	ir->x =  x;
	ir->y =  y;
	ir->ID = ID;

	;//printf("Debug: INSERT:  x: %d , y: %d \n", x->id, y->id);
	/// Make call to batchify to assign this data structure opeartion
	/// to be executed at another time.
	Cilk_batchify(ws, &insertPar, ds, ir, sizeof(InsertRecord), NULL);
#else
	//Do insert here

	/* DEBUG
	//if x is null
	if (!(x && y) ){
               skipping insert; x(%d): %p y(%d):%p\n",
			   x->id, x, y->id, y);
		return;
	}
	*/

	/// Debug messages
	;//printf("Debug: INSERT: , x: %d , y: %d \n", x->id, y->id);
	switch(ID){
	case HEBREW_ID:
		//if x->next is null, x  is tail
		if ((x->next_h) == NULL)
			ds->tail = y;

		//change next pointers
		y->next_h = x->next_h;

		if(!(__sync_bool_compare_and_swap(&(x->next_h), x->next_h, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}

		ds->size_h++;

		break;
	case ENGLISH_ID:
		//if x->next is null, x  is tail
		if (!(x->next_e))
			ds->tail = y;

		//change next pointers
		y->next_e = x->next_e;

		if(!(__sync_bool_compare_and_swap(&(x->next_e), x->next_e, y)))
		{
			printf("Exiting, atomic insert failed");
			exit(0);
		}

		ds->size_e++;

		break;
	}

#endif
}

#elif defined OM_IS_BENDER
/// Insert y after x into appropriate list based on ID
/// Returns 1 if full and needs reorderd immediately and 0 otherwise
int OM_DS_insert(CilkWorkerState *const ws, OM_Node * x, OM_Node * y, const int ID){

#ifdef BATCHIFY_WORKING
	/// Create the insert record, which will be passed to the operation function
	/// that we reference in Cilk_batchify(...,  &callback operation, ....);
	InsertRecord * ir = (InsertRecord * ) Cilk_malloc(sizeof(InsertRecord));
	ir->x =  x;
	ir->y =  y;
	ir->ID = ID;

	;//printf("Debug: INSERT:  x: %d , y: %d \n", x->id, y->id);
	/// Make call to batchify to assign this data structure opeartion
	/// to be executed at another time.
	Cilk_batchify(ws, &insert_node_batch_op, WS_TOP_LIST, ir, sizeof(InsertRecord), NULL);
	return 0;
#else
	Bottom_List * ds;

	//INT_BIT_SIZE =  32;

	switch(ID){
	case HEBREW_ID:

		/// Retrieve the data structure known node
		ds = x->ds_h;
	
		/// Update the ds y is in 
		y->ds_h = ds;

		/* DEBUG
		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 0 ;
		}
		*/
		
		/// This is the procedure:
		y->next_h = x->next_h;
		x->next_h->prev_h = y;
		x->next_h = y;
		y->prev_h = x;
		
		/// Assign y's tag
		y->tag_h = ((y->next_h->tag_h >> 1) + (y->prev_h->tag_h >> 1));
		
		/// Update flag as necessary
		if(ds->size_h < (INT_BIT_SIZE >> 1))
			ds->Reorder_flag_h = 0;
		else
			ds->Reorder_flag_h = 1;

		ds->size_h++;

		if(ds->size_h == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

		break;

	case ENGLISH_ID:

		/// Retrieve the data structure known node
		ds = x->ds_e;
	
		/// Update the ds y is in 
		y->ds_e = ds;

		/* DEBUG
		//if x is null
		if (!(x && y && ds) ){
			printf("Some node or ds is null,\
               skipping insert; x(%d): %p y(%d):%p tail(%d):%p\n",
				   x->id, x, y->id, y, ds->tail->id, ds->tail);
			return 1;
		}
		*/

		/// This is the procedure:
		y->next_e = x->next_e;
		x->next_e->prev_e = y;
		x->next_e = y;
		y->prev_e = x;
		
		/// Assign y's tag
		y->tag_e = ((y->next_e->tag_e >> 1) + (y->prev_e->tag_e >> 1));

		/// Update flag as necessary
		if(ds->size_e < (INT_BIT_SIZE >> 1))
			ds->Reorder_flag_e = 0;
		else
			ds->Reorder_flag_e = 1;

		ds->size_e++;

		if(ds->size_e == INT_BIT_SIZE)
			return 1; ///< Needs to be split
		return 0; ///< Doesn't needs immediately split

	}

	/// Specific number for checks
	return 100;
/// End else for if BATCHIFY_WORKING
#endif
}
#endif


/// Function that splits a sublist in half and adds second
/// half to top list as a new sublist
void Split_and_add_to_top(Top_List * tlist, Bottom_List * blist) {
	/// ***** Alex's version ***** ///
	
	Bottom_List * to_add = Bottom_List_init();/*Init_bottom_list();*/
	
	int node_count_e = 1, node_count_h = 1;
	/// English
	if (blist->size_e > (MAX_NUMBER >> 1 )){
		unsigned long skip_size = (MAX_NUMBER - 1) / (unsigned long)((blist->size_e >> 1) + 1) ;
		

		/// Assign first node tag
		OM_Node * current_e = blist->head->next_e;
		current_e->tag_e = skip_size;

		/// Update first half of the list
		while(node_count_e < (blist->size_e >> 1) ) {
			/// Move current node along
			current_e = current_e->next_e;
			/// Update current tage
			current_e->tag_e = current_e->prev_e->tag_e + skip_size;
			/// Update node count
			node_count_e++;
		}

		/// Update the tail of each list
		blist->tail->prev_e = current_e;/*middle_node;*/


		/// Update size of first and second lists
		to_add->size_e = blist->size_e - node_count_e;
		blist->size_e = node_count_e; 

		///Reassign head->next_e of to_add
		// and update to_add head and tail references
		current_e = current_e->next_e;
		to_add->head->next_e = to_add->tail->prev_e = current_e;
		
		current_e->prev_e = to_add->head;
		
		/// Update current node reference to ds
		current_e->ds_e = to_add;

		current_e->tag_e = skip_size;
		
		while (current_e->next_e != blist->tail)
		{
			/// Move along current node
			current_e = current_e->next_e;

			/// Update current tag_e
			current_e->tag_e = current_e->prev_e->tag_e + skip_size;

			/// Update node's ds
			current_e->ds_e = to_add;
		}
		/// Update tag_e
		/*current_e->tag_e = current_e->prev_e->tag_e + skip_size;*/

		/// Update next_e pointer to the new list's tail
		current_e->next_e = to_add->tail;

		/// Update tail of to_add list
		to_add->tail->prev_e = current_e;

		

	}
	/// Hebrew
	if (blist->size_h > (MAX_NUMBER >> 1 )){
		unsigned long skip_size = (MAX_NUMBER - 1) / (unsigned long)(1+ (blist->size_h >> 1)) ;
		

		/// Assign first node tag
		OM_Node * current_h = blist->head->next_h;
		current_h->tag_h = skip_size;

		/// Update first half of the list
		while(node_count_h < (blist->size_h >> 1) ) {
			/// Move current node along
			current_h = current_h->next_h;
			/// Update current tage
			current_h->tag_h = current_h->prev_h->tag_h + skip_size;
			/// Update node count
			node_count_h++;
		}

		/// Holds the middle node
		/*OM_Node * middle_node = current_h;*/

		/// Update the tail of each list
		blist->tail->prev_h = current_h;/*middle_node;*/


		/// Update size of first and second lists
		to_add->size_h = blist->size_h - node_count_h;
		blist->size_h = node_count_h; 

		///Reassign head->next_h of to_add
		// and update to_add head and tail references
		current_h = current_h->next_h;
		to_add->head->next_h = to_add->tail->prev_h = current_h;
		
		/// Update current node references to prev and next
		/* don't update this yet, or we will lose the next node reference
		 * current_h->next_h = to_add->tail;
		 * */

		current_h->prev_h = to_add->head;
		
		/// Update current node reference to ds
		current_h->ds_h = to_add;

		current_h->tag_h = skip_size;
		
		while (current_h->next_h != blist->tail)
		{
			/// Move along current node
			current_h = current_h->next_h;

			/// Update current tag_h
			current_h->tag_h = current_h->prev_h->tag_h + skip_size;

			/// Update node's ds
			current_h->ds_h = to_add;
		}
		/// Update tag_h
		/*current_h->tag_h = current_h->prev_h->tag_h + skip_size;*/

		/// Update next_h pointer to the new list's tail
		current_h->next_h = to_add->tail;

		/// Update tail of to_add list
		to_add->tail->prev_h = current_h;



	}

	/// Reset reorder flag
	blist->Reorder_flag_e = blist->Reorder_flag_h = 0;

	nsert_top_list(tlist, blist, to_add);


}

/// Iterate through the top list to find sublists needing reordered
void Rebalance_bottom_lists(Top_List * list) {

	/// The iterators
	Bottom_List * current_e, * current_h;
	current_e = current_h = list->head;

	/// NOTE: Only eng/heb distinction for the flags, not top_list splits

	/// English
	while(current_e != list->tail) {
		if(current_e->Reorder_flag_e == 1) ///< If 1, then needs split
			Split_and_add_to_top(list, current_e);
		current_e = current_e->next;
	}

	/// Hebrew
	while(current_h != list->tail) {
		if(current_h->Reorder_flag_h == 1) ///< If 1, then needs split
			Split_and_add_to_top(list, current_h);
		current_h = current_h->next;
	} 
}

/// Relabels the range of nodes from x to y
void tag_range_relabel (Bottom_List *x, Bottom_List *y, unsigned long tag_spacing )
{
	while (x->next != y){
		/// Debug:
		/// insert x->next after x but with y->tag as the end tag
		y->tag = x->tag + tag_spacing;

		/// Move along x pointer
		x = x->next;

	}
	return ;
}		/* -----  end of function tag_range_relabel  ----- */


/// Within the void *ds, depending on macros defined in main, determine the order
/// of x and y. If x <= y, return true. Otherwise, return false.
/// Note: the ID will determine which ordering to follow (english or hebrew)
int OM_DS_order(OM_Node * x, OM_Node * y, const int ID){
#ifdef OM_IS_LL

	/// To retrieve Bottom_List
	Bottom_List * ds;

	///Temp node to hold current node
	OM_Node * current;

	if (ID == HEBREW_ID)
		ds = x->ds_h;
	else
		ds = x->ds_e;

    /// Point tmp to head
	current = ds->head;

	/// For a linked list, inc. the node pointer down the correct list.
	/// If at any point current is x, we immediately exit true
	/// If at any point current is y, we immediately exit false
	do {
		if (current == y)
			return 0;
		else if (current == x)
			return 1;
		else if (ID == ENGLISH_ID)
			current = current->next_e;
		else if (ID == HEBREW_ID)
			current = current->next_h;
	} while( current != ds->tail);

	/// Debug code, to ensure we are not getting false or true by calling order
	/// on incorrect nodes.
	printf("Debug: Neither node found in linked list. Error, exiting.");
	exit(1);

	return 0;

#elif defined OM_IS_BENDER
	/// First check to see if the Top_List tags are ordered
	/// If they're not, then the nodes are in the same sub_list
	/// and it suffices to check their tags against another

	Bottom_List * x_bl, * y_bl;
/// Debug
	if( !(x && y) ) {
		printf("Debug: Order - one of the nodes null\n");
		exit(10);
	}

	switch(ID){
	case HEBREW_ID:

		/// The tags of the lists of nodes themselves
		x_bl = x->ds_h;
		y_bl = y->ds_h;

		/// If the nodes are in different lists, compare the list tags
		if(x_bl->tag != y_bl->tag)
			if(x_bl->tag > y_bl->tag) return 0; else return 1;

		/// Otherwise just check them directly
		/// Note: if they are the same node, return 0 (by convention)
		else
			if(x->tag_h >= y->tag_h) return 0; else return 1;

	case ENGLISH_ID:

		/// The tags of the lists of nodes themselves
		x_bl = x->ds_e;
		y_bl = y->ds_e;

		/// If the nodes are in different lists, compare the list tags
		if(x_bl->tag != y_bl->tag)
			if(x_bl->tag > y_bl->tag) return 0; else return 1;

		/// Otherwise just check them directly
		/// Note: if they are the same node, return 0 (by convention)
		else
			if(x->tag_e >= y->tag_e) return 0; else return 1;
	}

	printf("Debug: something went wrong in OM_DS_order\n");
	return 0;

#else
	printf("Order called with no applicable ds macro specified.\n");
	exit(10);
#endif
}



/*!******************************************************************************
******************** OM_DS Functions that are Runtime Specific *****************
********************************************************************************/
void OM_DS_before_spawn(CilkWorkerState *const ws, CilkStackFrame *frame, const int FAST_NOT_SLOW){
/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
//	    printf("Debug: In batch node, no race detect needed");
		return;
	}
/// Instantiate three new nodes
//
/// cont_node: continuation node, will follow from the current node (continues after spawn)
/// post_sync_node: the node to move to after the next sync occurs (only malloc'ed if first_Spawn_flag is 0)
/// spawned_func_node: the node that the spawned function will be represented by
	OM_Node * cont_node = NULL , * post_sync_node = NULL, * spawned_func_node = NULL;

/// Create heap memory for the two guaranteed nodes
	cont_node =  Cilk_malloc(sizeof(OM_Node));
	spawned_func_node =  Cilk_malloc(sizeof(OM_Node));

/// Assign uniquee node ID's
	cont_node->id = global_node_count++;
	spawned_func_node->id = global_node_count++;

/// Enter only if this is the first spawn after a sync/in a function
	if (frame->first_spawn_flag != 1){
/// Allocate heap memory
		post_sync_node =  Cilk_malloc(sizeof(OM_Node));

/// Assign unique id
		post_sync_node->id = global_node_count++;

/// Set first_spawn_flag to indicate the next spawn will not be the first, barring
/// that a sync occurs.
		frame->first_spawn_flag = 1;

/// Set the frame's post sync node to this node. This ensures we can keep track of the node
/// if the child that is spawned creates its own tree beneath it.
		frame->post_sync_node = post_sync_node;
	}

/// Asserts we have a valid (non-null) frame current node before we start inserting
	CILK_ASSERT(ws, frame->current_node != NULL);

	;/* Debug messages
		if (FAST_NOT_SLOW)//TODO
		printf("Debug: OM_DS_before_spawn_fast called currnt node id: %d\n", frame->current_node->id);
		else
		printf("Debug: OM_DS_before_spawn_slow called currnt node id: %d\n", frame->current_node->id);
		;*/

/// Insert {current, spawned function, continuation node} into the english OM_DS

#ifdef OM_IS_LL
	OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, spawned_func_node, 	ENGLISH_ID);
	OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, cont_node, 			ENGLISH_ID);
	if (post_sync_node) OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, post_sync_node,	ENGLISH_ID);

/// Insert {current, continuation node, spawned function} into the hebrew OM_DS
	OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, cont_node, 			HEBREW_ID);
	OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, spawned_func_node, 				HEBREW_ID);
	if (post_sync_node) OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, post_sync_node, HEBREW_ID);
/// If we had updates to post_sync_node, reset the frame's post_sync_node
#elif defined OM_IS_BENDER

	#ifdef BATCHIFY_WORKING
		OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, spawned_func_node, 	ENGLISH_ID);
		OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, cont_node, 			ENGLISH_ID);
		if (post_sync_node) OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, post_sync_node,	ENGLISH_ID);

		/// Insert {current, continuation node, spawned function} into the hebrew OM_DS
		OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, cont_node, 			HEBREW_ID);
		OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, spawned_func_node, 				HEBREW_ID);
		if (post_sync_node) OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, post_sync_node, HEBREW_ID);
	#else 

		int rebalance_needed = 0;
		if (OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, spawned_func_node, 	ENGLISH_ID))
		{
			Split_and_add_to_top( WS_TOP_LIST, spawned_func_node->ds_e);
			rebalance_needed = 1;
		}
		if (OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, cont_node, 			ENGLISH_ID))
		{
			Split_and_add_to_top( WS_TOP_LIST, cont_node->ds_e);
			rebalance_needed = 1;
		}
		if (post_sync_node) 
			if (OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, post_sync_node,	ENGLISH_ID))
			{
				Split_and_add_to_top( WS_TOP_LIST, post_sync_node->ds_e);
				rebalance_needed = 1;
			}

	/// Insert {current, continuation node, spawned function} into the hebrew OM_DS
		if (OM_DS_insert(ws, /*WS_REF_DS,*/ frame->current_node, cont_node, 			HEBREW_ID))
		{
			Split_and_add_to_top( WS_TOP_LIST, cont_node->ds_h);
			rebalance_needed = 1;
		}

		if (OM_DS_insert(ws, /*WS_REF_DS,*/ cont_node, spawned_func_node, 				HEBREW_ID))
		{
			Split_and_add_to_top( WS_TOP_LIST, spawned_func_node->ds_h);
			rebalance_needed = 1;
		}

		if (post_sync_node) 
			if (  OM_DS_insert(ws, /*WS_REF_DS,*/ spawned_func_node, post_sync_node, HEBREW_ID))
			{
				Split_and_add_to_top( WS_TOP_LIST, post_sync_node->ds_h);
				rebalance_needed = 1;
			}

		/// Rebalance all remaining lists that need to be rebalanced
		if (rebalance_needed) /// This will changed when we batchify it
			Rebalance_bottom_lists(ws, WS_TOP_LIST);
		/// End else for (if BATCHIFY_WORKING)
#endif 

#endif
	if (post_sync_node) frame->post_sync_node = post_sync_node;

/// Move the current node to the continuaion node
/// &
/// Update the worker state's current node so any calls to Race_detect_{read,write} have
/// the most current value.
	frame->current_node = ws->current_node = cont_node;

/// Update the next spawned node so when the function is actually spawned and it looks to the
/// frame above it on the stack (which is this frame) it can locate its current frame node.
	frame->next_spawned_node = spawned_func_node;

/// Used for debug
	;//printList(/*WS_REF_DS,*/ ENGLISH_ID);
	;//printList(/*WS_REF_DS,*/ HEBREW_ID);


}

/// After a sync in a slow clone, execute this function.
void OM_DS_sync_slow(CilkWorkerState *const ws, CilkStackFrame *frame){
	/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
		//	    printf("Debug: In batch node, no race detect needed");
		return; //then in batcher
	}

	/// For debug
	;//printf("Debug: OM_DS_sync_slow, current frame id: %d\n",  frame->current_node->id );

	/// If the sync was legitimate, then reset frame and worker state to post_sync_node.
	CILK_ASSERT(ws, frame->post_sync_node != NULL);

	/// Assign frame/worker state's current node to the post sync node
	frame->current_node = ws->current_node = frame->post_sync_node;

	/// Reset the frame's next_spawned_node, since to use this we would need to spawn again.
	/// At that point a new one will be created
	frame->next_spawned_node = NULL;

	/// Reset spawn flag
	frame->first_spawn_flag = 0;
}


/// After a sync in a fast clone, execute this function.
void OM_DS_sync_fast(CilkWorkerState *const ws, CilkStackFrame *frame){
	/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
		//	    printf("Debug: In batch node, no race detect needed");
		return;
	}

	/// For debug
	;//printf("Debug: OM_DS_sync_slow, current frame id: %d\n",  frame->current_node->id );

	/// If the sync was legitimate, then reset frame and worker state to post_sync_node.
	CILK_ASSERT(ws, frame->post_sync_node != NULL);

	/// Assign frame/worker state's current node to the post sync node
	frame->current_node = ws->current_node = frame->post_sync_node;

	/// Reset the frame's next_spawned_node, since to use this we would need to spawn again.
	/// At that point a new one will be created
	frame->next_spawned_node = NULL;

	/// Reset spawn flag
	frame->first_spawn_flag = 0;
}

/// After a spawn is finished, update the worker state to match the frame
inline void OM_DS_after_spawn_fast(CilkWorkerState *const ws, CilkStackFrame *frame){
	/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
		//	    printf("Debug: In batch node, no race detect needed");
		return;
	}

	ws->current_node = frame->current_node;
}
/// After a spawn is finished, update the worker state to match the frame
inline void OM_DS_after_spawn_slow(CilkWorkerState *const ws, CilkStackFrame *frame){
	/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
		//	    printf("Debug: In batch node, no race detect needed");
		return;
	}

	ws->current_node = frame->current_node;
}

/// Start a new thread: reset first spawn flag
void OM_DS_new_thread_start(CilkWorkerState *const ws, CilkStackFrame *frame){
	/// Exit function immediately if a batch node
	if  (ws->batch_id != 0)
	{
		//    printf("Debug: In batch node, no race detect needed");
		return;
	}

	if (!(frame->current_node)) //this frame has not been entered yet
		frame->first_spawn_flag = 0;

	ws->current_node = frame->current_node;
}
/**************************************************************!
 *        === Race detect functions in particular ===         *
 **************************************************************/

/*! Initializes the lock for a RD_Memory_struct
  \param ws The current workerstate upon being called
  \param mem The struct whose mutex is being initialized
*/
static void RD_mutex_init(CilkWorkerState * const ws, RD_Memory_Struct * mem)
{
	/*printf("Debug: RD_mutex_init\n");*/
	Cilk_mutex_init(ws->context, &(mem->mutex) );
}

/*! Frees the allocated memory for the lock for RD_Memory_Struct
  \param ws The current workerstate upon being called
  \param mem The struct whose mutex is being initialized
*/
static void RD_mutex_destroy(CilkWorkerState * const ws, RD_Memory_Struct * mem)
{
	/*printf("Debug: RD_mutex_destroy\n");*/
	Cilk_mutex_destroy(ws->context, &(mem->mutex) );
}

/*! Creates the structure upon the call to this function and returns a pointer
  to the address in memory to be utilized in place of the desired variable
  \param ws The CilkWorkerState of the given program
  \param size The size of the data type to read/written
  \return the memory location of the RD_Memory_Struct
*/
void * RD_structure_create(CilkWorkerState * const ws, size_t size)
{
	RD_Memory_Struct * memPtr;
	/*printf("Debug: RD_structure_create\n");*/
	memPtr = Cilk_malloc(sizeof(RD_Memory_Struct));

	//!Inialize known members
	memPtr->size = size;
	memPtr->data = Cilk_malloc(size);

	//!Initialize the lock
	RD_mutex_init(ws, memPtr);

	return (void *)memPtr;
}

void RD_free(CilkWorkerState * const ws, void * mem)
{
	RD_Memory_Struct * memptr;
	/*printf("Debug: RD_free\n");*/
	memptr = (RD_Memory_Struct*)mem;

	//!First free data held in struct
	Cilk_free(memptr->data);
	memptr->data = NULL; //!< Prevents dangling pointers

	//! Clear the lock
	RD_mutex_destroy(ws, memptr);

	//!Finally, free the struct itself
	Cilk_free(mem);
	mem = NULL; //!< Prevents dangling pointers
}


/*! Function that detects potential races on a given memory read
  \param ws CilkWorkerState Node for program
  \param memPtr The memory address of the struct used in checking
  \return memory address of read location
*/
void Race_detect_read_b(CilkWorkerState * const ws,
						const void * memPtr,
						void * holder,
						const char * func_name,
						const int line_num,
						int * rd_result)
{

	//!Get struct
	RD_Memory_Struct * mem;
	/*printf("Debug: Race_detect_Read\n");*/
	mem = (RD_Memory_Struct *)memPtr;

	//!Get lock
	Cilk_mutex_wait(ws->context, ws,  &(mem->mutex) );
	;//printf("Debug: Got lock - RD_read in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);

	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first read-node checked
	if( (mem->left_r == NULL) && (mem->right_r == NULL) )
	{
		;//printf("Debug: Initalizes l&r read ptrs\n");
		//! Initalize ptrs for struct
		mem->left_r = mem->right_r = currentNode;
	}

	/*! Check if there is a race:
	 * Race if another write occurs in parallel
	 * (1)
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 */
	if(
		(
			(mem->left_w != NULL)
			&&
			(
				//(1)
				(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, HEBREW_ID))
				||  //(2)
				(OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, HEBREW_ID))
				)
			)
		||
		(
			(mem->right_w != NULL)
			&&
			(
				//(3)
				(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, HEBREW_ID))
				||  //(4)
				(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, HEBREW_ID))
				)
			)
		)
	{
		//!Make boolean true
		*rd_result = 1;

		//!Print that there's a race and continue
		printf("Detected Race: Read on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);
	}
	else
		*rd_result = 0; //!< Make the bool 0

	;//printf("Debug: Left_r: %i  right_r: %i and current: %i\n", mem->left_r->id, mem->right_r->id, currentNode->id);
	//! Update nodes (if necessary)
	if(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID))
		mem->left_r = currentNode;
	if(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID))
		mem->right_r = currentNode;

	//! Write the data into holder
	memcpy(holder, mem->data, mem->size);

	;//printf("Debug: About to release lock - RD_read in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//!No race, release lock
	Cilk_mutex_signal(ws->context, &(mem->mutex) );

	return;

}

/*! Function that detects potential races on a given memory read
  \param ws CilkWorkerState Node for program
  \param memPtr The memory address of the struct used in checking
  \return memory address of read location
*/
void Race_detect_read(CilkWorkerState * const ws,
					  const void * memPtr,
					  void * holder,
					  const char * func_name,
					  const int line_num)
{

	//!Get struct
	RD_Memory_Struct * mem;
	/*printf("Debug: Race_detect_Read\n");*/
	mem = (RD_Memory_Struct *)memPtr;

	//!Get lock
	Cilk_mutex_wait(ws->context, ws,  &(mem->mutex) );
	;//printf("Debug: Got lock - RD_read in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);

	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first read-node checked
	if( (mem->left_r == NULL) && (mem->right_r == NULL) )
	{
		//! Initalize ptrs for struct
		mem->left_r = mem->right_r = currentNode;
	}

	/*! Check if there is a race:
	 * Race if another write occurs in parallel
	 * (1)
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 */
	if(
		(
			(mem->left_w != NULL)
			&&
			(
				//(1)
				(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, HEBREW_ID))
				||  //(2)
				(OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, HEBREW_ID))
				)
			)
		||
		(
			(mem->right_w != NULL)
			&&
			(
				//(3)
				(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, HEBREW_ID))
				||  //(4)
				(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID) &&
				 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, HEBREW_ID))
				)
			)
		)
	{
		//!Print that there's a race and continue
		printf("Detected Race: Read on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);
	}

	//! Update nodes (if necessary)
	if(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID))
		mem->left_r = currentNode;
	if(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID))
		mem->right_r = currentNode;

	//! Write the data into holder
	memcpy(holder, mem->data, mem->size);

	;//printf("Debug: About to release lock - RD_read in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//!No race, release lock
	Cilk_mutex_signal(ws->context, &(mem->mutex) );

	return;

}

/*! Function that detects potential races on a given memory write
  \param ws CilkWorkerState Node for program
  \param memPtr Pointer to the memory address of the struct utilized (mem)
  \param writeValue Manually passed in value to be written to mem->data
*/
void Race_detect_write_b(CilkWorkerState * const ws,
						 void * memPtr,
						 const void * writeValue,
						 const char *func_name,
						 const int line_num,
						 int * rd_result)
{

	//!Get struct
	RD_Memory_Struct * mem;
	/*printf("Race_detect_write\n");*/
	mem = (RD_Memory_Struct *)memPtr;

	//!Get Lock
	Cilk_mutex_wait(ws->context, ws, &(mem->mutex) );

	;//printf("Debug: Got lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first write-node checked
	if( (mem->left_w == NULL) && (mem->right_w == NULL) )
	{
		;//printf("Debug: Initalizes l&r write ptrs\n");

		//!Inialize ptrs for struct
		mem->left_w = mem->right_w = currentNode;

		/*! ****Fuller Explanation of Race Detection Conditions Below****
		 * In the event that the first write node is encounterd, races must be
		 * check only with read nodes, for the conditionals below will detect a
		 * "race" with itself (since at this point the left=right=current).
		 * As a result, we check for races within this if and write instead
		 */
		if(
			(
				(mem->left_r != NULL)
				&&
				(
					//(5)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, HEBREW_ID))
					||  //(6)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_r != NULL)
				&&
				(
					//(7)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, HEBREW_ID))
					||  //(8)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, HEBREW_ID))
					)
				)
			)
		{
			//! Print the race
			printf("Detected Race: Write on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);

			//!Make boolean true
			*rd_result = 1;

			//! Write the data
			memcpy( mem->data, writeValue, mem->size);

			;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
			//! Have to release lock
			Cilk_mutex_signal(ws->context, &(mem->mutex) );

			return;
		} else {

			//!Make boolean false
			*rd_result = 0;

			//! Write the data
			memcpy( mem->data, writeValue, mem->size);

			;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
			//! Have to release lock
			Cilk_mutex_signal(ws->context, &(mem->mutex) );

			return;
		}
	}

	/*! Check if there is a race:
	 * Race if another write/read occurs in parallel
	 * (1)   == WRITES ==
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (5) or  == READS ==
	 *   if the currentNode is before leftmost read in eng and
	 *      the currentNode is after leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (6) or
	 *   if the currentNode is after leftmost read in eng and
	 *      the currentNode is before leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (7) or
	 *   if the currentNode is before rightmost read in eng and
	 *      the currentNode is after rightmost read in heb,
	 *      then they are in parallel => race condition
	 * (8) or
	 *   if the currentNode is after rightmost read in eng and
	 *      the currentNode is before rightmost read in heb,
	 *      then they are in parallel => race condition
	 */
	if(

		//! Check the writes for races
		(
			(
				(mem->left_w != NULL)
				&&
				(
					//(1)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, HEBREW_ID))
					||  //(2)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_w != NULL)
				&&
				(
					//(3)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, HEBREW_ID))
					||  //(4)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, HEBREW_ID))
					)
				)
			)

		||

		//! Now check the reads for races
		(
			(
				(mem->left_r != NULL)
				&&
				(
					//(5)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, HEBREW_ID))
					||  //(6)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_r != NULL)
				&&
				(
					//(7)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, HEBREW_ID))
					||  //(8)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, HEBREW_ID))
					)
				)
			)
		)
	{
		//! Print the race
		printf("Detected Race: Write on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);

		//!Make boolean true
		*rd_result = 1;
	}
	else
		*rd_result = 0; //!< Make bool 0

	;//printf("Debug: Left_w: %i  right_w: %i and current: %i\n", mem->left_w->id, mem->right_w->id, currentNode->id);
	//! Update nodes (if necessary)
	if(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID))
		mem->left_w = currentNode;
	if(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID))
		mem->right_w = currentNode;

	//! Write the data
	memcpy( mem->data, writeValue, mem->size);


	;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//!Release Lock
	Cilk_mutex_signal(ws->context, &(mem->mutex) );

}

/*! Function that detects potential races on a given memory write
  \param ws CilkWorkerState Node for program
  \param memPtr Pointer to the memory address of the struct utilized (mem)
  \param writeValue Manually passed in value to be written to mem->data
*/
void Race_detect_write(CilkWorkerState * const ws,
					   void * memPtr,
					   const void * writeValue,
					   const char *func_name,
					   const int line_num)
{

	//!Get struct
	RD_Memory_Struct * mem;
	/*printf("Race_detect_write\n");*/
	mem = (RD_Memory_Struct *)memPtr;

	//!Get Lock
	Cilk_mutex_wait(ws->context, ws, &(mem->mutex) );

	;//printf("Debug: Got lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//! Retrieve currentNode from workerstate
	OM_Node * currentNode = ws->current_node;

	//! This is only true when it is the first write-node checked
	if( (mem->left_w == NULL) && (mem->right_w == NULL) )
	{
		//!Inialize ptrs for struct
		mem->left_w = mem->right_w = currentNode;

		/*! ****Fuller Explanation of Race Detection Conditions Below****
		 * In the event that the first write node is encounterd, races must be
		 * check only with read nodes, for the conditionals below will detect a
		 * "race" with itself (since at this point the left=right=current).
		 * As a result, we check for races within this if and write instead
		 */
		if(
			(
				(mem->left_r != NULL)
				&&
				(
					//(5)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, HEBREW_ID))
					||  //(6)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_r != NULL)
				&&
				(
					//(7)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, HEBREW_ID))
					||  //(8)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, HEBREW_ID))
					)
				)
			)
		{
			//! Print the race
			printf("Detected Race: Write on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);

			//! Write the data
			memcpy( mem->data, writeValue, mem->size);

			;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
			//! Have to release lock
			Cilk_mutex_signal(ws->context, &(mem->mutex) );

			return;
		} else {

			//! Write the data
			memcpy( mem->data, writeValue, mem->size);


			;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
			//! Have to release lock
			Cilk_mutex_signal(ws->context, &(mem->mutex) );

			return;
		}
	}

	/*! Check if there is a race:
	 * Race if another write/read occurs in parallel
	 * (1)   == WRITES ==
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (5) or  == READS ==
	 *   if the currentNode is before leftmost read in eng and
	 *      the currentNode is after leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (6) or
	 *   if the currentNode is after leftmost read in eng and
	 *      the currentNode is before leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (7) or
	 *   if the currentNode is before rightmost read in eng and
	 *      the currentNode is after rightmost read in heb,
	 *      then they are in parallel => race condition
	 * (8) or
	 *   if the currentNode is after rightmost read in eng and
	 *      the currentNode is before rightmost read in heb,
	 *      then they are in parallel => race condition
	 */
	if(

		//! Check the writes for races
		(
			(
				(mem->left_w != NULL)
				&&
				(
					//(1)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, HEBREW_ID))
					||  //(2)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_w, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_w != NULL)
				&&
				(
					//(3)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, HEBREW_ID))
					||  //(4)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_w, HEBREW_ID))
					)
				)
			)

		||

		//! Now check the reads for races
		(
			(
				(mem->left_r != NULL)
				&&
				(
					//(5)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, HEBREW_ID))
					||  //(6)
					(OM_DS_order(/*WS_REF_DS,*/ mem->left_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_r, HEBREW_ID))
					)
				)
			||
			(
				(mem->right_r != NULL)
				&&
				(
					//(7)
					(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, HEBREW_ID))
					||  //(8)
					(OM_DS_order(/*WS_REF_DS,*/ mem->right_r, currentNode, ENGLISH_ID) &&
					 OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->right_r, HEBREW_ID))
					)
				)
			)
		)
	{
		//! Print the race
		printf("Detected Race: Write on Memory Address{%p} in function %s at line %d\n", mem, func_name, line_num);
	}

	//! Update nodes (if necessary)
	if(OM_DS_order(/*WS_REF_DS,*/ currentNode, mem->left_w, ENGLISH_ID))
		mem->left_w = currentNode;
	if(OM_DS_order(/*WS_REF_DS,*/ mem->right_w, currentNode, ENGLISH_ID))
		mem->right_w = currentNode;

	//! Write the data
	memcpy( mem->data, writeValue, mem->size);

	;//printf("Debug: about to release lock - RD_write in node (%i) on memloc (%p) \n", ws->current_node->id, memPtr);
	//!Release Lock
	Cilk_mutex_signal(ws->context, &(mem->mutex) );

}

/* ============= End Order Maintenence Functions ============= */

/*
 * initialization of the scheduler.
 */
void Cilk_scheduler_init(CilkContext *const context)
{
	CILK_CHECK(USE_PARAMETER1(active_size) > 0,
			   (context, NULL, "Partition size must be positive\n"));
	create_deques(context);
	Cilk_internal_malloc_global_init(context);
	Cilk_internal_malloc_global_init_2(context);
}

void Cilk_scheduler_terminate(CilkContext *const context)
{
	Cilk_internal_malloc_global_terminate_2(context);
	Cilk_internal_malloc_global_terminate(context);
	free_deques(context);
}

void Cilk_scheduler_init_2(CilkContext *const context)
{
	USE_SHARED1(done) = 0;
	init_deques(context);
	/*Cilk_internal_malloc_global_init_2(context); */
}

void Cilk_scheduler_terminate_2(CilkContext *const UNUSED(context))
{
	/*Cilk_internal_malloc_global_terminate_2(context); */
}

void Cilk_scheduler_per_worker_init(CilkWorkerState *const ws)
{
	WHEN_CILK_TIMING(ws->cp_hack = 0);
	WHEN_CILK_TIMING(ws->work_hack = 0);

	ws->cache.stack =
		Cilk_malloc_fixed(USE_PARAMETER(options->stackdepth) *
						  sizeof(CilkStackFrame *));
	ws->ds_cache.stack =
		Cilk_malloc_fixed(USE_PARAMETER(options->stackdepth) *
						  sizeof(CilkStackFrame *));
	ws->stackdepth = USE_PARAMETER(options->stackdepth);
	Cilk_switch2core(ws);

	Cilk_reset_stack_depth_stats(ws);

	CILK_CHECK(ws->cache.stack, (ws->context, ws, "failed to allocate stack\n"));
	CILK_CHECK(ws->ds_cache.stack, (ws->context, ws, "failed to allocate stack\n"));
}

void Cilk_scheduler_per_worker_terminate(CilkWorkerState *const ws)
{
	Cilk_free(ws->cache.stack);
	Cilk_free(ws->ds_cache.stack);
}

/*
 * From the frame of <cl>, which should be the invoke_main closure,
 * get the inlet that should be executed when cilk_main returns.
 * (All the inlet does, is to set the proper return value and call
 * abort).
 * Pass <res>, the exit value, to the inlet, and enque it as
 * a complete inlet.
 */
static void make_exit_inlet_closure(CilkWorkerState *const ws,
									Closure *cl, int res)
{
	struct InletClosure *p;

	p = make_incomplete_inlet_closure(ws, cl, 1);

	memcpy(p->inlet_args, &res, sizeof(int));
	p->next = cl->complete_inlets;
	cl->complete_inlets = p;
}

/*
 * This procedure is called by Cilk_exit().
 * <cl> is the invoke_main closure, and <res> is the return
 * value passed in by Cilk_exit.
 *
 * The real work is done by first enqueing an inlet that calls
 * abort, and then either signal exception or polls the inlet.
 * So, cilk_main, which is spawned by invoke_main will be
 * aborted.
 * (Refer to "invoke_main.c" for details.)
 */
void Cilk_exit_from_user_main(CilkWorkerState *const ws, Closure *cl, int res)
{
	Closure_assert_alienation(ws, cl);

	Closure_lock(ws, cl);
	make_exit_inlet_closure(ws, cl, res);

	if (cl->status == CLOSURE_RUNNING)
		signal_immediate_exception(ws, cl);
	else
		poll_inlets(ws, cl);

	Closure_unlock(ws, cl);
}
