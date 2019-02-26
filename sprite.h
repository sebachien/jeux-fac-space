/* sprite.h */
/*Groupe 17: Louis Sebastien / Ridolfi -- Matte Sacha */

#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <stdbool.h>

/* define the sprite type */
struct Sprite {
  SDL_Rect coord;
  double vx;
  double vy;
  double angle_deg; 
  double angle_rad;
  double speed;
  SDL_Surface *pic;
  SDL_Rect pic_pos;
  double pic_size;
  int type; /* ship = 1 / asteroid : gros = 21, moyen = 22, petit = 23 /
	       alien = 3 / fire = 5 */
  int life;
};

typedef struct Sprite sprite;

/*Fonction declaration*/

sprite * new_(double x, double y, double speed , SDL_Surface *pic , 
	      int pic_pos_start , double pic_size , int type, int life);

void edges_collide(sprite * object, int SCREEN_WIDTH, int SCREEN_HEIGHT);

bool collide_test(sprite * objectA, sprite * objectB);

#endif 

