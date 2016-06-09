#ifndef _OM_OPTIONS_H
#define _OM_OPTIONS_H

typedef struct
{
	int inserts;
	int rand;
	int beg;
	int end;
	int printSplits;
	
} OmOptions;

int getOptions(int argc, char ** argv, OmOptions * opt);
void usage();

#endif	
