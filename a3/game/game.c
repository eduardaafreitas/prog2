#include <allegro5/allegro5.h>																										
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_ttf.h>	 	
#include <allegro5/allegro_image.h>		
#include <allegro5/keyboard.h>																					
#include <allegro5/allegro_primitives.h>																							

#include <stdio.h>	
#include <stdlib.h>																												
#include "map.h"
#include "player.h"
#include "game.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* display;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* background;

character *zangief;
character *deejay;


void inicia_allegro(bool teste, char *descricao){
    if(teste) 
        return;
    fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
    exit(1);
}

void state_init(){
    inicia_allegro(al_init(), "allegro");
    inicia_allegro(al_install_keyboard(), "keyboard");
    inicia_allegro(al_init_image_addon(), "image");
    inicia_allegro(al_init_font_addon(), "font");
    inicia_allegro(al_init_ttf_addon(), "ttf");

    timer = al_create_timer(1.0 / 60.0);
    inicia_allegro(timer, "timer");

    queue = al_create_event_queue();
    inicia_allegro(queue, "queue");

    display = al_create_display(SIZE_X, SIZE_Y);
    inicia_allegro(display, "display");

    font = al_load_font("resources/fonts/fightkid.ttf", 25, 0);
    inicia_allegro(font, "font");

    background = al_load_bitmap(BG_PATH);
    inicia_allegro(background, "background");

    zangief = init_player(al_load_bitmap(Z_PATH));
    deejay = init_player(al_load_bitmap(D_PATH));

	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);

  	int pos_x = 0, pos_y = 0;    

}