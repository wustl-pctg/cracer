#!/bin/zsh

ITER=5

OPMIN=100000000
OPMAX=100000000
OPINTER=100000000

PMIN=1
PMAX=16
PINTER=1

cd ../prob/testbed && make cilk-clean && make clean && make cilk && make stack
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
echo "NPROC,FC,TERM" > ${FILE}

for ((P = $PMIN; P <= $PMAX; P += $PINTER))
do
    for ((OPS = $OPMIN; OPS <= $OPMAX; OPS += $OPINTER))
    do
        OUT1=0
        OUT2=0
        OUT3=0
        OUT4=0
        for ((I = 0; I < $ITER; I += 1))
        do
				    # PROG=${FCPATH}/test_intel64
				    # ARGS=" fcstack 1 non 0 non 0 non 0 1 {$P} 50 50 0.0 0 10 0 0 0"
				    # echo "Running flat combining - 50/50"
				    # TEMP=`$PROG fcstack 1 non 0 non 0 non 0 1 $P 50 50 0.0 0 10 0 0 0 2> /dev/null`
				    # OUT4=$((${OUT4}+`echo $TEMP | cut -d" " -f1`))

            PROG=${BATCHPATH}/stackBatch_test
            #        echo "Running full test."
            #        OUT1=`$PROG --nproc $P --dsprob 0 --batchprob 0 -o $OPS`

            ARGS+=" --raw"
            echo "Running raw test (no invoke_batch_slow, no collect, term)."
            OUT2=$((${OUT2}+`$PROG --nproc $P --dsprob 0 --batchprob 0 --raw -o $OPS`))

        done
    done

    # Should actually pipe to bc instead...
    OUT2=$((${OUT2}/${ITER}))
#    OUT4=$((${OUT4}/${ITER}))
    echo ${P},${OUT4},$((${OPS}/${OUT2})) >> ${FILE}
done
