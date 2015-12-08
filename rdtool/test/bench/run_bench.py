#!/usr/bin/env python
# Run the benchmarks
from __future__ import print_function
import os, sys, subprocess, shlex
import StringIO

cilkscreen = "/home/rob/src/cilktools-linux-004421/bin/cilkscreen"

def run_tests():
    cores = [1,2,4,6,8]# + (range(2,17,2))
    tests = ["matmul", "fft", "cholesky"]
    args = ["-n 1024", "-n 1048576", "-n 1000 -z 4000"]
    #args = ["-n 32", "-n 32", "-n 100 -z 20"]
    comp = ["base", "insert", "bench", "icc"]
    use_cilkscreen = True

    print("{},".format('P'), end='')
    for c in comp:
        print("{: >20},".format(c), end='')
    if use_cilkscreen:
        print("{: >20},".format("cilkscreen"), end='')
    print()

    for i in range(0,len(tests)):
        base = tests[i]
        print("------------------------------")
        print("Running {}:".format(base))
        base = os.path.join(os.getcwd(), base)

        if use_cilkscreen:
            prog = cilkscreen + " -- " + base + "_icc"
            cmd = prog + " " + args[i]
            proc = subprocess.Popen(cmd.split(), shell=False,
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE)
            out, err = proc.communicate()
            buf = StringIO.StringIO(out)
            cilkscreen_result = buf.readline()[:-1]

        for p in cores:
            print("{},".format(p), end='')
            sys.stdout.flush()
            env = dict(os.environ, CILK_NWORKERS=str(p))

            for c in comp:
                prog = base + "_" + c
                cmd = prog + " " + args[i]

                proc = subprocess.Popen(cmd.split(), env=env, shell=False,
                                        stdout=subprocess.PIPE,
                                        stderr=subprocess.PIPE)
                out, err = proc.communicate()
                buf = StringIO.StringIO(out)
                print("{: >20},".format(buf.readline()[:-1]), end='')
                sys.stdout.flush()

            if use_cilkscreen:
                print("{: >20},".format(cilkscreen_result), end='')
                sys.stdout.flush()

            print()

def main(argv=None):
    run_tests()


if __name__ == "__main__":
    sys.exit(main())
