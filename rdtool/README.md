An asymptotically efficient race detector for Cilk Plus programs. It
requires a version of Cilk Plus that supports both -fno-inline-detach
and the Batcher runtime system.

TODO:

* We should pick a good name for this tool. Obviously low priority.
* Restructure the file hierarchy and add good Makefiles.
* Split this off into a separate project repo -- it should not be part of Batcher.
* Run with tests other than matmul -- cilksort, cholesky, etc.
* Debug performance issues
* Cleanup the OM data structure.
