#include <stdlib.h>
#include <stdio.h>

typedef struct trie_node_s {
	unsigned int value;
	unsigned short level;
	struct trie_node_s * root;
	struct trie_node_s * parent;
	struct trie_node_s * left;
	struct trie_node_s * right;

} trie_node;

trie_node * initialize_trie();
void insert(trie_node * x, trie_node * y);
void printBits(unsigned int value);




/*
typedef struct trie_node {
	int value;
	struct trie_node *left;
	struct trie_node *right;
} trie_node;

void insert(short number, trie_node **head);
short nearest(short number, trie_node *head);
*/
