#!/bin/bash

ITER=5

OPMIN=1000000
OPMAX=1000000
OPINTER=1000000

PMIN=1
PMAX=16
PINTER=1

cd ../prob/testbed && make clean && make cilk && make stack
# && make noop
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit
fi

cd ../..
BENCHPATH=prob/testbed
TIMESTAMP=`date +%j`
FILE="logs/overhead.${TIMESTAMP}.log"
echo "NPROC,FULL,RAW,NOOP" > ${FILE}

for ((P = $PMIN; P <= $PMAX; P += $PINTER))
do
    for ((OPS = $OPMIN; OPS <= $OPMAX; OPS += $OPINTER))
    do
        PROG="${BENCHPATH}/stackBatch_test"
        ARGS=" --nproc ${P} --dsprob 0 --batchprob 0 -o ${OPS}"
        COMMAND=${PROG}${ARGS}
        echo ${COMMAND}
        OUT1=`${COMMAND}`

        ARGS+=" --raw"
        COMMAND=${PROG}${ARGS}
        echo ${COMMAND}
        OUT2=`${COMMAND}`

        PROG="${BENCHPATH}/prob/testbed/noop"
        ARGS=" --nproc ${P} --dsprob 0 --batchprob 0 -o ${OPS}"
        COMMAND=$PROG$ARGS
        echo $COMMAND
        OUT3=`${COMMAND}`
        echo ${P},$((${OPS}/${OUT1})),$((${OPS}/${OUT2})),$((${OPS}/${OUT3})) >> ${FILE}
    done
done
