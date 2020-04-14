# Makefile

# Compiler flags
CFLAGS = -Wall -g

# External libraries
LIBS = 

# Pre-defined macros for conditional compilation
DEFS =  

# The final executable program file
BIN = main

# Object files from which $BIN depends
OBJS = node.o graph.o dijkstra.o

# This default rule compiles the executable program
$(BIN): $(OBJS) $(BIN).c
	$(CC) $(CFLAGS) $(DEFS) $(OBJS) $(BIN).c $(LIBS) -o $(BIN).out

# This rule compiles each module into its object file
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $(DEFS) $< -o $@

clean:
	rm -f *~ *.o

depend:
	makedepend -Y -- $(CFLAGS) $(DEFS) -- *.c