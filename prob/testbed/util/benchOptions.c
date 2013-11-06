#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "benchOptions.h"

void usage()
{
	printf("Usage:<benchmark> [OPTIONS]\n");
	printf("Run the given data structure benchmark.\n\n");
	printf("Mandatory arguments to long options are mandatory for short options too.\n");
	printf("\t-v, --verbose\t\t\tPrint extra runtime information.\n");
	printf("\t-b, --noBatch\t\t\tDon't run the batch version of the benchmark.\n");
	printf("\t-l, --noLock\t\t\tDon't run the lock version of the benchmark.\n");
	printf("\t-o, --operations\t\tHow many operations to perform.\n");
	printf("\t-i, --iterations\t\tHow many iterations of the benchmark to perform.\n");
	printf("\t-x, --special paramemter\t\tAny special integer paramter for the benchmark. I.e. Operation ratio for Stack.\n");
	printf("\t-r, --raw\t\t Specify to run using a batchify_raw functin.\n");
}

int getOptions(int argc, char **argv, BenchOptions *opt)
{
	struct option longOptions[] =
		{
			{"verbose", no_argument, 0, 'v'},
			{"noBatch", no_argument, 0, 'b'},
			{"noLock", no_argument, 0, 'l'},
			{"dedicated", no_argument, 0, 'd'},
			{"operations", required_argument, 0, 'o'},
			{"iterations", required_argument, 0, 'i'},
			{"special", required_argument, 0, 'x'},
			{"raw", no_argument, 0, 'r'},
		};

	int optIndex = 0;
	int option;

	while((option = getopt_long(argc, argv, "o:i:vbldx:r", longOptions, &optIndex))
				!= -1)
		{
			switch(option)
				{
				case 0:
					if (longOptions[optIndex].flag != 0) break;
					printf("Invalid options.\n");
					usage();
					exit(1);
					break;
				case 'v':
					opt->verbose = 1;
					break;
				case 'b':
					opt->noBatch = 1;
					break;
				case 'l':
					opt->noLock = 1;
					break;
				case 'o':
					opt->operations = atoi(optarg);
					break;
				case 'i':
					opt->iterations = atoi(optarg);
					break;
				case 'd':
					opt->dedicated = 1;
					break;
				case 'x':
					opt->special = atoi(optarg);
					break;
				case 'r':
				  opt->raw=1;
				  break;
				default:
					usage();
					exit(1);
				}
		}
}
