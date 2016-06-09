// Checking some bit stuf
#include	<stdlib.h>
#include	<stdio.h>
#include	<time.h>
#include "trie.h"

void printBitsInt(unsigned int num)
{
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1 << (size * 8-1);
    int i = 0;
	for(; i < size*8; ++i){
		// print last bit and shift left.
		printf("%u ", num&maxPow ? 1 : 0);
		num = num << 1;
	}
	printf("\n");
}


int main ( int argc, char *argv[] ) {


	unsigned int bitone = 0x0;
	unsigned int bittwo = 0x0;
	unsigned int bitfive = 0x0;
	unsigned int bittest = 0x0;
	unsigned int test;

	printf("They should be 0:\n");
	printf("1: %u 2: %u 3: %u\n", bitone, bittwo, bitfive);

	printBitsInt(bitone);
	printBitsInt(0);
	printBitsInt(~bitone & 1);
	if (~bitone & 1)
		printf("o is o true\n");
	else
		printf("not true\n");
	
	bittest |= 1 << 1;
	bittest |= 1 << 3;
	bittest |= 1 << 5;
	printf("Bittest: %u\n", bittest);
	printBitsInt(bittest);
	bittest = bittest + 0x1;
	printf("Bittest +: %u\n", bittest);
	printBitsInt(bittest);
	bittest = (bittest<<1) + 0x1;
	printf("Bittest ++: %u\n", bittest);
	printBitsInt(bittest);
	
	printBitsInt(bittest);
	test ^= (bittest<<1);
	printf("test: %u\n", test);
	printBitsInt(test);
	bitone |= 1 << 0;
	bittwo |= 1 << 2;
	bitfive |= 1 << 5;

	printf("They should be set:\n");
	printf("1: %u 2: %u 3: %u\n", bitone, bittwo, bitfive);
	
	bitone &= ~(1<<0);
	bittwo &= ~(1<<2);
	bitfive &= ~(1<<5);

	printf("They should be clear:\n");
	printf("1: %u 2: %u 3: %u\n", bitone, bittwo, bitfive);

	bitone ^= 1<<0;
	bittwo ^= 1<<2;
	bitfive ^= 1<<5;

	printf("They should be toggled:\n");
	printf("1: %u 2: %u 3: %u\n", bitone, bittwo, bitfive);
	
	printBitsInt(bitone);
	//printBitsInt(bittwo);
	//printBitsInt(bitfive);

/*
	trie_node ** head = malloc(sizeof(trie_node*));
	insert(3,head);
	insert(2,head);
	insert(11,head);
	insert(10,head);
	insert(12,head);
	insert(50,head);
	insert(1,head);
*/	
/*	
	trie_node * trie = initialize_trie();
	trie_node * one = malloc(sizeof(one));
	trie_node * two = malloc(sizeof(two));
	trie_node * three = malloc(sizeof(three));
	insert(trie, one);
	printBitsInt(trie->value);
	printBitsInt(one->value);
	insert(one,two);
	printBitsInt(two->value);
	insert(two,three);
	printBitsInt(three->value);
*/
}
