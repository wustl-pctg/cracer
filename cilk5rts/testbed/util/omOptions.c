#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "omOptions.h"

/// Flag for insertion type
static int insertion_flag;

void usage()
{

	printf("Usage: <ds test> [OPTIONS]\n");
	printf("Run the given ds test benchmark.\n\n");
	printf("Mandatory to include iterations\n");
	printf("By default does random inserts\n\n");
	printf("\t-i, --iterations\t\t\tNumber of inserts to test.\n");
	printf("\t-r, --random\t\t\tThis flag specifies random insert order.\n");
	printf("\t-b, --beginning\t\t\tThis flag specifies inserts at the beginning.\n");
	printf("\t-e, --end\t\t\tThis flag specifies inserts at the end.\n");
	printf("\t-p, --print-splits", no_argument, 0, 'p');
}

int getOptions(int argc, char **argv, OmOptions* opt)
{

	struct option longOptions[] =
	{ 
		{"inserts", required_argument, 0, 'i'},
		{"random", no_argument, 0, 'r'},
		{"beginning", no_argument, 0, 'b'},
		{"end", no_argument, 0, 'e'},
		{"print-splits", no_argument, 0, 'p'},
	};

	int optIndex = 0;
	int option;
	
	while((option = getopt_long(argc, argv, "i:rbep", longOptions, &optIndex))
		  != 1)
	{

		switch(option)
		{

		case 0:
			if (longOptions[optIndex].flag != 0) break;
			printf("Invalid options.\n");
			usage();
			return 1;
			break;
		case 'i':
			opt->inserts = atoi(optarg);
			break;
		case 'r':
			opt->rand = 1;
			opt->beg = 0;
			opt->end = 0;
			break;
		case 'b':
			opt->beg = 1;
			opt->rand = 0;
			opt->end = 0;
			break;
		case 'e':
			opt->end = 1;
			opt->rand = 0;
			opt->beg = 0;
			break;
		case 'p':
			opt->printSplits = 1;
			break;
		default:
			return 1;
			break;
		}
	}
	return 0;
}
				
			

