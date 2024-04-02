parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=csvreader

all: $(nomePrograma)

$(nomePrograma): csvreader.o io.o
	gcc -o $(nomePrograma) csvreader.o io.o $(parametrosCompilacao)

csvreader.o: csvreader.c
	gcc -c csvreader.c $(parametrosCompilacao)

io.o: io.h io.c
	gcc -c io.c $(parametrosCompilacao)


clean:
	rm -f *~ *.o $(nomePrograma)