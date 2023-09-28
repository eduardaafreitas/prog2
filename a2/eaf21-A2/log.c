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
        perror ("Erro ao abrir arquivo - valida_arff");
        exit (1) ;
    }
    char linha[2049];

    //flags:
    int UDP_Flood = 0;
    int Smurf = 0;
    int SIDDOS = 0;
    int HTTP_FLOOD = 0;

    //caso o arquivo nao esteja com o ptr de leitura no @data esse while fara isso

    reposiciona_ponteiro(arff, linha);

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
        if(strstr( linha, "UDP-Flood") != NULL){
            UDP_Flood++;
        }

        if(strstr( linha, "Smurf") != NULL){
            Smurf++;
        }

        if(strstr( linha, "SIDDOS") != NULL){
            SIDDOS++;
        }

        if(strstr( linha, "HTTP-FLOOD") != NULL){
            HTTP_FLOOD++;
        }
    }
    if (UDP_Flood != 0){
        cria_relatorio_ataque("UDP-Flood", UDP_Flood) ;  
    }
    if (Smurf != 0){
        cria_relatorio_ataque("Smurf", Smurf) ;  
    }
    if (SIDDOS != 0){
        cria_relatorio_ataque("SIDDOS", SIDDOS) ;  
    }
    if (HTTP_FLOOD != 0){
        cria_relatorio_ataque("HTTP-FLOOD", HTTP_FLOOD) ;  
    }  
    if (UDP_Flood == 0 || Smurf == 0 || SIDDOS == 0 || HTTP_FLOOD == 0){
        ataque = 1;
    }
}

void cria_relatorio_tamanho(char *nome_ataque, int tamanho){
    FILE *TAMANHO = fopen("R_TAMANHO.txt", "a");
    if(!TAMANHO){
        perror ("Erro ao abrir arquivo - cria_relatorio");
        exit (1) ;
    }
    fprintf(TAMANHO, "%s: %d\n", nome_ataque, tamanho);
    fclose(TAMANHO);
}

//funcao para generalizar o filtro de ataques:
/*int verifica(FILE *arff, atributo *atributos, int quantidade){

    int conta_linha = 0;
    char linha[2049];
    reposiciona_ponteiro(arff, linha);
    int normal = 0;
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
        conta_linha++;
    }
    if(strstr( linha, "Normal") != NULL){
    return conta_linha;
    }
}*/

void filtro_tamanho(FILE *arff, atributo *atributos, int quantidade){ //inicio da funcao que calcula tamanho
    int tamanho = 0;
    char *tok;
    char linha[2049];
    int aux = 0;

    for (int i = 0; i < quantidade; i++){ //pega o indice do atributo PKT_AVG_SIZE
        printf("rotulo: %s\n", atributos[i].rotulo);
        if (strcmp(atributos[i].rotulo, "PKT_AVG_SIZE") == 0){
            aux = i;
        }
    }

    reposiciona_ponteiro(arff, linha);
    while(!feof(arff)){
        if (!fgets(linha, sizeof(linha), arff)) {
            break;
        }
        
        fgets(linha, sizeof(linha), arff);
        for(int i = 0; i < aux; i++){
            tok = strtok(linha, ",");
        }
        tamanho = tamanho + atoi(tok);
    }
    cria_relatorio_tamanho("Tamanho", tamanho);
    
    
    
}
