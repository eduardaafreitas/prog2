#ifndef __SPRITES__
#define __SPRITES__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "map.h"
#include "player.h"

void sprites_player (ALLEGRO_BITMAP *sprite, character *player);

#endif