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
	shot *next;
	shot *removed;

	if (list == NULL) {
		return NULL;
	} else if (current == NULL) {
		return NULL;
	} else if (current == list->first) {
		removed = list->first;
		list->first = list->first->next;
		if (list->first == NULL) {
			list->last = NULL;
		}
	} else {
		removed = current;
		previous->next = current->next;
		if (current == list->last) {
			list->last = previous;
		}
	}
	next = removed->next;
	free(removed);
	return next;
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

	shot *p = (shot*) list->first;
	shot *q = NULL;

	while (p != NULL) {
		if (p->position_y == 0) {
			p = remove_shot(p, q, list);
		} else if (board->map[p->position_y - 1][p->position_x].entity != NULL) {
			p = remove_shot(p, q, list);
		} else {
			board->map[p->position_y][p->position_x].entity = NULL;
			p->position_y--;
			board->map[p->position_y][p->position_x].entity = p;
			q = p;
			p = p->next;
		}
	}

}

shot *straight_shoot(space *board, shot_sentinel *list, enemy *shooter)
{
	shot *new_shot = (shot*) malloc (sizeof(shot));
	if (new_shot == NULL) return NULL;

	new_shot->position_x = shooter->position_x;
	new_shot->position_y = shooter->position_y;

	if (list->first == NULL) {
		list->first = new_shot;
		list->last = new_shot;
	} else {
		list->last->next = new_shot;
		list->last = new_shot;
	}
	return new_shot;
}


int add_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Adiciona um inimigo no tabuleiro. Essa tarefa inclui a alocação do mesmo
	enemy *new_enemy = (enemy*) malloc (sizeof(enemy));

	new_enemy->position_x = position_x;
	new_enemy->position_y = position_y;

	board->map[position_y][position_x].entity = new_enemy;

	return 0;
}

int remove_enemy(space *board, int position_y, int position_x){
//IMPLEMENTAR!
//Remove um inimigo do tabuleiro. Essa tarefa inclui a desalocação do mesmo
	enemy *removed_enemy = (enemy*) board->map[position_y][position_x].entity;

	if (removed_enemy == NULL) {
		return 0;
	} else {
		free(removed_enemy);
		board->map[position_y][position_x].entity = NULL;
		return 1;
	}
	return 0;
}
