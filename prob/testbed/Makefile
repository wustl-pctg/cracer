CILK = ../support/cilkclocal
CFLAGS = -Wall
MAKEDEPEND = $(CILK) -MM
#SRCS = $(shell find . -name "*.cilk" | sed 's/.\///g')
SRCS = $(wildcard *.cilk)
DEPS = $(wildcard util/*.h)
DEPS += $(wildcard util/*.cilkh)

ifeq ($(shell uname -s),Linux)
	CFLAGS += -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
endif
ifeq ($(shell uname -s),Darwin)
	CFLAGS +=
endif

ifeq ($(mode),)
	mode = debug
endif
ifeq ($(mode),test)
	CFLAGS += -O3 -DNDEBUG
endif
ifeq ($(mode),debug)
	CFLAGS += -O0 -g -save-temps -DDEBUG
endif

tests:  information $(SRCS:.cilk=)
all: cilk tests
% : %.cilk $(DEPS)
	$(CILK) $(CFLAGS) -o $@ $<

### I simply can't get automatic dependency generation to work correctly.
### See http://make.paulandlesley.org/autodep.html
# % : %.cilk
# 	@$(MAKEDEPEND) $< > $(basename $<).d; \
# 		cp $*.d $*.P; \
# 		sed -e 's/^[^:]*: *$*.cilk//'  -e 's/ \\$$//' -e 's/^ *//' $*.d | \
# 		tr ' ' '\n' | sed -e 's/$$/:/' >> $*.P; \
# 		rm -f $*.d
# 	$(CILK) -o $@ $<

# -include $(SRCS:.cilk=.P)

cilk:
	cd ..; make
	make clean

.PHONY: clean cilk-clean check-syntax doc information
clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o
	rm -f $(SRCS:.cilk=)

cilk-clean:
	cd ..;make clean
	make clean

check-syntax:
	$(CILK) -o /dev/null -c ${CHK_SOURCES}

doc:
	@command -v doxygen >/dev/null 2>&1 || { echo "Doxygen not installed. Aborting." >&2; exit 1; }
	doxygen Doxyfile

information:
ifneq ($(mode),test)
ifneq ($(mode),debug)
	@echo "Invalid build mode."
	@echo "Only 'mode=test' and 'mode=debug' are currently supported."
	@exit 1
endif
endif
	@echo "Building on "$(mode)" mode"
	@echo ".........................."
