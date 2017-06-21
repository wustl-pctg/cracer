# CRacer and Batcher Runtime 

This repository contains the [CRacer][cracer] race detection tools. CRacer
is a provably-good race detector for Cilk Plus programs. CRacer has
relatively low overhead (most overhead occurs because of memory
instrumentation) and runs in parallel.

CRacer uses a modified version of the [Batcher][batcher] runtime system, so
that code is also included. Batcher automatically groups data
structure operations into groups and calls the appropriate batched
data structure operation. This makes it easy to develop parallel
algorithms that use shared data structures, provided you can write a
decently-parallel batched operation.

## Contents

- `cilkplusrts`: The Cilk Plus runtime, with changes for Cracer/Batcher.
- `rdtool`: The CRacer tool itself.
- `cilk5rts`: The MIT Cilk 5 version of the Batcher runtime. No longer in use.
- `ds`: Unfinished batched data structures that were for the Cilk 5 version.
- `scripts`: A set of scripts, mostly used for running the cilk5 benchmarks.

## License

The MIT Cilk 5 runtime and our modifications to it are distributed
under the terms of the GNU General Public License. The Intel Cilk Plus
runtime and our modifications to it are distributed under the terms of
the BSD-3-Clause license. All other code in this repository is
distributed under the MIT license, unless otherwise specified.

## Dependencies

To use CRacer you will need a compiler that supports Cilk Plus and the
`-fcilk-no-inline` option. The only known such compiler is
the [WUSTL LLVM branch](https://gitlab.com/wustl-pctg-pub/llvm-cilk). To reproduce performance results, you will
probably also want to use link-time optimization (requires `gold`
linker) and Google's `tcmalloc`.

## Compiling and Using

Instructions for compiling and using CRacer can be found in rdtool/README.md.

## TODO
- Cleanup old scripts.
- Archive Cilk 5 version and remove it
- Split the runtime info a separate project and make sure vanilla Batcher functionality isn't broken.
- Finish and convert data structures to use Cilk Plus
- Compare to flat combining. We did this initially but the results
  were too weird to get anything useful from a comparison.

[cracer]: https://dl.acm.org/citation.cfm?id=2935801
[batcher]: https://dl.acm.org/citation.cfm?id=2612688
