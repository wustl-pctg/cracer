#!/bin/bash
gcc -lm -O3 -m64 -Ids om_top_list_test.c ds/order-maintenance-par-rebalance.c
for ((i=50000; i <= 128000000; i = i * 2)); do printf "%i," $i; ./a.out $i; done
