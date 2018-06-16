CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I.
OBJECTS = hellomake.o func.o

.PHONY: clean cleanall

hellomake: $(OBJECTS)
	$(CC) $^ -o $@ 

hellomake.o: hellomake.c func.c hellomake.h	
func.o: func.c	hellomake.h

clean: 
	@echo "removing garbage"
	-rm -f $(OBJECTS)

cleanall:
	@echo "removing all"
	-rm -f $(OBJECTS) core *~
