COMPILER_HOME ?= /home/rob/llvm-cilk
#TOOL_HOME ?= /home/rob/devel/cilksan
RUNTIME_INTERNAL ?= /home/rob/devel/batch/cilkplusrts
RUNTIME_LIB ?= /home/rob/llvm-cilk/lib/x86_64/libcilkrts.a
MALLOC=-ltcmalloc # Can also be empty or "-ltbbmalloc_proxy"
