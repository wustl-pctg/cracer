#!/usr/bin/env python
# Run BATCHER tests and collect the data

import subprocess
import time

test_sets = ['prob'] # add split later

# Tests we want to run
tests = {}
#     Name of Test   Start Size, End Size, Increment
#tests['counter'] = range(10000, 20000+1, 10000)
tests['btree'] = range(100000,100001,1)
# tests['dynArray'] = range(1,3,1)
# tests['concHash'] = range(1,3,1)
# tests['om'] = range(1,3,1)

iterations = range(1)
nproc = range(2, 17, 2)

for tset in test_sets:
    for test in tests:
        file = open(test+'Time.log','w')
        file.write('OPS,P,BATCH,LOCK\n')
        for size in tests[test]:
            for p in nproc:
                file.write(str(size)+','+str(p)+',')
                for i in iterations:
                    command = tset + '/testbed/' + test + '_test'
                    args = ' --nproc ' + str(p) + ' -o ' + str(size)
                    # start = time.time()
                    process = subprocess.Popen(command+args, shell=True,
                                               stdout=subprocess.PIPE,
                                               stderr=subprocess.STDOUT)
                    output = process.communicate()[0]
                # If I actually use more than 1 iteration, I'll need
                # to average the result here? Or not, if the actual
                # benchmark does it itself.
                output = output.replace('\n',',').rstrip(',')
                file.write(output+'\n')
                # print 'That took ' + str(time.time() - start)
