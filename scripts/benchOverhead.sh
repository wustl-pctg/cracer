#!/bin/zsh

ITER=5

OPMIN=100000000
OPMAX=100000000
OPINTER=100000000

PMIN=1
PMAX=16
PINTER=1

cd ../prob/testbed && make clean && make cilk && make stack
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
echo "NPROC,RAW,COMPACT,FC" > ${FILE}

for ((P = $PMIN; P <= $PMAX; P += $PINTER))
do
    for ((OPS = $OPMIN; OPS <= $OPMAX; OPS += $OPINTER))
    do
        RAWTIME=0
        COMPTIME=0
        FCTHROUGHPUT=0
        for ((I = 0; I < $ITER; I += 1))
        do
            echo "Raw test (sequential, $P cores, $OPS operations)"
            PROG=${BATCHPATH}/stackBatch_test
            TEMP=`$PROG --nproc $P --dsprob 0 --batchprob 0 --raw -o $OPS`
            RAWTIME=$((${RAWTIME}+${TEMP}))

            echo "Compaction test (sequential, $P cores, $OPS operations)"
            PROG=${BATCHPATH}/stackBatch_test
            TEMP=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`
            while [ $? -ne 0 ]; do
                echo "Failed: ${TEMP}"
                TEMP=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`
            done
            COMPTIME=$((${COMPTIME}+${TEMP}))

				    echo "Running flat combining (50/50, $P cores, $OPS operations)"
				    PROG=${FCPATH}/test_intel64
				    TEMP=`$PROG fcstack 1 non 0 non 0 non 0 1 $P 50 50 0.0 0 10 0 0 0 2> /dev/null`
				    FCTHROUGHPUT=$((${FCTHROUGHPUT}+`echo $TEMP | cut -d" " -f1`))

        done
    done

    # Should actually pipe to bc instead...
    RAWTIME=$((${RAWTIME}/${ITER}))
    COMPTIME=$((${COMPTIME}/${ITER}))
    FCTHROUGHPUT=$((${FCTHROUGHPUT}/${ITER}))

    RAWTHROUGHPUT=$((${OPMAX}/${RAWTIME}))
    COMPTHROUGHPUT=$((${OPMAX}/${COMPTIME}))
    echo ${P},${RAWTHROUGHPUT},${COMPTHROUGHPUT},${FCTHROUGHPUT} >> ${FILE}
done
