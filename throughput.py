#!/usr/bin/env python
# Run BATCHER stack and compare with FC stack.

import subprocess
import time
import string
import math

test='stackBatch_test'
iterations = range(1)
nproc = range(4, 17, 4)
probdir = 'prob'
fcdir = 'flat_combining'
size=2000000
outFile = open("stackLog.txt","w");
outFile.write("PROCS,BATCH,FC")
for p in nproc:
  for i in iterations:
    probcmd = probdir + '/testbed/' + test
    probargs = ' --nproc ' + str(p) + ' ' + str(size)
    fccmd =  fcdir + '/test_intel64 '
    fcargs = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 50 50 0.0 ' + str(size) + ' 5 0 0 0'

    process = subprocess.Popen(probcmd+probargs, shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    batchThroughput= size/float(tp[0])

    #now run FC
    #print fccmd,fcargs
    process = subprocess.Popen(fccmd + fcargs, shell=True,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE)
    output = process.communicate()[0]
    tp = string.split(output);
    fcThroughput = tp[0]
    outFile.write("{0},{1},{2}".format(str(p),math.floor(batchThroughput),fcThroughput));
