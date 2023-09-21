#include <stdio.h>
#include <stdlib.h>

/*--------------------------structs--------------------------*/

typedef struct noticia_t {
    char titulo[33];
    char texto[513];
    int idade;
    struct noticia_t* prox;
    struct noticia_t* anterior;
} noticia_t;

typedef struct fila_t {
    noticia_t* comeco;
    noticia_t* fim;
    
} fila_t;

/*--------------------------funcoes--------------------------*/

void requisita(char *titulo, char *texto){
    getchar();
    printf("\t Digite o título: ");
    fgets(titulo, 33, stdin);
    printf("\t Digite o texto: ");
    fgets(texto, 513, stdin);
}

int fila_vazia(fila_t* fila){
    if (fila->comeco == NULL)
        return 1;
    return 0;
}

fila_t* cria_fila() {                                       //essa função funciona!!!!!
    fila_t* fila = malloc(sizeof(fila_t));
    if (fila == NULL) {
        printf("\n Erro ao alocar memória para a fila");
        exit(0);
    }   
    fila->comeco = NULL;
    fila->fim = NULL;
    return fila;
}

void insere_fim(fila_t* fila, noticia_t* nova_noticia) {

	if (fila_vazia(fila) == 1){
		fila->comeco = nova_noticia;
		fila->fim = nova_noticia;
        printf("\t if insere fim ok \n");
	} else {
		//adiciona o novo e atualiza o ponteiro para o fim
		nova_noticia->anterior = fila->fim;
		fila->fim->prox = nova_noticia;
		fila->fim = nova_noticia;
        printf("\t else insere else fim ok \n");
	}
}

void retira_comeco(fila_t* fila){
	noticia_t* aux = fila->comeco;
    printf("\t retira começo antes if \n");

	if (aux->prox != NULL){
		fila->comeco = aux->prox;
		aux->prox->anterior = NULL;
		//aux->prox = NULL;
        printf("\t if retira começo ok \n");
	} else {
		fila->comeco = NULL;
		fila->fim = NULL;
        printf("\t else retira começo ok \n");
	}
    printf("%u", aux);
	free(aux);
    printf("\t retira começo depois free \n");
}


void destroi_fila(fila_t* fila){
    while (fila_vazia(fila)) {
        retira_comeco(fila);
    }
    free(fila);
}

void cadastro( fila_t* breaknews, fila_t* informes){
    int tipo_noticia;
    noticia_t* nova_noticia = malloc(sizeof(noticia_t));

    nova_noticia->idade = 0;

    requisita(nova_noticia->titulo, nova_noticia->texto);

    printf("\t Digite o tipo da noticia: ");
    scanf("%d", &tipo_noticia);

    if (tipo_noticia == 0){
        insere_fim(breaknews, nova_noticia);
    } else {
        insere_fim(informes, nova_noticia);
    }
    return;
}

void retira_invalidas(fila_t* fila){
    noticia_t* aux = fila->comeco;
    if (fila_vazia(fila) == 1){
        return;
    }   
    while (aux != NULL){
        if (fila->comeco->idade > 3){
            retira_comeco(fila);
        }
        aux = aux->prox;
    }
}

void verifica_jornal(fila_t* breaknews, fila_t* informes, fila_t* jornal){
    noticia_t* aux_breaknews = breaknews->comeco;
    noticia_t* aux_informes = informes->comeco;
    int control = 0;

    while ((aux_breaknews != NULL) && (control < 2)){
        insere_fim(jornal, breaknews->comeco);
        printf(" passou no insere fim1");
        breaknews->comeco = breaknews->comeco->prox;
        printf("passou 2");
        control++;
    }

    while ((aux_informes != NULL) && (control < 2)){
        insere_fim(jornal, informes->comeco);
        informes->comeco = informes->comeco->prox;
        control++;
    }
    printf("terminou verifica jornal");
}

void imprime_jornal( fila_t* jornal){
    printf("\t -------------------------------------- \n");
    printf("\t ====================================== \n");
    while (fila_vazia(jornal) == 0){
        printf("\t %s", jornal->comeco->titulo);
        printf("\t %s", jornal->comeco->texto);
        retira_comeco(jornal);
        printf("\t == \n");
    }
}

void imprime_fila( fila_t* jornal){
    printf("\t -------------------------------------- \n");
    printf("\t ====================================== \n");
    while (fila_vazia(jornal) == 0){
        printf("\t %s", jornal->comeco->titulo);
        printf("\t %s", jornal->comeco->texto);
        printf("\t antes do retira comeco");
        retira_comeco(jornal);
        printf("\t == \n");
    }
}

void finaliza_edicao( fila_t* breaknews, fila_t* informes){
    fila_t* jornal = cria_fila();
    if (fila_vazia(breaknews) == 1 && fila_vazia(informes) == 1){
        printf("\t Não há notícias para serem impressas!\n");
        return;
    }
    retira_invalidas(breaknews);
    retira_invalidas(informes);
    verifica_jornal(breaknews, informes, jornal);
    imprime_jornal(jornal);
    destroi_fila(breaknews);
    destroi_fila(informes);
    destroi_fila(jornal);
}

void imprime_header(){

    printf("\t ----------- Menu Principal ----------- \n");
    printf("\t 1 - Cadastrar noticia\n");
    printf("\t 2 - Fechar edicao\n");
    printf("\t 3 - Sair do programa\n");
    printf("\t -------------------------------------- \n");
}


int menu_inicial(){
    int opcao;
    imprime_header();
    printf("\t Digite a opcao desejada: ");
    scanf("%d", &opcao);
    printf("\n");
    while (opcao < 1 || opcao > 3){
        printf("\t Opcao invalida, digite novamente: ");
        scanf("%d", &opcao);
    }
    return opcao;
}


/*-----------------------------------------------------------*/


int main(){

    printf("\t -------------------------------------- \n");
    printf("\t --- Bem-vindo(a) ao Jornal do DInf!--- \n");
    printf("\t -------------------------------------- \n");

    fila_t* breaknews = cria_fila();
    fila_t* informes = cria_fila();
    int opcao;

    noticia_t* nova_noticia = malloc(sizeof(noticia_t));

    requisita(nova_noticia->titulo, nova_noticia->texto);

    insere_fim(breaknews, nova_noticia);
    imprime_fila(breaknews);
    //retira_comeco(breaknews);






    //opcao = menu_inicial();
/*    while(opcao != 3){
        switch (opcao){
        case 1:
            cadastro(breaknews, informes);
            break;
        
        case 2:
            finaliza_edicao(breaknews, informes);
            break;
        
        case 3:
            exit(0);
        }
        opcao = menu_inicial();
    }*/
    return 0;
}