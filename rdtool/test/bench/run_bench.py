#!/usr/bin/env python
# Run the benchmarks
from __future__ import print_function
import os, sys, subprocess, shlex
import StringIO

cilkscreen = "/home/rob/src/cilktools-linux-004421/bin/cilkscreen"
use_cilkscreen = True

def runit(prog, args, env):
    cmd = prog + " " + args
    proc = subprocess.Popen(cmd.split(), shell=False, env=env,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    out, err = proc.communicate()
    buf = StringIO.StringIO(out)
    result = buf.readline()[:-1]
    return result

def pre_status(prev_line):
    start_time = subprocess.check_output("date".split(), shell=False)
    s = "Started latest at {}".format(start_time)[0:-1]
    s = s.rjust(160 - len(prev_line))
    print("{}".format(s), end='')
    sys.stdout.flush()    

def post_status(new_line):
    print("\r{}".format(new_line), end='')
    sys.stdout.flush()

def print_header(comp):
    commit = subprocess.check_output("git rev-parse HEAD".split(), shell=False)[0:-1]
    print("Running benchmarks with tool built from {}".format(commit))
    print("{},".format('P'), end='')
    if use_cilkscreen:
        print("{: >20},".format("cilkscreen"), end='')
    for c in comp:
        print("{: >20},".format(c), end='')
    print()


def run_tests():
    cores = [1] + range(2,17,2)
    tests = ["matmul", "fft", "cholesky"]
    args = ["-n 1024", "-n 1048576", "-n 1000 -z 4000"]
    #args = ["-n 32", "-n 32", "-n 100 -z 20"]
    comp = ["base", "insert", "bench", "icc"]

    print_header(comp)

    for i in range(0,len(tests)):
        base = tests[i]
        print("------------------------------")
        print("Running {}:".format(base))
        base = os.path.join(os.getcwd(), base)
        cilkscreen_result = ""

        for p in cores:
            env = dict(os.environ, CILK_NWORKERS=str(p))
            line = "{:2},".format(p)
            print(line, end='')

            if use_cilkscreen:
                if p == 1:
                    prog = cilkscreen + " -- " + base + "_icc"
                    pre_status(line)
                    cilkscreen_result = runit(prog, args[i], env)
                line += "{: >20},".format(cilkscreen_result)
                post_status(line)

            for c in comp:
                prog = base + "_" + c
                pre_status(line)
                result = runit(prog, args[i], env)
                line += "{: >20},".format(result)
                post_status(line)

            s = " " * 20
            s = s.rjust(160 - len(line))
            print(s)
            sys.stdout.flush()    

def main(argv=None):
    run_tests()


if __name__ == "__main__":
    sys.exit(main())
