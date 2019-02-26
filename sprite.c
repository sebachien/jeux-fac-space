/* sprite.c */
/*Groupe 17: Louis Sebastien / Ridolfi -- Matte Sacha */

#include "SDL.h" 
#include "sprite.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constructor */

sprite * new_(double x , double y , double speed , SDL_Surface *pic , 
	      int pic_pos_start , double pic_size , int type,int life) {
  sprite * new_;
  new_ = (sprite *)malloc(sizeof(sprite));
  new_->coord.x = x;
  new_->coord.y = y;
  
  new_->angle_deg = (pic_pos_start/pic_size) * 10;
  new_->angle_rad = (new_->angle_deg /180) * 3.14159;
  new_->vx = cos(new_->angle_rad);
  new_->vy = sin(new_->angle_rad);
  new_->speed = speed;
  new_->pic = pic;
  new_->pic_pos.x = pic_pos_start;
  new_->pic_pos.y = 0;
  new_->pic_pos.w = pic_size;
  new_->pic_pos.h = pic_size;
  new_->pic_size = pic_size;
  new_->type = type;
  new_->life = life;
  return new_;
}


/*fonction collide with edges of screen*/
void edges_collide(sprite * object, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
  if (object->coord.x < 0) { 
    object->coord.x = SCREEN_WIDTH;
  } 
  else if (object->coord.x > SCREEN_WIDTH +1) {
    object->coord.x = - object->pic_size+1; 
  }
  
  if (object->coord.y < 0) {
    object->coord.y = SCREEN_HEIGHT ;
  }
  else if (object->coord.y > SCREEN_HEIGHT +1) {
    object->coord.y = 0;
  } 
}


/*fonction which test if two object is in collide */

bool collide_test(sprite * objectA, sprite * objectB){
  return sqrt( pow( (objectB->coord.x + objectB->pic_size/2) 
		    -(objectA->coord.x + objectA->pic_size/2),2) 
	       + pow((objectB->coord.y + objectB->pic_size/2) 
		     - (objectA->coord.y + objectA->pic_size/2),2) ) 
    <= objectA->pic_size/2 + objectB->pic_size/2;     
}
