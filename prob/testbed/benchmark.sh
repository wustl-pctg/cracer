#!/bin/bash
gcc -lm -O3 -m64 -Ids om_general_test.c ds/order-maintenance-general.c
for ((i=10000; i <= 128000000; i = i * 2)); do printf "%i," $i; ./a.out $i; done
