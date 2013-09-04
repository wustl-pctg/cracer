#!/usr/bin/env python
# Run BATCHER stack and compare with FC stack.

import subprocess
import time
import string

test='stackBatch_test'
iterations = range(3)
nproc = range(4, 17, 4)
probdir = 'prob'
fcdir = 'flat_combining'
size=2000000
for p in nproc:
  for i in iterations:
    probcmd = probdir + '/testbed/' + test
    probargs = ' --nproc ' + str(p) + ' ' + str(size)
    fccmd =  fcdir + '/test_intel64 '
    fcargs = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 50 50 0.0 ' + str(size) + ' 10 1 0 0'

    process = subprocess.Popen(probcmd+probargs, shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output);
    print "b",str(p)
    print size/float(tp[0]);
    #print size/float(tp[1]);
    output
  

    
    #now run FC
    #print fccmd,fcargs
    process = subprocess.Popen(fccmd + fcargs, shell=True,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE)
    output = process.communicate()[0]
    tp = string.split(output);
    print "fc"
    print tp[0]
    #print tp[1]
