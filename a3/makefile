parametrosCompilacao=-Wall -g #-Wshadow
nomePrograma=A3

all: $(nomePrograma)

$(nomePrograma): A3.o enemy.o space.o
	gcc -o $(nomePrograma) A3.o enemy.o space.o $(parametrosCompilacao)

A3.o: A3.c
	gcc -c A3.c $(parametrosCompilacao)

enemy.o: enemy.h enemy.c
	gcc -c enemy.c $(parametrosCompilacao)

space.o: space.h space.c
	gcc -c space.c $(parametrosCompilacao)


clean:
	rm -f *~ *.o output.txt $(nomePrograma)