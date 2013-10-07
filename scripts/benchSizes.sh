#!/bin/bash

NPROC=16
WORK=1.0
OPS=100000
#ITER=5

DATE=$(date +%m%d%H%M)

#cd prob/testbed && make clean && make param profile=1
cd prob/testbed && make cilk && make btree mode=debug profile=1
if [ $? -ne 0 ]; then
		echo "Compilation failed. Exiting."
		exit
fi
cd ../..

HEADER=
for ((i = 2; i <= NPROC; i += 2))
do
		HEADER="${HEADER}$i,"
done
HEADER=$(echo $HEADER | sed s'/,$//')

for ((P = 2; P <= NPROC; P += 2))
do
# for ((BTEST = 0; BTEST <= 3; BTEST += 1))
# do
#FILE="sizes${BTEST}.${DATE}.log";
		FILE="btreeSizes.${DATE}.log";
#prob/testbed/param --nproc $NPROC --btest $BTEST --stats -1 -w $WORK
# -o $OPS -i $ITER 2>> $FILE
		prob/testbed/btree_test --nproc $NPROC --stats -1 -o $OPS 2>> $FILE

		echo $HEADER > temp
		cat $FILE | grep "Batch sizes" | tr -d 'Batch sizes:' | tr -d ' ' | sed s'/,$//' >> temp
		mv temp $FILE
done
# done
