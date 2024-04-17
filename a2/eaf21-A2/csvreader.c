#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "lib.h"
#include "lib_filter.h"
#include "lib_description.h"


int main( int argc, char *argv[]){
    int opt = 0;  

    if (argc < 1){
        fprintf(stderr, "Forma de uso: ./csvreader <arq_in> \n");
        exit(1);
    }
    
    FILE *archive = fopen(argv[1], "r");
    if (!archive){
        fprintf(stderr, "Erro ao abrir o archive!\n");
        exit(2);
    }   
    
    unsigned long column, row;
    column = count_columns(archive);
    fseek(archive, 0, SEEK_SET);

    row = count_rows(archive);
    fseek(archive, 0, SEEK_SET);

    //aloca memorias para as structs:
    csv *keeper = alloc_csv();
    base *database = alloc_database();
    base *database_copy = alloc_database();
       
    //armazena o arquivo csv na struct base
    layin_csv(archive, keeper, database, row, column);
    fseek(archive, 0, SEEK_SET);
    // count_stringsize(archive, keeper, row, column);
    layin_csv(archive, keeper, database_copy, row, column);
    fseek(archive, 0, SEEK_SET);
//--------------------------------------------------------

    printf("==== LEITOR DE ARQUIVOS CSV ====\n");
    options();

    scanf("%d", &opt); //selecionando opcao desejada

    while (opt != 9) {
        if (opt == 1){
            sumario(keeper, database);
        }
        else if (opt == 2){
            mostrar(keeper, database);
        }
        else if (opt == 3){
            filtrar(database, keeper, database_copy);
        }
        else if (opt == 4){
            descreve(database, keeper, database_copy);
            printf("descricao em desenvolvimento!\n\n");
        }
        else if (opt == 5){
            printf("ordenacao em desenvolvimento!\n");
        }
        else if (opt == 6){
            printf("selecao em desenvolvimento!\n");
        }
        else if (opt == 7){
            printf("dados faltantes em desenvolvimento!\n");
        }
        else if (opt == 8){
            printf("salvamento de dados em desenvolvimento!\n");
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
    free_database(database);
    printf("Fim do programa!\n");
    return 0 ;
}