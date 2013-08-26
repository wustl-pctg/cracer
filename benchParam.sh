#!/bin/zsh

NPROC=16
ITER=10

WORKMIN=0.05
WORKMAX=2.0
WORKINTER=0.15

OPMIN=100000
OPMAX=100000
OPINTER=100000

DATE=$(date +%m%d%H%M)

cd prob/testbed && make clean && make param
if [ $? -ne 0 ]; then
		echo "Compilation failed. Exiting."
		exit
fi
cd ../..

for ((BTEST = 0; BTEST <= 3; BTEST += 1))
do
		FILE="param${BTEST}.${DATE}.log"
		echo "WORKTIME,REALTIME,WORK,OPS,NPROC,ITER,LOCKS,BATCH,RATIO" >> $FILE
		for ((WORK = WORKMIN;WORK <= $WORKMAX;WORK += $WORKINTER))
		do
				for ((OP = OPMIN;OP <= $OPMAX;OP += $OPINTER))
				do
						prob/testbed/param --nproc $NPROC --btest $BTEST -w $WORK -o $OP -i $ITER >> $FILE
				done
		done
done
