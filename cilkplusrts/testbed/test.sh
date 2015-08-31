#!/bin/bash
set -e # exit immediately if a 'simple' command fails

# Recompile cilk unless we pass in a 1
if [[ ( "$#" < 1 ) || ( "$1" == 0 ) ]] ; then
    cd ..
#    make clean
    make
    make install
    cd testbed
fi

g++ -g -O2 -D_DEBUG skiplist_test.cpp skiplist.cpp -fcilkplus -lcilkrts -o test

set +e
for ((ITER = 1; ITER <= 100; ITER++)); do 
    CILK_NWORKERS=16 timeout 20 ./test;
    if [[ $? -ne 0 ]]; then
        echo "Infinite loop. Test failed."
        exit 1
    fi
done;
