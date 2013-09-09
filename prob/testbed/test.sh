#!/bin/sh

WORK=1.0
OPS=100000
ITER=5

for P in 8 16
do
		FILE=$P.log
		echo ./param --nproc $P --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER --sequential
		./param --nproc $P --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER --sequential
		echo ./param --nproc $P --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER
		./param --nproc $P --dsprob 0 --batchprob 0 -w $WORK -o $OPS -i $ITER
		echo ./param --nproc $P --dsprob 0 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
		./param --nproc $P --dsprob 0 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
		echo ./param --nproc $P --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER
		./param --nproc $P --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER
		echo ./param --nproc $P --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
		./param --nproc $P --dsprob 50 --batchprob 100 -w $WORK -o $OPS -i $ITER --sequential
done
