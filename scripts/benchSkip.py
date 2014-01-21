#!/usr/bin/env python

import subprocess, sys
import time, datetime
import string, shlex
import math
import signal

# Directory setup.
base_dir = "../"
log_dir = base_dir + "logs/"
graph_dir = base_dir + "graphs/"
batch_dir = base_dir + "prob/testbed/"
fc_dir = base_dir + "flat_combining/"

# General parameters.
silent = False
verbose = 2
iterations = range(3)
nproc = range(44, 49, 2)
spots = 10 # Later, iterate over this: range(16,17)
timeout = 10*60 # 10 minutes max. per run.

# Note: initial size of batch is assumed to be 20000.
# Should later set this as a parameter (--special?)
initial_size = 20000

# Batch parameters.
batch_test = "skiplist"
batch_ops = 1000000
ds_prob = 0

# Flat combining parameters.
fc_test = "test_intel64"
fc_time = 20 # seconds to run the flat combining benchmark

dedicated = 0 # Run all adds first, then removes.
percent_adds = 100 # What percentage of total operations should be adds.

class Alarm(Exception):
  pass

def alarm_handler(signum, frame):
  raise Alarm

def run_experiment(cmd, error_msg):

  if verbose >= 2:
    print(' '.join(cmd))

  signal.signal(signal.SIGALRM, alarm_handler)
  signal.alarm(timeout)

  try:
    process = subprocess.Popen(cmd, shell = False,
                               stdout = subprocess.PIPE,
                               stderr = subprocess.PIPE)
    output = process.communicate()
  except Alarm:
    print(' '.join(cmd) + " took too long.")
    sys.exit(1)

  # Flat combining outputs statistics on stderr, so we can't use
  # this. Dumb.
  # if output[1]:
  #   print(error_msg)
  #   print(output[1])
  #   sys.exit(1)

  if verbose >= 2:
    print(output[0])

  return output[0]

def run_flat_combining(p, i):

  # Arguments:
  # alg1_name alg1_num alg2_name alg2_num alg3_name alg3_num
  # alg4_name alg4_num test_no n_threads add_ops remove_ops
  # load_factor capacity runtime is_dedicated_flag tm_status(?)
  # read_write_delay
  fc_args = 'fcskiplist 1 non 0 non 0 non 0'.split()
  fc_args = fc_args + ['1', str(p), str(percent_adds), str(100 - percent_adds)]
  fc_args = fc_args + ['0.0', str(initial_size), str(fc_time)]
  fc_args = fc_args + [str(dedicated), '0', '0']

  fc_cmd =  [fc_dir + fc_test] + fc_args
  error_msg = "Error running flat combining "
  error_msg += " with {0} processors, iteration {1}:".format(p, i)

  result = run_experiment(fc_cmd, error_msg)

  return float(string.split(result)[0])

def run_batch_seq(p, i):
  batch_args = ['--nproc', str(p), '--dsprob', str(ds_prob)]
  batch_args = batch_args + ['--batchprob', '0']
  batch_args = batch_args + ['-o', str(batch_ops)]
  batch_args = batch_args + ['--seq']

  batch_cmd = [batch_dir + batch_test] + batch_args
  error_msg = "Error running sequential batch "
  error_msg += " with {0} processors, iteration {1}:".format(p, i)

  result = run_experiment(batch_cmd, error_msg)

  return float(result)


def run_batch_par(p, i, num_batch_ops, num_spots=1, batchprob=100):
  batch_args = ['--nproc', str(p)]
  batch_args = batch_args + ['--dsprob', str(ds_prob)]
  batch_args = batch_args + ['--batchprob', str(batchprob)]
  batch_args = batch_args + ['--batchvals', str(num_spots)]
  batch_args = batch_args + ['-o', str(num_batch_ops)]

  batch_cmd = [batch_dir + batch_test] + batch_args
  error_msg = "Error running parallel batch "
  error_msg += " with {0} processors, iteration {1}:".format(p, i)

  result = run_experiment(batch_cmd, error_msg)

  return float(result)


def write_throughput_log(p, throughputs, out_file):
  output = str(p) + ','
  output += ",".join(str(item) for item in throughputs)

  if verbose >= 1:
    print output

  out_file.write(output + '\n')

def main():
  global verbose
  if silent: verbose = 0

  # Log output setup.
  current = datetime.datetime.now();
  filename = "skiplist{0}-{1}-{2}-{3}.log".format(current.month,current.day,
                                                  current.hour,current.minute)
  out_file = open(log_dir + filename, "w");
  out_file.write("P,FC,BatchSeq,BatchPar, BatchMulti\n");

  for p in nproc:

    if verbose >= 1:
      print "--- Running tests on {0} cores. ---".format(p)

    throughputs = 4 * [0.0]

    for i in iterations:

      # throughputs[0] = throughputs[0] + run_flat_combining(p, i)

      # Sequential Single Batches
      throughputs[1] = throughputs[1] \
                       + float(batch_ops) * spots \
                       / run_batch_par(p, i, batch_ops * spots, 1, 0)
#                       / run_batch_seq(p,i)

      # Parallel Single Batches
      throughputs[2] = throughputs[2] \
                       + float(batch_ops) * spots \
                       / run_batch_par(p, i, batch_ops * spots, 1, 100)

      # Parallel Multi-Batches
      throughputs[3] = throughputs[3] \
                       + float(batch_ops) * spots \
                       / run_batch_par(p, i, batch_ops, spots, 100)

    throughputs = [ float(total) / len(iterations) for total in throughputs ]
    write_throughput_log(p, throughputs, out_file)

    #graph the output
    '''
    plotscript="plot.gnu"
    arguments="\"filename='{0}';cols={1};outfile='{2}'".format(logdir+filename,len(throughput),graphdir+filename);
    graphcmd = "gnuplots -e {0} {1}".format(arguments,plotscript);
    print graphcmd
    print arguments
    print plotscript
    '''

if __name__ == "__main__":
  main()
