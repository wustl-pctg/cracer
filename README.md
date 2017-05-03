# Batcher Runtime 

This repo contains the Batcher runtime system. Batcher automatically
groups data structure operations into groups and calls the appropriate
batched data structure operation. This makes it easy to develop
parallel algorithms that use shared data structures, provided you can
write a decently-parallel batched operation.

- `cilkplusrts`: The Cilk Plus version, with changes for rdtool.
- `rdtool`: A provably-good race detector using a modified version of Batcher.
- `ds`: Unfinished batched data structures that were for the Cilk 5 version.
- `cilk5rts`: The Cilk 5 version of the runtime. No longer in use.
- `logs`: An old directory that should be removed...
- `scripts`: A set of scripts, mostly used for running the cilk5 benchmarks.

Note: The code in cilkplusrts currently contains some modifications to work
with the code in rdtool. I *think* I haven't broken any of the normal
Batcher functionality, but I haven't tested this.

# TODO
- Archive Cilk 5 version and remove it
- Split rdtool stuff into separate project
- Finish and convert data structures to use Cilk Plus
- Compare to flat combining. We did this initially but the results
  were too weird to get anything useful from a comparison. We may have
  done something wrong.
