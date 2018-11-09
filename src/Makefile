# compiler
CC = gcc

# compile flags
CFLAGS = -Wall -g

# source files DEPLOYMENT
SRCS = includes/converter.c test.c

# object files
OBJS = $(SRCS:.c=.o)

# target DEPLOYMENT
TARG = test

all: $(TARG)

# generates the target executable
$(TARG):$(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

# generates the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# cleans stuff
clean:
	rm -f $(OBJS) $(TARG) *~
