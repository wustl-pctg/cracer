/** Lower-level linked-list for order-maintenance. */
#ifndef _BLIST_H
#define _BLIST_H

#include <stdbool.h> // bool
#include <stdio.h>

struct blist_s;
typedef struct blist_s blist;

struct blist_node_s {
  label_t label;
  struct blist_node_s* next;
  struct blist_node_s* prev;
  struct blist_s* list; // Needed for node comparison
};

typedef struct blist_node_s bl_node;

/** Initialize a pre-allocated list. */
void bl_create(blist* self);

/** Allocate a list from the heap and initialize.
  * @return An empty list.
*/
blist* bl_new();

/** Destroy, but do not de-allocate, list. */
void bl_destroy(blist* self);

/** Destroy and deallocate list. */
void bl_free(blist* self);

/** Insert a new item into the order.
 *  @param base Node after which to insert.
 *  @return A pointer to the new node.
 */
bl_node* bl_insert(blist* self, bl_node* base);
bl_node* bl_insert_initial(blist* self);

/** Returns true if x precedes y in the order, false otherwise. */
bool bl_precedes(const bl_node* x, const bl_node* y);

/** Verify all labels are in the correct order. */
int bl_verify(const blist* self);

size_t bl_size(const blist* self);

/** Print list to a file. */
void bl_fprint(const blist* self, FILE* out);

/** Print list to standard output. */
void bl_print(const blist* self);

#endif // ifndef _BLIST_H
