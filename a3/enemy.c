#include <stdlib.h>

#include "enemy.h"

shot_sentinel* create_shotlist(void){
	shot_sentinel *list = (shot_sentinel*) malloc (sizeof(shot_sentinel));
	if (list == NULL) return NULL;
	list->first = NULL;
	list->last  = NULL;
	return list;
}

shot* remove_shot(shot* current, shot* previous, shot_sentinel *list){
//IMPLEMENTAR!
//	Remove os tiros da lista
}

void clean_shots(shot_sentinel *list){
	shot *p = (shot*) list->first;
	shot *q = NULL;
	while (p != NULL)
		p = remove_shot(p, q, list);
}

void update_shots(space *board, shot_sentinel *list){
//IMPLEMENTAR!
//Os tiros presentes no tabuleiro devem ser atualizados
//  Se o tiro acertar um alvo, ou sair do tabuleiro, ele deve ser removido da lista
//  Caso contrário, ele deve "andar" uma casa (sqm) à frente
}

shot* straight_shot(space *board, shot_sentinel *list, enemy *shooter){
//IMPLEMENTAR!
//Adiciona um novo tiro à lista. Neste momento, todos os tiros se movem apenas para frente
}

int add_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
}

int remove_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
}
