This repo contains the Batcher runtime system. One version is a
modification of Cilk 5, the other of Cilk Plus. It also includes code
for race detection and for several batched data structures. Later,
some of this may be split into separate project repos, especially the
data structures. The code for flat combining is also included,
although this should really be a git submodule linked to the original
authors' repo.

# TODO
- Remove flat combining code
- Archive Cilk 5 version and remove it
- Split rdtool stuff into separate project
- Clean up the code
- Add more data structures
