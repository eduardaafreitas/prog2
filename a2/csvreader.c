#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"


int main( int argc, char *argv[]){
    int opt = 0;

    //tratamentos entrada
    if (argc < 1){
        fprintf(stderr, "Forma de uso: ./csvreader <arq_in> \n");
        exit(3);
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo){
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        exit(4);
    }
    
    unsigned long column = count_columns(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    unsigned long row = count_rows(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    csv *keeper = alloc_csv();
    base *database = alloc_database();

    printf("passou allocs\n");

    armazenar(arquivo, keeper, database, row, column);
//--------------------------------------------------------
    printf("==== LEITOR DE ARQUIVOS CSV ====\n");
    options();


    scanf("%d", &opt); //selecionando opcao desejada

    while (opt != 3) {
        if (opt == 1){
            sumario(keeper, row, column);
        }
        else if (opt == 2){
            printf("em desenvolvimento!\n");
            //mostrar(keeper, rows, columns);
        }
        else{
            fprintf(stderr, "Digite uma opcao valida!\n");
        }
        printf("Pressione Enter para continuar\n\n");
        getchar();
        options();
        opt = 0;
        scanf("%d", &opt);
    }

    free_csv(keeper);
    printf("Fim do programa!\n");
    return 0 ;
}