#!/usr/bin/env python
# Run the benchmarks
from __future__ import print_function
import os, sys, subprocess, shlex
import StringIO

cilkscreen = "/home/rob/src/cilktools-linux-004421/bin/cilkscreen"
use_cilkscreen = False
print_status = True # Makes log file ugly
column_size = 22
column_format = "{: >" + str(column_size) + "},"
status_column = 200

# For now, returns just the runtime, in a list
def parse_result(bench_type, proc):
    ## @todo check for errors with err
    out, err = proc.communicate()
    buf = StringIO.StringIO(out)
    line = buf.readline()
    return [int(line[:-1])]

def runit(n, bench_type, prog, args, env):
    cmd = prog + " " + args
    results = []
    for i in range(n):
        proc = subprocess.Popen(cmd.split(), shell=False, env=env,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)
        results += parse_result(bench_type, proc)
    avg = sum(results) / float(n)
    return "{0:.2f}".format(avg)

def pre_status(prev_line):
    if not print_status: return
    start_time = subprocess.check_output("date".split(), shell=False)
    s = "Started latest at {}".format(start_time)[0:-1]
    s = s.rjust(status_column - len(prev_line))
    print(s, end='')
    sys.stdout.flush()    

def post_status(new_line):
    if print_status:
        new_line = '\r' + new_line
    print(new_line, end='')
    sys.stdout.flush()

def print_header(comp):
    commit = subprocess.check_output("git rev-parse HEAD".split(), shell=False)[0:-1]
    print("Running benchmarks with tool built from {}".format(commit))
    print("P", end='')
    for c in comp:
        print(column_format.format(c), end='')
    if use_cilkscreen:
        print(column_format.format("cilkscreen"), end='')
    print()


def run_tests():
    num_iter = 3
    cores = [1] + range(2,17,2)

    # Currently, knapsack is too short and lu segfaults on 4 procs
    # runs = {"matmul": "-n 32",
    #         "fft": "-n " + str(1024),
    #         "cholesky": "-n 500 -z 5000",
    #         "cilksort": "-n 1000",
    #         "strassen": "-n 256",
    #         "fib": "10",
    #         "fibx": "10",
    #         "heat": "-nx 32 -ny 32 -nt 10",
    #         "nqueens": "5",
    #         "qsort": "1024",
    #         "rectmul": "-x 32 -y 32 -z 32",
    #         # "knapsack": "-benchmark long",
    #         # "lu": "-n 32",
    # }
    runs = {"matmul": "-n 2048",
            "fft": "-n " + str(64),#str(64*1024*1024),
            "cholesky": "-n 3000 -z 30000",
            "cilksort": "-n 25000000",
            "strassen": "-n 4096",
            "fib": "42",
            "fibx": "772",
            "heat": "-nx 2048 -ny 2048 -nt 500",
            "nqueens": "14",
            "qsort": str(64*1024*1024),
            "rectmul": "-x 4096 -y 4096 -z 4096",
            "knapsack": "-n ",
            "lu": "-n ",
    }
    tests = runs.keys()
    args = runs.values()

    comp = ["icc", "base", "insert", "brd", "cilksan"]
    global status_column
    status_column = (len(comp)+1) * (column_size+3) + 25
    if use_cilkscreen: status_column += column_size
    bin_dir = "bin"

    print_header(comp)

    for i in range(0,len(tests)):
        base = tests[i]
        print("------------------------------")
        print("Running {} {} times with '{}':".format(base,num_iter,args[i]))
        base = os.path.join(os.getcwd(), bin_dir, base)
        cilkscreen_result = ""
        cilksan_result = ""

        for p in cores:
            env = dict(os.environ, CILK_NWORKERS=str(p))
            line = "{:2},".format(p)
            print(line, end='')

            for c in comp:
                prog = base + "_" + c
                if c == "cilksan" and p != 1:
                    if not print_status: line = ""
                    line += column_format.format(cilksan_result)
                    post_status(line)
                    continue
                pre_status(line)
                result = runit(num_iter, c, prog, args[i], env)
                if c == "cilksan" and p == 1: cilksan_result = result
                if not print_status: line = ""
                line += column_format.format(result)
                post_status(line)

            if use_cilkscreen:
                if p == 1:
                    prog = cilkscreen + " -- " + base + "_icc"
                    pre_status(line)
                    cilkscreen_result = runit(num_iter, "cilkscreen", prog, args[i], env)
                if not print_status: line = ""
                line += column_format.format(cilkscreen_result)
                post_status(line)

            if print_status:
                s = " " * 84
                s = s.rjust(status_column - len(line))
            else:
                s = ""
            print(s)
            sys.stdout.flush()    

def main(argv=None):
    run_tests()


if __name__ == "__main__":
    sys.exit(main())
