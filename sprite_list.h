#ifndef SPRITE_LIST_H
#define SPRITE_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "sprite.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define G_ASTERO_SIZE 64

typedef struct List {
  sprite * first;
  struct List * next;
} * list_t;


/*Declaration*/
list_t empty_l(void);
list_t cons (sprite * x, list_t L);
bool is_empty (list_t L);
sprite * first_of (list_t L);
list_t rest (list_t L);
void print_list (list_t L);
void free_list (list_t L);
list_t init_list_astero (int nb, SDL_Surface *screen, list_t list_astero);
void move_astero(list_t l_a,SDL_Surface *screen, int * time);
list_t shot_add (list_t shot_l,sprite * ship, SDL_Surface *screen);
void move_shot(list_t shot_l, SDL_Surface *screen,int * time2 ,sprite * ship);
list_t delete_check(list_t sprite_l);
void collide_check(list_t astero_l, sprite * ship, int *gm_time, bool * god_mode );
bool collide_check_2(list_t astero_l, list_t shot_l,SDL_Surface * screen);
list_t add_2_astero(list_t  list_astero,SDL_Surface * screen, int * score);
void life_update(sprite * ship,sprite * life_1, sprite * life_2, sprite * life_3, SDL_Surface *screen);
#endif

