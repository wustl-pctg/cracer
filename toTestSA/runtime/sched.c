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
*****************************************
* Order Maintenance for race detection *
*****************************************/
void OM_DS_init(CilkContext *const context){
	if (context->Cilk_global_state){
		printf("Debug: OM_DS_init\n");
		context->Cilk_global_state->hebrewOM_DS = (OM_DS* )Cilk_malloc(sizeof(OM_DS));
		context->Cilk_global_state->englishOM_DS = (OM_DS *) Cilk_malloc(sizeof(OM_DS));
	}
#define OM_IS_LL
}

void OM_DS_free_and_free_nodes(CilkContext *const context){
	//free nodes
#ifdef OM_IS_LL
	OM_LL_free_nodes_internal(context);
#else
	OM_free_nodes_internal(context);
#endif
	//free ds
	printf("Debug: free OMDS\n");
	Cilk_free(context->Cilk_global_state->hebrewOM_DS);
	Cilk_free(context->Cilk_global_state->englishOM_DS);

}

//! frees node if LL is the OM_DS
void OM_LL_free_nodes_internal(CilkContext *const context){
	int i = 0;
	OM_Node  * node, *nextNode;

	printf("DEBUG:LL free nodes\n");
	node = context->Cilk_global_state->englishOM_DS->head;

	while(node != NULL){
		nextNode = node->next;
		Cilk_free(node);
		node = nextNode;
	}
}

void printList(OM_DS * list) {
	OM_Node * n;

	n = list->head;
	printf("Head->");
	while (n != NULL){
	printf("%s->", n->id);
	n = n->next;
	}

	printf("Tail\n");
}

//! frees node if OM_DS is not linked list
void OM_free_nodes_internal(CilkContext *const context)
{printf("DEBUG: OMDS free nodes -- NOT COMPLETED\n");}

void OM_DS_insert(void *ds, void * _x, void * _y){

#ifdef BATCHIFY_WORKING

	InsertRecord * ir = (InsertRecord * ) malloc(sizeof(InsertRecord));
    ir->x = (OM_Node *)_x;
	ir->y = (OM_Node *) _y;

	Cilk_batchify(_cilk_ws, &insertPar, list, ir, sizeof(Node), NULL);
#else
	//Do insert here

	OM_Node * x = (OM_Node *)_x;
	OM_Node * y = (OM_Node *)_y;
	OM_Node * z;

	//if x is null
	if (!(x && y && ds) ){
		printf("Some node or ds is null, skipping insert\n");
		return;
	}

	//if x isnt tail
	if (x->next)
		z = x->next;
	else  //make z null and change y to tail
	{
		z = NULL;
		( (OM_DS*)ds)->tail = y;
	}

	//change next pointers
	y->next = x->next;

	if(!(__sync_bool_compare_and_swap(&(x->next), x->next, y)))
		{
		printf("Exiting, atomic insert failed");
		exit(0);
		}

	((OM_DS*)ds)->size++;
#endif
}

//! Simple append function for when OM_LL is defined
void OM_DS_append(void *ds, void * _x){
	printf("Debug: appending node\n");
#ifdef OM_IS_LL

	if (ds && _x){
		OM_DS * om_ds = (OM_DS *)ds;
		OM_Node * node = (OM_Node*)_x;
		if (om_ds->size == 0)
		{
			om_ds->tail = om_ds->head = node;
			node->next = NULL;
			om_ds->size++;
		}
		else 	{ //if l.l. has nodes already
			om_ds->tail->next = node;
			om_ds->tail = node;
			node->next = NULL;
			om_ds->size++;
		}
	}
	else {
		printf("Debug: appending null node or to null ds\n");
	}
#else
	printf("Debug: Don't know how to append to OM_DS yet\n");
#endif
}

#define WS_REF_ENG ws->context->Cilk_global_state->englishOM_DS
#define WS_REF_HEB ws->context->Cilk_global_state->hebrewOM_DS

int OM_DS_order(void *ds, void * _x, void * _y){
#ifdef OM_IS_LL
	// Really basic order function for ll
	// Assumes both _x and _y are in the list
	OM_Node * current;
	current = ((OM_DS*)ds)->head;

	do {
		if (current == _y)
			return 0;
		else if (current == _x)
			return 1;
		else
			current = current->next;
	} while( current != ((OM_DS*)ds)->tail);

	printf( "Neither node found in linked list. Returning false");

#else
	printf("Debug: Don't know how to order with OM_DS yet\n");
	return 0;
#endif
}

void OM_DS_before_spawn_fast(CilkWorkerState *const ws, CilkStackFrame *frame){

	/*! Create three new nodes to be inserted into OM_DS*/
	OM_Node * cont_node, * post_sync_node, * spawned_func_node;

	cont_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));
	post_sync_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));
	spawned_func_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));

    cont_node->id = global_node_count++;
    spawned_func_node->id = global_node_count++;
    post_sync_node->id = global_node_count++;

	printf("Debug: OM_DS_before_spawn_fast called WS: %p\t Frame: %p\n", ws, frame);
	//there could be redundant post_sync_node, so free it if necessary
	if (frame->post_sync_node){
		//do i need a lock here?
		Cilk_free(frame->post_sync_node);
	}
	/*! insert the new nodes into the OM_DS*/

	OM_DS_insert(WS_REF_ENG, frame->current_node, spawned_func_node);
	OM_DS_insert(WS_REF_ENG, spawned_func_node, cont_node);
	OM_DS_insert(WS_REF_ENG, cont_node, post_sync_node);

	OM_DS_insert(WS_REF_HEB, frame->current_node, cont_node);
	OM_DS_insert(WS_REF_HEB, cont_node, spawned_func_node);
	OM_DS_insert(WS_REF_HEB, spawned_func_node, post_sync_node);

		printf("ENG: "); printList(WS_REF_ENG);
		printf("HEB: "); printList(WS_REF_HEB);
	/*!update frame variables*/
	frame->post_sync_node = post_sync_node;
	frame->current_node = cont_node;

    /*update the worker state variables*/
    ws->current_node = frame->current_node;
    ws->next_func_node = spawned_func_node;
}
/*! The only slow threads are going to either be the main thread or a stolen frame*/
void OM_DS_before_spawn_slow(CilkWorkerState *const ws, CilkStackFrame *frame){

	/*! Create three new nodes to be inserted into OM_DS*/
	OM_Node * cont_node, * post_sync_node, * spawned_func_node;

	cont_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));
	post_sync_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));
	spawned_func_node = (OM_Node *) Cilk_malloc(sizeof(OM_Node));

    cont_node->id = global_node_count++;
    spawned_func_node->id = global_node_count++;
    post_sync_node->id = global_node_count++;

	printf("Debug: OM_DS_before_spawn_slow called WS: %p\t Frame: %p\n", ws, frame);

	//there could be redundant post_sync_node, so free it if necessary
	if (frame->post_sync_node){
		//do i need a lock here?
		Cilk_free(frame->post_sync_node);
	}
	/*! insert the new nodes into the OM_DS*/
	OM_DS_insert(WS_REF_ENG, frame->current_node, spawned_func_node);
	OM_DS_insert(WS_REF_ENG, spawned_func_node, cont_node);
	OM_DS_insert(WS_REF_ENG, cont_node, post_sync_node);

	OM_DS_insert(WS_REF_HEB, frame->current_node, cont_node);
	OM_DS_insert(WS_REF_HEB, cont_node, spawned_func_node);
	OM_DS_insert(WS_REF_HEB, spawned_func_node, post_sync_node);

	/*!update frame variables*/
	frame->post_sync_node = post_sync_node;
	frame->current_node = cont_node;
    
    /*! update worker state*/
    ws->current_node = frame->current_node;
    ws->next_func_node = spawned_func_node;
}

void OM_DS_sync_slow(CilkWorkerState *const ws, CilkStackFrame *frame){
	printf("Debug: OM_DS_sync_slow WS: %p\t Frame: %p\n", ws, frame);

	if (frame->post_sync_node)
		frame->current_node = frame->post_sync_node;
    else
		printf("No post sync node \n");
}


void OM_DS_sync_fast(CilkWorkerState *const ws, CilkStackFrame *frame){
	printf("Debug: OM_DS_sync_fast WS: %p\t Frame: %p\n", ws, frame);

	/*update frame varriables*/
	if (frame->post_sync_node)
		frame->current_node = frame->post_sync_node;
    else
		printf("No post sync node \n");
}

//a new thread is started, get the next_function_node from the ws and put as the
//current node
void OM_DS_new_thread_start(CilkWorkerState *const ws, CilkStackFrame *frame){

		printf("New thread start.\n");
		frame->current_node = ws->next_func_node;

}


/*! === Race detect functions in particular === */

//! Initializes the lock for a RD_Memory_struct
void RD_mutex_init(CilkWorkerState * const ws, RD_Memory_Struct * mem) {
	Cilk_mutex_init(ws->context, mem->mutex); 
}

//! Frees the allocated memory for the lock for RD_Memory_Struct 
void RD_mutex_destroy(CilkWorkerState * const ws, RD_Memory_Struct * mem) {
	Cilk_mutex_destroy(ws->context, mem->mutex);
}

/*! Function that detects potential races on a given memory read
  \param ws CilkWorkerState Node for program
  \param memloc The variable to be read
*/
void * Race_detect_read(CilkWorkerState * const ws, RD_Memory_Struct * mem) {

	
	//! Retrieve currentNode from workerstate
	OM_Node * currentNode;
	currentNode = ws->current_node;

	//! This is only true when it is the first read-node checked
	if( !(mem->left_r && mem->right_r) )
	{
		mem->left_r = mem->right_r = currentNode;
		return mem->memloc;
	}
	
	/*! Check if there is a race:
	 * Race if another write occurs in parallel
	 * (1)
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (4) or
	 *   if the currentNode is after rightmost write in eng and
	 *      the currentNode is before rightmost write in heb,
	 *      then they are in parallel => race condition
	 */
	if(    //(1)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w) &&
		 OM_DS_order(WS_REF_HEB, mem->left_w, currentNode))
		||  //(2)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_w) &&
		 OM_DS_order(WS_REF_HEB, mem->right_w, currentNode))
		||  //(3)
		(OM_DS_order(WS_REF_ENG, mem->left_w, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_w))
		||  //(4)
		(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_w))
		) { exit(0); } // Halt program
	//TODO ========== THROW RACE DETECTION ===== FIGURE THIS OUT

	//! Update nodes (if necessary)
	//TODO: Is this wrong? The logic may not be correct for reads... they may be parallel
	// but that would be mean updating may be incorrect.
	if(OM_DS_order(WS_REF_ENG, currentNode, mem->left_r))
		mem->left_r = currentNode;
	if(OM_DS_order(WS_REF_ENG, mem->right_r, currentNode))
		mem->right_r = currentNode;

	//! Read the data
	return mem->memloc;

}

/*! Function that detects potential races on a given memory write
  \param ws CilkWorkerState Node for program
  \param memloc The variable to be written
*/
void Race_detect_write(CilkWorkerState * const ws, RD_Memory_Struct * mem, const void * writeValue, size_t writeValueTypeSize) {

	//! Retrieve currentNode from workerstate
	OM_Node * currentNode;
	currentNode = ws->current_node;

	//! This is only true when it is the first write-node checked
	if( !(mem->left_w && mem->right_w) )
	{
		mem->left_w = mem->right_w = currentNode;
		return;
	}
	
	/*! Check if there is a race:
	 * Race if another write/read occurs in parallel
	 * (1)   == WRITES ==
	 *   if the currentNode is before leftmost write in eng and
	 *      the currentNode is after leftmost write in heb,
	 *      then they are in parallel => race condition
	 * (2) or
	 *   if the currentNode is before rightmost write in eng and
	 *      the currentNode is after rightmost write in heb,
	 *      then they are in parallel => race condition
	 * (3) or
	 *   if the currentNode is after leftmost write in eng and
	 *      the currentNode is before leftmost write in heb,
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
	 *   if the currentNode is before rightmost read in eng and
	 *      the currentNode is after rightmost read in heb,
	 *      then they are in parallel => race condition
	 * (7) or
	 *   if the currentNode is after leftmost read in eng and
	 *      the currentNode is before leftmost read in heb,
	 *      then they are in parallel => race condition
	 * (8) or
	 *   if the currentNode is after rightmost read in eng and
	 *      the currentNode is before rightmost read in heb,
	 *      then they are in parallel => race condition
	 */
	if(    //(1)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w) &&
		 OM_DS_order(WS_REF_HEB, mem->left_w, currentNode))
		||  //(2)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_w) &&
		 OM_DS_order(WS_REF_HEB, mem->right_w, currentNode))
		||  //(3)
		(OM_DS_order(WS_REF_ENG, mem->left_w, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_w))
		||  //(4)
		(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_w))
		||  //(5)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->left_r) &&
		 OM_DS_order(WS_REF_HEB, mem->left_r, currentNode))
		||  //(6)
		(OM_DS_order(WS_REF_ENG, currentNode, mem->right_r) &&
		 OM_DS_order(WS_REF_HEB, mem->right_r, currentNode))
		||  //(7)
		(OM_DS_order(WS_REF_ENG, mem->left_r, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->left_r))
		||  //(8)
		(OM_DS_order(WS_REF_ENG, mem->right_r, currentNode) &&
		 OM_DS_order(WS_REF_HEB, currentNode, mem->right_r))
		) { exit(0); } // Halt program
	/* ========== THROW RACE DETECTION ===== FIGURE THIS OUT */
	//TODO: Decide how to interrupt for race-detection

	//! Update nodes (if necessary)
	if(OM_DS_order(WS_REF_ENG, currentNode, mem->left_w))
		mem->left_w = currentNode;
	if(OM_DS_order(WS_REF_ENG, mem->right_w, currentNode))
		mem->right_w = currentNode;

	//! Write the data
	memcpy(&(mem->memloc),&writeValue,writeValueTypeSize);
	
}
/* End Order Maintenence Functions */

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
