#ifndef __GAME__
#define __GAME__

#include <allegro5/allegro5.h>																										
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_ttf.h>	 	
#include <allegro5/allegro_image.h>		
#include <allegro5/keyboard.h>																					
#include <allegro5/allegro_primitives.h>																							

#include <stdio.h>	
#include "player.h"
#include "map.h"

 
#define BG_PATH "resources/bg.jpg"
#define Z_PATH "resources/zangief.png"
#define D_PATH "resources/deejay.png"

void inicia_allegro(bool teste, char *descricao);
void state_init();

#endif