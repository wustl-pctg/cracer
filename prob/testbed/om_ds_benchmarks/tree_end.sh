#!/bin/bash
./../../support/cilkclocal -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L -lm -O3 -m64 -I./../ds -I./../util -I./../  ./../om_parallel_test.cilk ./../ds/order-maintenance-par-rebalance.cilk ./../util/omOptions.c
for ((i=50000; i <= 51200000; i = i * 2)); do printf "%i," $i; ./a.out -i $i -e; done

