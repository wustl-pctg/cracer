#include <stdlib.h>
#include "trie.h"

#define BITS_NUMBER sizeof(unsigned int) * 8
#define MAX_BIT 1U << (BITS_NUMBER - 1)

void printBits(unsigned int num)
{
    short size = sizeof(unsigned int);
    short maxPow = 1 << (size * 8-1);
    int i = 0;
	for(; i < size*8; ++i){
		// print last bit and shift left.
		printf("%u ", num&maxPow ? 1 : 0);
		num = num << 1;
	}
	printf("\n");
}

trie_node * initialize_trie()
{
	trie_node * trie = malloc(sizeof(trie));
	trie->root = trie;
	trie->value = 0;
	trie->level = 0;
	trie->left = trie->right = NULL;
	return trie;
}

void insert(trie_node * x, trie_node * y) 
{
	// Check for first insert
	if (x->level == 0) 
	{
		y->level = 1;

		if (x->left != NULL)
		{
			x->left = y;

			// Value is 0
			y->value = 0x0;
			
			y->parent = x;
		}
		else
		{
			// Error
			exit();
		}

		
	}
	else if (x->level == BITS_NUMBER)
	{
		// Split should have happened, error
		exit();
	}
	else
	{
		// Check to see if it is the second insert - start second side of trie
		if ((x->level == 1 &&  x->parent->right != NULL) 
		{
			// Update pointers, level, value
			x->parent->right = y;
			y->level = 1;
			y->value = 0x1;
			y->parent = x->parent;
		}
		else
		{
			// Make a dummy node to take the place of x before it shifts left
			trie_node * new_x = malloc(sizeof(new_x));

			new_x = x;

			// Update the levels of each
			y->level = x->level + 1;
			++(x->level); // The original node is also it's left node

			// Shift to the left, add 1
			y->value = (x->value<<1) + 0x1;

			// Shift the original one
			x->value = (x->value<<1);

			// Adjust pointers
			new_x->right = y;
			new_x->left = x;

			if (y->level == BITS_NUMBER)
			{
				// split
				return;
			}
		}
	}
}











/*

void printBits(short num)
{
    short size = sizeof(short);
    short maxPow = 1 << (size * 8-1);
    int i = 0;
	for(; i < size*8; ++i){
		// print last bit and shift left.
		printf("%u ", num&maxPow ? 1 : 0);
		num = num << 1;
	}
	printf("\n");
}

void create_node(trie_node **node, short value) {
	*node = malloc(sizeof(trie_node));
	(*node)->value = value;
	(*node)->left = 0;
	(*node)->right = 0;
}

void insert(short number, trie_node **head) {
	int i, digit;
	if (!(*head)) {
		create_node(head, -1);
	}
	trie_node *current = *head;
	for (i = 0; i < BITS_NUMBER; i++) {
		digit = (MAX_BIT >> i) & number;
		if (!digit) {
			if (!(current->left)) {
				create_node(&(current->left), 0);
			}
			current = current->left;
		} else {
			if (!(current->right)) {
				create_node(&(current->right), 1);
			}
			current = current->right;
		}
	}
}

short nearest(short number, trie_node *head) {
	int i, digit;
	int result = 0;
	trie_node *current = head;
	if (!head) return -1;
	for (i = 0; i < BITS_NUMBER; i++) {
		int d = MAX_BIT >> i;
		digit = (MAX_BIT >> i) & number;
		if (!digit) {
			if (current->left) {
				current = current->left;
			} else {
				result |= MAX_BIT >> i; 
				current = current->right;
			}
		} else {
			if (current->right) {
				result |= MAX_BIT >> i; 
				current = current->right;
			} else {
				current = current->left;
			}
		}
	}
	return result;
}
*/
