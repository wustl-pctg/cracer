/** An efficient order-maintenance data structure meant to be used
    with Batcher.
*/
#ifndef _OM_H
#define _OM_H

#include <stdbool.h> // bool

typedef unsigned long label_t;
#include "blist.h"

struct tl_node_s;
typedef struct tl_node_s tl_node;
struct om_s;
typedef struct om_s om;

typedef bl_node om_node;

size_t g_marked_array_size = 0;
tl_node** g_marked_array = NULL;

/** Allocate a tree from the heap and initialize.
  * @return An empty tree.
*/
om* om_new();

/** Initialize a pre-allocated list. */
void om_create(om* self);

/** Destroy and deallocate list. */
void om_free(om* self);

/** Destroy, but do not de-allocate, list. */
void om_destroy(om* self);

om_node* om_insert(om* self, om_node* base);

/** Returns true if x precedes y in the order, false otherwise. */
bool om_precedes(const om_node* x, const om_node* y);

/** Verify all labels are in the correct order. */
void om_verify(const om* self);

/** Print list to a file. */
void om_fprint(const om* self, FILE* out);

/** Print list to standard output. */
void om_print(const om* self);

#endif // ifndef _OM_H
