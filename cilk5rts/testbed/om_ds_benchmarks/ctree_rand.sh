#!/bin/bash
gcc -O3 -m64 -I./../ds -I./../ -I./../util -lm ./../om_parallel_test.c ./../ds/order-maintenance-par-rebalance.c ./../util/omOptions.c -lm
for ((i=50000; i <= 51200000; i = i * 2)); do printf "%i," $i; ./a.out -i $i -r; done

