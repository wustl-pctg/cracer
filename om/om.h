/** An efficient order-maintenance data structure meant to be used
    with Batcher.
*/
#ifndef _OM_H
#define _OM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h> // bool
#include <stdio.h>

#include "om_common.h"
#include "blist.h"

struct tl_node_s;
typedef struct tl_node_s tl_node;
struct om_s;
typedef struct om_s om;

typedef bl_node om_node;

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
om_node* om_insert_initial(om* self);

/** Returns true if x precedes y in the order, false otherwise. */
bool om_precedes(const om_node* x, const om_node* y);

tl_node* om_get_tl(om_node* n);

/** Verify all labels are in the correct order and struct is valid. */
int om_verify(const om* self);

/** Print list to a file. */
void om_fprint(const om* self, FILE* out);

/** Print list to standard output. */
void om_print(const om* self);

void om_relabel(om* self, tl_node** heavy_lists, size_t num_heavy_lists);

#ifdef __cplusplus
} // extern C
#endif

#endif // ifndef _OM_H
