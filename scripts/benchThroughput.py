#!/usr/bin/env python
# Run BATCHER stack and compare with FC stack.

import subprocess
import time
import string
import math
import datetime

test='stackBatch_test'
iterations = range(1)
#nproc = range(1, 17, 1)
nproc = range(16,17,1)
probdir = 'prob'
fcdir = 'flat_combining'
fctime = 10 # seconds to run the flat combining benchmark
batchops=2000000
fcinitialsize=20000
dedicated=0

current = datetime.datetime.now();
filename = "stackBench{0}-{1}-{2}-{3}.log".format(current.month,current.day,current.hour,current.minute)
outFile = open(filename,"w");
outFile.write("PROCS,BATCH,BATCH_NO_STEAL,FC_50_50,FC_80_20\n");
for p in nproc:
  for i in iterations:
    throughput=[0,0,0,0]
    batchcmd = probdir + '/testbed/' + test
    batch_args = ' --nproc ' + str(p) + ' -o ' + str(batchops) 
    batch_no_steal_args = ' --nproc ' + str(p) +' --batchprob 0 --dsprob 0 ' + ' -o ' + str(batchops) 
    
    # Arguments:
    # alg1_name alg1_num alg2_name alg2_num alg3_name alg3_num
    # alg4_name alg4_num test_no n_threads add_ops remove_ops
    # load_factor capacity runtime is_dedicated_flag tm_status(?)
    # read_write_delay
    fccmd =  fcdir + '/test_intel64 '
    fc_50_50_args = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 50 50 0.0 '
    fc_80_20_args = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 80 20 0.0 '
    fc_50_50_args += str(fcinitialsize) + ' ' + str(fctime) + ' ' + str(dedicated) + ' 0 0'
    fc_80_20_args += str(fcinitialsize) + ' ' + str(fctime) + ' ' + str(dedicated) + ' 0 0'
    
    
    #First run batch
    process = subprocess.Popen(batchcmd+batch_args, shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput[0]=batchops/float(tp[0])

    process = subprocess.Popen(batchcmd+batch_no_steal_args, shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput[1] = batchops/float(tp[0])

    #now run FC
    #print fccmd,fcargs
    process = subprocess.Popen(fccmd + fc_50_50_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    throughput[2]=(string.split(output))[0]
 
    process = subprocess.Popen(fccmd + fc_80_20_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    throughput[3]=(string.split(output))[0]

    #then print output
    #print throughput
    outFile.write("{0},{1},{2},{3},{4}\n".format(str(p),math.floor(throughput[0]),math.floor(throughput[1]),
                                                 throughput[2],throughput[3]))
