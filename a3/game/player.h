#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>																										
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_ttf.h>	 	
#include <allegro5/allegro_image.h>		
#include <allegro5/keyboard.h>																					
#include <allegro5/allegro_primitives.h>																							

#include "map.h"

//sprites zangief:

struct character{
    ALLEGRO_BITMAP* standard[4];
    ALLEGRO_BITMAP* walking[6];
    ALLEGRO_BITMAP* lowered[3];
    ALLEGRO_BITMAP* jumping[4];
    ALLEGRO_BITMAP* punch[4];
    ALLEGRO_BITMAP* kick[3];
    
    int up, down, right, left;
    int punch_flag, kick_flag;
    int pos_x, pos_y;
    int score;
    int health_points;

};
typedef struct character character;

//ALLEGRO_BITMAP* standard, ALLEGRO_BITMAP* walking, ALLEGRO_BITMAP* lowered, ALLEGRO_BITMAP* jumping, ALLEGRO_BITMAP* punch, ALLEGRO_BITMAP* kick
character *init_player();

#endif