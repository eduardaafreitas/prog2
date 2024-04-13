parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=csvreader

all: $(nomePrograma)

$(nomePrograma): csvreader.o io.o lib.o
	gcc -o $(nomePrograma) csvreader.o io.o lib.o $(parametrosCompilacao)

csvreader.o: csvreader.c
	gcc -c csvreader.c $(parametrosCompilacao)

io.o: io.h io.c
	gcc -c io.c $(parametrosCompilacao)

lib.o: lib.h lib.c
	gcc -c lib.c $(parametrosCompilacao)

clean:
	rm -f *.o 

purge:
	rm -f *~ *.o $(nomePrograma)