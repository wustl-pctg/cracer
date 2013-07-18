#!/usr/bin/env python
# Run BATCHER tests and collect the data

import subprocess
import time

test_sets = ['prob'] # add split later

# Tests we want to run
tests = {}
#     Name of Test   Start Size, End Size, Increment
tests['counter'] = range(10000, 20000+1, 10000)
# tests['param'] = range(1,3,1)
# tests['btree'] = range(1,3,1)
# tests['dynArray'] = range(1,3,1)
# tests['concHash'] = range(1,3,1)
# tests['om'] = range(1,3,1)

iterations = range(1)
nproc = range(4, 8+1, 4)

for tset in test_sets:
    for test in tests:
        for size in tests[test]:
            for p in nproc:
                for i in iterations:
                    command = tset + '/testbed/' + test + ' '
                    args = '-nproc ' + str(p) + ' ' + str(size)
                    print "Run " + command + args
                    start = time.time()
                    process = subprocess.Popen(command+args, shell=True,
                                               stdout=subprocess.PIPE,
                                               stderr=subprocess.STDOUT)
                    output = process.communicate()[0]
                    print output
                    print 'That took ' + str(time.time() - start)
