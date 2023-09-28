#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "log.h"
#include "arff.h"


void cria_relatorio_ataque(char *nome_ataque, int numero_ocorrencias){
    FILE *ATAQUES = fopen("R_ATAQUES.txt", "a");
    if(!ATAQUES){
        perror ("Erro ao abrir arquivo - cria_relatorio");
        exit (1) ;
    }
    fprintf(ATAQUES, "%s: %d\n", nome_ataque, numero_ocorrencias);
    fclose(ATAQUES);
}


void filtro_ataque(FILE *arff, atributo *atributos, int quantidade){
    
    if(!arff){ //verifica se o arquivo foi aberto
        perror ("Erro ao abrir arquivo - valida_arff");
        exit (1) ;
    }
    char linha[2049];

    //flags:
    int ataque = 0;


    //variaveis auxiliares:
    char *token;
    int UDP_Flood = 0;
    int Smurf = 0;
    int SIDDOS = 0;
    int HTTP_FLOOD = 0;

    //para localizar as linhas(caso necessario):
    int contador = 0;

    //caso o arquivo nao esteja com o ptr de leitura no @data esse while fara isso
    while (fgets(linha, sizeof(linha), arff) != NULL) {
        if (strstr(linha, "@data") != NULL) {
            break; // Encontramos a linha "@data", vamos parar de ler
        }
    }
    fseek(arff, ftell(arff), SEEK_SET);

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
}

