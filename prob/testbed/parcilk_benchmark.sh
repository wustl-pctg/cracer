#!/bin/bash
/home/alexjones/DS/prob/support/cilkclocal -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L -lm -O3 -m64 -Ids om_parallel_test.cilk ds/order-maintenance-par-rebalance.cilk
for ((i=50000; i <= 25600000; i = i * 2)); do printf "%i," $i; ./a.out $i; done
