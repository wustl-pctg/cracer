#!/usr/bin/env python
# Run BATCHER tests and collect the data

import subprocess
import time

test_sets = ['prob'] # add split later

# Tests we want to run
tests = {}
#     Name of Test   Start Size, End Size, Increment
#tests['counter'] = range(10000, 20000+1, 10000)
tests['btree'] = range(1,3,1)
# tests['dynArray'] = range(1,3,1)
# tests['concHash'] = range(1,3,1)
# tests['om'] = range(1,3,1)

iterations = range(1)
nproc = range(8, 8+1, 4)

for tset in test_sets:
    for test in tests:
        for size in tests[test]:
            for p in nproc:
                for i in iterations:
                    command = 'sudo ' + tset + '/testbed/' + test
                    lock = command + 'Lock_test'
                    batch = command + 'Batch_test'
                    args = ' --nproc ' + str(p) + ' ' + str(size)
                    print lock,args,': ',
                    # start = time.time()
                    process = subprocess.Popen(lock+args, shell=True,
                                               stdout=subprocess.PIPE,
                                               stderr=subprocess.STDOUT)
                    output = process.communicate()[0]
                    print output
                    # print 'That took ' + str(time.time() - start)

                    print batch,args,': ',
                    process = subprocess.Popen(batch+args, shell=True,
                                               stdout=subprocess.PIPE,
                                               stderr=subprocess.STDOUT)
                    output = process.communicate()[0]
                    print output
