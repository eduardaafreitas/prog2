#include <stdio.h>

/*--------------------------structs--------------------------*/

typedef struct noticia {
    char* titulo;
    char* texto;
    int idade;
    noticia_t* prox;
    noticia_t* anterior;
} noticia_t;

typedef struct fila {
    noticia_t* comeco;
    noticia_t* fim;
    
} fila_t;

/*--------------------------funcoes--------------------------*/

void requisita(char *titulo, char *texto){
    getchar();
    printf("\nDigite o título: ");
    fgets(titulo, 33, stdin);
    printf("Digite o texto: ");
    fgets(texto, 513, stdin);
}

fila_t* cria_fila() {
    fila_t* fila = malloc(sizeof(fila));
    if (fila == NULL) {
        printf("\n Erro ao alocar memória para a fila");
        exit(0);
    }   
    fila->comeco = NULL;
    fila->fim = NULL;
    return fila;
}

void insere_fim(fila_t* fila, noticia_t* nova_noticia) {
	noticia_t* nova_noticia = malloc(sizeof(nova_noticia));

    nova_noticia->titulo = malloc(sizeof(char) * 33);
    nova_noticia->texto = malloc(sizeof(char) * 513);

	if (fila_vazia(fila) == 1){
		fila->comeco = nova_noticia;
		fila->fim = nova_noticia;
	} else {
		//adiciona o novo e atualiza o ponteiro para o fim
		nova_noticia->anterior = fila->fim;
		fila->fim->prox = nova_noticia;
		fila->fim = nova_noticia;
	}
}

void retira_comeco(fila_t* fila){
	noticia_t* aux = fila->comeco;

	if (aux->prox != NULL){
		fila->comeco = aux->prox;
		aux->prox->anterior = NULL;
		aux->prox = NULL;
	} else {
		fila->comeco = NULL;
		fila->fim = NULL;
	}

	free(aux);
}

int fila_vazia(fila_t* fila){
    if (fila->comeco = NULL)
        return 1;
    return 0;
}

void destroi_fila(fila_t* fila){
    while (fila_vazia(fila)) {
        retira_comeco(fila);
    }
    free(fila);
}

void cadastro(){
    fila_t* breaknews = cria_fila();
    fila_t* informes = cria_fila();
    int tipo_noticia;
    noticia_t* noticia_topo;
    scanf("%d", &tipo_noticia);
    requisita(noticia_topo->titulo, noticia_topo->texto);

    if (tipo_noticia == 0){
        insere_fim(breaknews, noticia_topo);
    } else {
        insere_fim(informes, noticia_topo);
    }
    
}

void finaliza_edicao( fila_t* breaknews, fila_t* informes){
    fila_t* jornal = cria_fila();
    verifica_jornal(fila_t* breaknews, fila_t* informes);
    imprime_jornal();
    destroi_fila();
    destroi_fila();
}

void noticia_valida(fila_t* fila){
    while (fila->comeco != NULL){
        if (fila->comeco->idade > 3){
            retira_comeco(fila);
        }
    }
}


int verifica_jornal(){
    if (breaknews->comeco == NULL && informes->comeco == NULL){
        printf("\t Esta edição foi pulada por falta de notícias!\n");
        return 0;
    }
    return 1;
}

int menu(){
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

void imprime_header(){

    printf("\t ----------- Menu Principal ----------- \n");
    printf("\t 1 - Cadastrar noticia\n");
    printf("\t 2 - Fechar edicao\n");
    printf("\t 3 - Sair do programa\n");
    printf("\t -------------------------------------- \n");
}

void imprime_jornal(){
    printf("\t -------------------------------------- \n");
    printf("\t ====================================== \n");
}

/*-----------------------------------------------------------*/


int main(){

    printf("\t -------------------------------------- \n");
    printf("\t --- Bem-vindo(a) ao Jornal do DInf!--- \n");
    printf("\t -------------------------------------- \n");

    int opcao;
    opcao = menu();

    while(opcao != 0){
        switch (opcao){
        case 1:
            cadastro();
            break;
        
        case 2:
            finaliza_edicao();
            break;
        
        case 3:
            exit(0)
        }
    }
    return 0;
}