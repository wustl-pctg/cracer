CILK = ../support/cilkclocal
CFLAGS = -O0 -g -save-temps
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

tests: $(SRCS:.cilk=)
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

.PHONY: clean cilk-clean check-syntax
clean:
	rm -f *.i *.cilki *.cilkc *.d *.P *.o
	rm -f $(SRCS:.cilk=)

cilk-clean:
	cd ..;make clean
	make clean

check-syntax:
	$(CILK) -o /dev/null -c ${CHK_SOURCES}
