#include <allegro5/allegro5.h>																										
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_ttf.h>	 	
#include <allegro5/allegro_image.h>		
#include <allegro5/keyboard.h>																					
#include <allegro5/allegro_primitives.h>																							

#include <stdio.h>																													
#include "map.h"
#include "player.h"
#include "sprites.h"

character *init_player(ALLEGRO_BITMAP* sprite){
    
    character *player;

    player = malloc(sizeof(character));
    if (player == NULL){
        printf("Erro ao alocar memoria (player)\n");
        exit(1);
    }

    player->pos_x = 0;
    player->pos_y = 0;  
    player->up = 0;
    player->down = 0;
    player->right = 0;
    player->left = 0;
    player->punch_flag = 0;
    player->kick_flag = 0;
    player->score = 0;
    player->health_points = 10;

    sprites_player(sprite, player);

    return player;
}
