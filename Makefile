# Makefile: PhotoLab
# EECS 22 - Assignment 4
# 02/16/2025

# Compiler and Flags
CC = gcc
DEBUG = -g  
CFLAGS = -Wall -std=c11 $(DEBUG) -c
LFLAGS = -Wall $(DEBUG)

# Library name
LIBFILTER = libfilter.a

# Source files
SRCS = PhotoLab.c FileIO.c DIPs.c Advanced.c Image.c Test.c
HDRS = Constants.h FileIO.h DIPs.h Advanced.h Image.h Test.h
OBJS = PhotoLab.o FileIO.o DIPs.o Advanced.o Image.o Test.o
TESTOBJS = PhotoLabTest.o FileIO.o DIPs.o Advanced.o Image.o Test.o

# Convenience targets
all: PhotoLab PhotoLabTest

# Main executable 
PhotoLab: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -L. -lm -o PhotoLab

# Executable for DEBUG mode
PhotoLabTest: $(TESTOBJS)
	$(CC) $(LFLAGS) $(TESTOBJS) -L. -lm -o PhotoLabTest

# Compilation rules
PhotoLab.o: PhotoLab.c $(HDRS)
	$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.o

PhotoLabTest.o: PhotoLab.c $(HDRS)
	$(CC) $(CFLAGS) -DDEBUG PhotoLab.c -o PhotoLabTest.o

FileIO.o: FileIO.c FileIO.h Constants.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h Constants.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

Advanced.o: Advanced.c Advanced.h Constants.h
	$(CC) $(CFLAGS) Advanced.c -o Advanced.o

Image.o: Image.c Image.h
	$(CC) $(CFLAGS) Image.c -o Image.o

Test.o: Test.c Test.h Image.h
	$(CC) $(CFLAGS) -DDEBUG Test.c -o Test.o

# Creating the static library
$(LIBFILTER): DIPs.o Advanced.o
	ar rc $(LIBFILTER) DIPs.o Advanced.o
	ranlib $(LIBFILTER)

# Clean all object files and executables
clean:
	rm -f *.o
	rm -f $(LIBFILTER)
	rm -f PhotoLab PhotoLabTest

# Run test mode
test: PhotoLabTest
	./PhotoLabTest

# Run normal mode
run: PhotoLab
	./PhotoLab
