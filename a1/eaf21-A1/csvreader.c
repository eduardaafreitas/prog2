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
    
    int qtd_colunas = conta_colunas(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    int qtd_linhas = conta_linhas(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    char*** dados = processa_arquivo(arquivo, qtd_linhas, qtd_colunas);
    
    printf("==== LEITOR DE ARQUIVOS CSV ====\n");
    options();

    //selecionando opcao desejada

    scanf("%d", &opt);

    while (opt != 3) {
        if (opt == 1){
            sumario(dados, qtd_linhas, qtd_colunas);
        }
        else if (opt == 2){
            mostrar(dados, qtd_linhas, qtd_colunas);
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

    libera(dados, qtd_linhas, qtd_colunas);
    printf("Fim do programa!\n");
    return 0 ;
}