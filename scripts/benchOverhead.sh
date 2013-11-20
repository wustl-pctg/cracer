#!/bin/zsh

ITER=5

OPMIN=9000000
OPMAX=9000000
OPINTER=9000000

PMIN=1
PMAX=16
PINTER=1

cd ../prob/testbed && make clean && make cilk && make stack && make stackNoSpawn
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit
fi

cd ../..
BENCHPATH=prob/testbed
# Should use a better timestamp later.
TIMESTAMP=`date +%j`
FILE="logs/overhead.${TIMESTAMP}.log"
echo "NPROC,FULL,RAW,NOSPAWNS" > ${FILE}

for ((P = $PMIN; P <= $PMAX; P += $PINTER))
do
    for ((OPS = $OPMIN; OPS <= $OPMAX; OPS += $OPINTER))
    do
        PROG=${BENCHPATH}/stackBatch_test
        echo "Running full test."
        OUT1=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`

        ARGS+=" --raw"
        echo "Running raw test (no invoke_batch_slow)."
        OUT2=`$PROG --nproc $P --dsprob 0 --batchprob 0 --raw -o $OPS`

        PROG=${BENCHPATH}/stackNoSpawn
        ARGS=" --nproc ${P} --dsprob 0 --batchprob 0 -o ${OPS}"
        echo "Running stackNoSpawn (few spawns, no invoke_batch)."
        OUT3=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`

				# This isn't portable! For floating-point arithmetic, we
				# should really pipe the expression to bc instead.
        echo ${P},$((${OPS}/${OUT1})),$((${OPS}/${OUT2})),$((${OPS}/${OUT3})) >> ${FILE}
    done
done
