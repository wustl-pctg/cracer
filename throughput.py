#!/usr/bin/env python
# Run BATCHER stack and compare with FC stack.

import subprocess
import time
import string
import math

test='stackBatch_test'
iterations = range(1)
#nproc = range(1, 17, 1)
nproc = range(16,17,1)
probdir = 'prob'
fcdir = 'flat_combining'
fctime = 1000 # seconds to run the flat combining benchmark
batchops=2000000
fcinitialsize=20000
outFile = open("stack.log","w");
outFile.write("PROCS,BATCH,FC\n");
for p in nproc:
  for i in iterations:
    probcmd = probdir + '/testbed/' + test
    probargs = ' --nproc ' + str(p) + ' ' + str(batchops)
    fccmd =  fcdir + '/test_intel64 '

    # Arguments:
    # alg1_name alg1_num alg2_name alg2_num alg3_name alg3_num
    # alg4_name alg4_num test_no n_threads add_ops remove_ops
    # load_factor capacity runtime is_dedicated_flag tm_status(?)
    # read_write_delay
    fcargs = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 50 50 0.0 '
    fcargs += str(fcinitialsize) + ' ' + str(fctime) + ' 0 0 0'

    process = subprocess.Popen(probcmd+probargs, shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    batchThroughput= batchops/float(tp[0])

    #now run FC
    #print fccmd,fcargs
    process = subprocess.Popen(fccmd + fcargs, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    tp = string.split(output);
    fcThroughput = float(tp[0])/float(fctime * 1000)
    outFile.write("{0},{1},{2}\n".format(str(p),math.floor(batchThroughput),
                                         math.floor(fcThroughput)));
