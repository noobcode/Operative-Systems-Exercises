CC = gcc
CFLAGS = -g -c -Wall
AR = ar
ARFLAGS = rvs
INCLUDES = -I.
LDFLAGS = -L.

.PHONY: clean run valgrind export mtrace

stringlist: stringlist.o liblist.a
	$(CC) $< $(LDFLAGS) $(INCLUDES) -llist -o $@

liblist.a: liblist.o
	$(AR) $(ARFLAGS) $@ $^

liblist.o: liblist.c liblist.h
	$(CC) $(CFLAGS) $< -o $@

stringlist.o: stringlist.c liblist.h
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	@echo "removing garbage"
	-rm *.o *~ core *.a	
	
run:
	./stringlist ./input.txt
	
valgrind: 
	valgrind -v ./stringlist ./input.txt
	
export:
	export MALLOC_TRACE="./mtrace.out"
	
trace:	
	mtrace ./stringlist ${MALLOC_TRACE}
		
