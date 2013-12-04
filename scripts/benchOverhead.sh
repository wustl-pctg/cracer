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
    echo "Batcher compilation failed. Exiting."
    exit
fi

cd ../../flat_combining && make
if [ $? -ne 0 ]; then
		echo "Flat combining compilation failed. Exiting."
		exit
fi
cd ..

BATCHPATH=prob/testbed
FCPATH=flat_combining
# Should use a better timestamp later.
TIMESTAMP=`date +%j`
FILE="logs/overhead.${TIMESTAMP}.log"
echo "NPROC,FULL,RAW,RACE,FC" > ${FILE}

for ((P = $PMIN; P <= $PMAX; P += $PINTER))
do
    for ((OPS = $OPMIN; OPS <= $OPMAX; OPS += $OPINTER))
    do
        PROG=${BATCHPATH}/stackBatch_test
        echo "Running full test."
        OUT1=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`

        ARGS+=" --raw"
        echo "Running raw test (no invoke_batch_slow, no collect)."
        OUT2=`$PROG --nproc $P --dsprob 0 --batchprob 0 --raw -o $OPS`

        PROG=${BATCHPATH}/stackNoSpawn
        ARGS=" --nproc ${P} --dsprob 0 --batchprob 0 -o ${OPS}"
        echo "Running stackNoSpawn (no invoke_batch, no collect, no lock, just race)."
        OUT3=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`

				PROG=${FCPATH}/test_intel64
				ARGS=" fcstack 1 non 0 non 0 non 0 1 {$P} 50 50 0.0 0 10 0 0 0"
				echo "Running flat combingin - 50/50"
				TEMP=`$PROG fcstack 1 non 0 non 0 non 0 1 $P 50 50 0.0 0 10 0 0 0 2> /dev/null`
				OUT4=`echo $TEMP | cut -d" " -f1`

				# This isn't portable! For floating-point arithmetic, we
				# should really pipe the expression to bc instead.
        echo ${P},$((${OPS}/${OUT1})),$((${OPS}/${OUT2})),$((${OPS}/${OUT3})),${OUT4} >> ${FILE}
    done
done
