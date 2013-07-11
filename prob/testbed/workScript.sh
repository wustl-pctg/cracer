#!/bin/bash
echo "run tests"

WORKMAX=100000
OPMAX=100000
NPROCMAX=10
ITER=5
FILE=workTestLog.txt
OPINTER=5000
WORKINTER=5000
echo "WORK,OPS,ITER,LOCKS,BATCH" > $FILE
NPROC=$NPROCMAX
    echo "NPROC=$NPROC" >>$FILE
    for ((WORK=0;WORK<=$WORKMAX;WORK+=$WORKINTER))
    do
	for ((OP=1;OP<=$OPMAX;OP+=$OPINTER))
	do
	    ./work --nproc $NPROC -w $WORK -o $OP -i $ITER -b -a >> $FILE
	done
    done