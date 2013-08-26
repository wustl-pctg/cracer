#!/bin/bash

NPROC=16
WORK=0.1 #1.0
OPS=1  #100000
ITER=5

DATE=$(date +%m%d%H%M)

cd prob/testbed && make clean && make param profile=1
if [ $? -ne 0 ]; then
		echo "Compilation failed. Exiting."
		exit
fi
cd ../..

for ((BTEST = 0; BTEST <= 3; BTEST += 1))
do
		FILE="my.sizes${BTEST}.${DATE}.log";
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" >> $FILE;
		prob/testbed/param --nproc $NPROC --btest $BTEST --stats -1 -w $WORK -o $OP -i $ITER >> $FILE 2>> $FILE
		cat $FILE | grep "Batch sizes" | tr -d 'Batch sizes:' | tr -d ' ' | sed s'/,$//' > temp
		mv temp $FILE
done
