#!/usr/bin/env python
from __future__ import print_function
import subprocess, sys
import time, datetime, socket
import string, shlex
import math
from operator import add
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
nproc = range(2, 16, 2)
nproc.insert(0, 1)
timeout = 10*60 # 10 minutes max. per run.

# Note: initial size of batch is assumed to be 20000.
# Should later set this as a parameter (--special?)
initial_size = 20000

# Batch parameters.
batch_test = "skiplist"
total_batch_ops = 50000
ds_prob = 0
batch_prob = 100
biases = [0, 50]
batch_vals = [10, 1000]
sleep_times = [100]

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

  if verbose >= 1:
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
  print(output)

  if process.returncode != 0 or output[0] == None:
    print(error_msg)
    print(output)
    sys.exit(1)

  return output

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

def parse_batch_stats(stats_str):
  stats = dict()
  lines = stats_str.split('\n')

  for l in lines:
    if l == '': continue
    colon_index = string.find(l, ': ')
    name = l[0:colon_index]
    name = name.replace(' ', '_')
    name = string.lower(name)
    value = l[colon_index + 1:]

    if ',' not in value:
      value = int(value)
    else:
      value = value.split(',')
      if value[-1] == '': value = value[0:-1]
      value = [int(v) for v in value]

    stats[name] = value

  batch_sizes = list()
  counts = stats['size_counts']
  print("Counts: " + str(counts))
  for size in counts:
    batch_sizes = batch_sizes + [size] * (counts.index(size) + 1)

  avg_batch_size = float(sum(batch_sizes)) / sum(counts)

  stats['avg_batch_size'] = avg_batch_size

  return stats


def run_batch_par(p, i, total_batch_ops, num_spots=1, sleep_time=0, bias = 0):
  batch_args = ['--nproc', str(p)]
  batch_args = batch_args + ['--dsprob', str(ds_prob)]
  batch_args = batch_args + ['--batchprob', str(batch_prob)]
  batch_args = batch_args + ['--batchvals', str(num_spots)]
  batch_args = batch_args + ['--sleep', str(sleep_time)]
  batch_args = batch_args + ['--bias', str(bias)]
  batch_args = batch_args + ['--stats', str(-1)]
  batch_args = batch_args + ['-o', str(total_batch_ops / num_spots)]

  batch_cmd = [batch_dir + batch_test] + batch_args
  error_msg = "Error running parallel batch "
  error_msg += " with {0} processors, iteration {1}:".format(p, i)

  result = run_experiment(batch_cmd, error_msg)

  if verbose >= 1:
    print(result[0])
    if verbose >= 2:
      print(result[1])

  runtime = float(result[0])
  stats = parse_batch_stats(result[1])

  throughput = float(total_batch_ops) / runtime
  steals = [stats['successful_batch_steals'],
            stats['real_batch_steals'],
            stats['failed_batch_steals']]

  wanted = [throughput, stats['avg_batch_size'], steals]

  if verbose >= 1:
    print(wanted)

  return wanted


def write_throughput_log(p, throughputs, out_file):
  output = str(p) + ','
  output += ",".join(str(item) for item in throughputs)

  if verbose >= 1:
    print(output)

  print("Run: " + output)
#  out_file.write(output + '\n')

def write_header(files):
  for filename in files.keys():
    files[filename].write("P,")
    for n in batch_vals:
      for s in sleep_times:
        for b in biases:
          files[filename].write("n{0}s{1}b{2},".format(n,s,b))
    files[filename].write('\n')

def main():
  global verbose
  if silent: verbose = 0

  # Log output setup.
  files = {}
  hostname = socket.gethostname()
  current = datetime.datetime.now();
  ext = ".{0}.{1}.{2}.{3}.{4}".format(hostname,current.month,current.day,
                                     current.hour,current.minute)

  files['throughput'] = open(log_dir + "skiplist" +".throughput" +
                             ext + ".log", "w")
  files['sizes'] = open(log_dir + "skiplist" + ".sizes" +
                        ext + ".log", "w")
  files['succ_steals'] = open(log_dir + "skiplist" + ".succsteals" +
                             ext + ".log", "w")
  files['real_steals'] = open(log_dir + "skiplist" + ".realsteals" +
                             ext + ".log", "w")
  files['fail_steals'] = open(log_dir + "skiplist" + ".failsteals" +
                             ext + ".log", "w")

  write_header(files)

  for p in nproc:

    if verbose >= 1:
      print("--- Running tests on {0} cores. ---".format(p))

    files['throughput'].write(str(p) + ',')
    files['sizes'].write(str(p) + ',')
    files['succ_steals'].write(str(p) + ',')
    files['real_steals'].write(str(p) + ',')
    files['fail_steals'].write(str(p) + ',')

    for n in range(len(batch_vals)):
      for s in range(len(sleep_times)):
        for b in range(len(biases)):

          throughput, sizes = 0, 0
          steals = 3 * [0]

          for i in iterations:

            [t,si,st] = run_batch_par(p, i, total_batch_ops,
                                      batch_vals[n], sleep_times[s], biases[b])
            throughput = throughput + t
            sizes = sizes + si
            steals = map(add, steals, st)

          throughput = float(throughput) / len(iterations)
          sizes = float(sizes) / len(iterations)
          steals = [float(x) / len(iterations) for x in steals]

          files['throughput'].write("{0:.2f},".format(throughput))
          files['sizes'].write("{0:.2f},".format(sizes))
          files['succ_steals'].write("{0:.2f},".format(steals[0]))
          files['real_steals'].write("{0:.2f},".format(steals[1]))
          files['fail_steals'].write("{0:.2f},".format(steals[2]))

    files['throughput'].write('\n')
    files['sizes'].write('\n')
    files['succ_steals'].write('\n')
    files['real_steals'].write('\n')
    files['fail_steals'].write('\n')

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
