#!/bin/bash
gcc -lm -O3 -m64 -I./../ds -I./../util -I./../ ./../om_general_test.c ./../ds/order-maintenance-general.c ./../util/omOptions.c
for ((i=50000; i <= 51200000; i = i * 2)); do printf "%i," $i; ./a.out -i $i -e; done

