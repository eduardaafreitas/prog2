#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "log.h"
#include "arff.h"
int ataque = 0;

void reposiciona_ponteiro(FILE *arff, char *linha){
    while (fgets(linha, sizeof(linha), arff) != NULL) {
        if (strstr(linha, "@data") != NULL) {
            break; // Encontramos a linha "@data", vamos parar de ler
        }
    }
    fseek(arff, ftell(arff), SEEK_SET);
}

int encontra_rotulo(FILE *arff, atributo *atributos, int quantidade, char *rotulo_procurado){
    int aux = 0;
    for (int i = 0; i < quantidade; i++){ 
        if (strcmp(atributos[i].rotulo, rotulo_procurado) == 0){
            aux = i;
        }
    }
    return aux;
}

void cria_relatorio_ataque(char *nome_ataque, int numero_ocorrencias){
    FILE *ATAQUES = fopen("R_ATAQUES.txt", "a");
    if(!ATAQUES){
        perror ("Erro ao abrir arquivo - cria_relatorio");
        exit (1) ;
    }
    fprintf(ATAQUES, "%s: %d\n", nome_ataque, numero_ocorrencias);
    fclose(ATAQUES);
}

void filtro_ataque(FILE *arff, atributo *atributos, int quantidade){ //funcao feita para o netlog.arff, falta generalizar para outros arquivos tipo ARFF
    
    if(!arff){ //verifica se o arquivo foi aberto
        perror ("Erro ao abrir arquivo - filtro_ataque");
        exit (1) ;
    }
    char linha[2049];

    char** categorias = atributos[quantidade-1].categorias;
    int tamanho_categorias = 0;
    for (; categorias[tamanho_categorias]; tamanho_categorias++);

    int vetor_contador[tamanho_categorias];
    for (int i = 0; i < tamanho_categorias; i++){
        vetor_contador[i] = 0;
    }
    //flags:
    char *token;
    //caso o arquivo nao esteja com o ptr de leitura no @data essa funcao fara isso
    reposiciona_ponteiro(arff, linha);

    int ataque = encontra_rotulo(arff, atributos, quantidade, "PKT_CLASS");
    while(!feof(arff)){
        if (!fgets(linha, sizeof(linha), arff)) {
        break;
        }
        if(strcmp(linha, "\n") == 0){
        continue;
        }
        if(strcmp(linha, "@data") == 0){
        continue;
        }
        
        token = strtok(linha, ",");

        if (token == NULL){
            printf("Erro na linha (token eh NULL)\n");
            exit(1);
        }
        for(int i = 0;i <= ataque;i++){
            if (atributos[ataque].categorias != NULL) {
                for (int j = 0; atributos[ataque].categorias[j] != NULL; j++) {
                    if(strstr(token, atributos[ataque].categorias[j]) != NULL){
                        vetor_contador[j]++;
                    }
                }
            }
            token = strtok(NULL, ",");
        }
        
    }
    for (int i = 0; i < tamanho_categorias - 1 ; i++){
        if (vetor_contador[i] != 0){
            cria_relatorio_ataque(atributos[ataque].categorias[i], vetor_contador[i]);
        }
    }
}



void cria_relatorio_tamanho(char* nome_ataque, char* tamanho){
    FILE *TAMANHO = fopen("R_TAMANHO.txt", "a");
    if(!TAMANHO){
        perror ("Erro ao abrir arquivo - cria_relatorio");
        exit (1) ;
    }
    fprintf(TAMANHO, "%s: %s\n", nome_ataque, tamanho);
    fclose(TAMANHO);
}


/*void converte_num(char *token){
    char *endptr;
    double aux = strtod(token, &endptr);
    if (*endptr == '\0') {
        *media = aux;
    } else {
        printf("Erro na linha (token nao eh numerico)\n");
        exit(1);
    }
}*/


void filtro_tamanho(FILE *arff, atributo *atributos, int quantidade){ //inicio da funcao que calcula tamanho
    
    if(!arff){ //verifica se o arquivo foi aberto
        perror ("Erro ao abrir arquivo - filtro_ataque");
        exit (1) ;
    }
    char linha[2049];

    char** categorias = atributos[quantidade-1].categorias;
    int tamanho_categorias = 0;
    for (; categorias[tamanho_categorias]; tamanho_categorias++);

    char* vetor_contador[tamanho_categorias];
    for (int i = 0; i < tamanho_categorias; i++){
        vetor_contador[i] = 0;
    }
    //flags:
    char *token;

    //caso o arquivo nao esteja com o ptr de leitura no @data essa funcao fara isso
    reposiciona_ponteiro(arff, linha);

    int ataque = encontra_rotulo(arff, atributos, quantidade, "PKT_CLASS");
    int tamanho = encontra_rotulo(arff, atributos, quantidade, "PKT_AVG_SIZE");
    while(!feof(arff)){
        if (!fgets(linha, sizeof(linha), arff)) {
        break;
        }
        if(strcmp(linha, "\n") == 0){
        continue;
        }
        if(strcmp(linha, "@data") == 0){
        continue;
        }
        
        token = strtok(linha, ",");

        if (token == NULL){
            printf("Erro na linha (token eh NULL)\n");
            exit(1);
        }

        //double media = 0;
        //char **ataque
        for(int i = 0;i < quantidade;i++){
            if ( i == ataque ){
                printf("token %s\n", token);
                printf("tamanho %d\n", tamanho);
                printf("ataque %d\n", ataque);
                for(int i = 0;i <= ataque;i++){
                    if (atributos[ataque].categorias != NULL) {
                        for (int j = 0; atributos[ataque].categorias[j] != NULL; j++) {
                            if(strstr(token, atributos[ataque].categorias[j]) != NULL){
                                vetor_contador[j] = token;
                            }
                        }
                    }
                }
            }
            token = strtok(NULL, ",");
            printf("vet contador %s\n", vetor_contador[i]);
        }
    }
            



            /*if (i == tamanho){
                printf(" tamanho: %d\n", tamanho);
                vetor_contador[i] = token;
                printf("%s\n", vetor_contador[i]);
            } 
            else if (i == ataque){
            }
            token = strtok(NULL, ",");*/
}






    
    /*for (int i = 0; i < tamanho_categorias - 1 ; i++){
        if (vetor_contador[i] != 0){
            cria_relatorio_tamanho(atributos[ataque].categorias[i], vetor_contador[i]);
        }
    }*/
