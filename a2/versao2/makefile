parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=arff

all: $(nomePrograma)

$(nomePrograma): main.o arff.o log.o
	gcc -o $(nomePrograma) main.o arff.o log.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

arff.o: arff.h arff.c
	gcc -c arff.c $(parametrosCompilacao)

log.o: log.h log.c
	gcc -c log.c $(parametrosCompilacao)


clean:
	rm -f *~ *.o $(nomePrograma)