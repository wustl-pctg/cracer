#!/bin/bash
echo "run tests"

WORKMAX=100000
OPMAX=100000
NPROCMAX=9
ITER=5
FILE=workTestLog.txt
OPINTER=10000
WORKINTER=10000
echo "WORK,OPS,ITER,LOCKS,BATCH" > $FILE
NPROC=$NPROCMAX
    echo "NPROC=$NPROC" >>$FILE
    for ((WORK=0;WORK<=$WORKMAX;WORK+=$WORKINTER))
    do
	for ((OP=1;OP<=$OPMAX;OP+=$OPINTER))
	do
	    ./work --nproc $NPROC -w $WORK -o $OP -i $ITER -b -a * 2>> $FILE
	done
    done
