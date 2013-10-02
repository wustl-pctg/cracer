#!/bin/bash


#./test_intel64 fcskiplist 1 non 0 non 0 non 0 1 4 100 0 0.0 200 10 0 0 0
./test_cilk --nproc 4 --dsprob 50 --batchprob 50 dsskiplist 1 non 0 non 0 non 0 1 4 100 0 0.0 200 10 0 0 0
