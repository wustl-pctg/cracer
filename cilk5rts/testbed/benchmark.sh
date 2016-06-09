#!/bin/bash
gcc -lm -O3 -m64 -Ids om_general_test.c ds/order-maintenance-general.c
for ((i=1000000; i <= 64000000; i = i + 1000000)); do printf "%i," $i; ./a.out $i; done
