parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=arff

all: $(nomePrograma)

$(nomePrograma): main.o arff.o
	gcc -o $(nomePrograma) main.o arff.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

arff.o: arff.h arff.c
	gcc -c arff.c $(parametrosCompilacao)

clean:
	rm -f *~ *.o $(nomePrograma)