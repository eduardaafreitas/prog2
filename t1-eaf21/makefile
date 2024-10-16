CFLAG = -Wall
LIBS = -lm
CC = gcc
EXEC = LBP
MAIN = main.c
INCLUDE = lbp.c
OBJS = main.o lbp.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(LIBS) $(CFLAG)

main.o: main.c lbp.h 
lbp.o: lbp.c lbp.h 

clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)