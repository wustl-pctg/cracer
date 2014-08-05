#!/bin/bash
gcc -lm -O3 -m64 -Ids om_parallel_test.cilk ds/order-maintenance-par-rebalance.cilk
for ((i=50000; i <= 25600000; i = i * 2)); do printf "%i," $i; ./a.out $i; done
