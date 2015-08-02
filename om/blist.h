/** Lower-level linked-list for order-maintenance. */
#ifndef _BLIST_H
#define _BLIST_H

#include <stdbool.h> // bool

struct blist_s;
typedef struct blist_s blist;

struct blist_node_s;
typedef struct blist_node_s bl_node;

/** Allocate a list from the heap and initialize.
  * @return An empty list.
*/
blist* bl_new();

/** Destroy and deallocate list. */
void bl_free(blist* self);

/** Initialize a pre-allocated list. */
void bl_create(blist* self);

/** Destroy, but do not de-allocate, list. */
void bl_destroy(blist* self);

/** Insert a new item into the order.
 *  @param base Node after which to insert.
 *  @return A pointer to the new node.
 */
bl_node* bl_insert(blist* self, bl_node* base);

/** Returns true if x precedes y in the order, false otherwise. */
bool bl_precedes(const bl_node* x, const bl_node* y);

/** Verify all labels are in the correct order. */
void bl_verify(const blist* self);

/** Print list to a file. */
void bl_fprint(const blist* self, FILE* out);

/** Print list to standard output. */
void bl_print(const blist* self);

#endif // ifndef _BLIST_H
