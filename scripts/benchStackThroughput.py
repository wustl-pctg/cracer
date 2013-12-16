#!/usr/bin/env python
# Run BATCHER stack and compare with FC stack.

import subprocess
import time
import string
import math
import datetime

test='stackBatch_test'
iterations = range(1)
nproc = range(1, 2, 1)
#nproc = range(16,17,1)
probdir = '../prob'
fcdir = '../flat_combining'
fctime = 10 # seconds to run the flat combining benchmark
batchops=10000000
fcinitialsize=0
dedicated=0
logdir = '../logs/'
graphdir = '../graphs/'

current = datetime.datetime.now();
filename = "stackBench{0}-{1}-{2}-{3}.log".format(current.month,current.day,current.hour,current.minute)
outFile = open(logdir+filename,"w");
outFile.write("PROCS,B50_50,B50_50_NS,B80_20,B80_20NS,FC_50_50,FC_80_20,FC_90_10\n");
for p in nproc:
  for i in iterations:
    throughput=[str(p)]
    batchcmd = probdir + '/testbed/' + test
    batch_args = ' --nproc ' + str(p) + ' -o ' + str(batchops) 
    batch_percent=50;
    batch_no_steal_args = ' --nproc ' + str(p) +' --batchprob 0 --dsprob 0 ' + ' -o ' + str(batchops) 

    # Arguments:
    # alg1_name alg1_num alg2_name alg2_num alg3_name alg3_num
    # alg4_name alg4_num test_no n_threads add_ops remove_ops
    # load_factor capacity runtime is_dedicated_flag tm_status(?)
    # read_write_delay
    fccmd =  fcdir + '/test_intel64 '
    fc_50_50_args = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 50 50 0.0 '
    fc_50_50_args += str(fcinitialsize) + ' ' + str(fctime) + ' ' + str(dedicated) + ' 0 0'
    fc_80_20_args = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 80 20 0.0 '
    fc_80_20_args += str(fcinitialsize) + ' ' + str(fctime) + ' ' + str(dedicated) + ' 0 0'
    fc_90_10_args = 'fcstack 1 non 0 non 0 non 0 1 '+ str(p) + ' 90 10 0.0 '
    fc_90_10_args += str(fcinitialsize) + ' ' + str(fctime) + ' ' + str(dedicated) + ' 0 0'


    #First run batch
    batch_percent=50;
    process = subprocess.Popen(batchcmd+batch_args+' -x ' + str(batch_percent), shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput.append(math.floor(batchops/float(tp[0])))

    process = subprocess.Popen(batchcmd+batch_no_steal_args + ' -x ' + str(batch_percent), shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput.append(math.floor(batchops/float(tp[0])))
    #80_20 for batch
    batch_percent=80;
    process = subprocess.Popen(batchcmd+batch_args+' -x ' + str(batch_percent), shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput.append(math.floor(batchops/float(tp[0])))

    process = subprocess.Popen(batchcmd+batch_no_steal_args + ' -x ' + str(batch_percent), shell=True,
      stdout=subprocess.PIPE,
      stderr=subprocess.STDOUT)
    output = process.communicate()[0]
    tp = string.split(output)
    throughput.append(math.floor(batchops/float(tp[0])))
   
    #now run FC tests
    #50_50
    process = subprocess.Popen(fccmd + fc_50_50_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    throughput.append((string.split(output))[0])
    #80_20
    process = subprocess.Popen(fccmd + fc_80_20_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    throughput.append((string.split(output))[0])
    #90_10
    process = subprocess.Popen(fccmd + fc_90_10_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
    throughput.append((string.split(output))[0])
    

    
    output = ",".join(str(item) for item in throughput)
    outFile.write(output);
    outFile.write("\n");
'''    
    #graph the output
    plotscript="plot.gnu"
    arguments="\"filename='{0}';cols={1};outfile='{2}'".format(logdir+filename,len(throughput),graphdir+filename);
    graphcmd = "gnuplots -e {0} {1}".format(arguments,plotscript);
    print graphcmd
    print arguments
    print plotscript
    process = subprocess.Popen(fccmd + fc_90_10_args, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output = process.communicate()[0]
'''
