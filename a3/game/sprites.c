#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "player.h"
#include "sprites.h"


void sprites_player (ALLEGRO_BITMAP *sprite, character *player){
    player->kick[0] = al_create_sub_bitmap(sprite, 0, 0, 774, 924);
     for(int i = 1;i <= 6;i++)
        player->kick[i] = al_create_sub_bitmap(sprite, (774*i), 0, 774, 924);	

    player->punch[0] = al_create_sub_bitmap(sprite, 0, 154, 774, 924);
    for(int i = 1;i <= 4;i++)
        player->punch[i] = al_create_sub_bitmap(sprite, (774*i), 154, 774, 924);

    player->jumping[0] = al_create_sub_bitmap(sprite, 0, 308, 774, 924);
    for(int i = 1;i <= 4;i++)
        player->jumping[i] = al_create_sub_bitmap(sprite, (774*i), 308, 774, 924);
    
    player->lowered[0] = al_create_sub_bitmap(sprite, 0, 462, 774, 924);
    for(int i = 1;i <= 3;i++)
        player->lowered[i] = al_create_sub_bitmap(sprite, (774*i), 462, 774, 924);

    player->walking[0] = al_create_sub_bitmap(sprite, 0, 616, 774, 924);
    for(int i = 1;i <= 6;i++)
        player->walking[i] = al_create_sub_bitmap(sprite, (774*i), 616, 774, 924);
    
    player->standard[0] = al_create_sub_bitmap(sprite, 0, 770, 774, 924);
    for(int i = 1;i <= 4;i++)
        player->standard[i] = al_create_sub_bitmap(sprite, (774*i), 770, 774, 924);
}

