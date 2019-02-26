
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "sprite_list.h"
#include "sprite.h"
#include <time.h>  

/* Creat a new empty list */
list_t empty_l (void) {
  return NULL;
}

/* the constructor to add a new element in the list */
list_t cons (sprite * x, list_t L) {
  list_t M;
  M = malloc (sizeof(*M));
  M -> first = x;
  M -> next = L;
  return M;
}

/* test if a list is empty or not */
bool is_empty (list_t L) {
  return L == NULL;
}

/* test if a list is empty or not */
sprite * first_of (list_t L) {
  if (is_empty(L)) {
    printf ("Calcul de prem sur une liste vide. \n");
    exit (EXIT_FAILURE);
  }
  return L -> first;
}

/* return all the list whithout the fisrt element */
list_t rest (list_t L){
  if (is_empty(L)) {
    return empty_l();
  }
  return L -> next;
}

/* free the memory of a list */
void free_list (list_t L) {
  if (is_empty (L)){
    return ;
  }
  free_list (rest(L));
  free (L);
}


list_t  init_list_astero (int nb,SDL_Surface *screen,list_t list_astero) {
  int i;
  int x;
  list_astero = empty_l ();
  i=0;
  SDL_Surface * temp  = SDL_LoadBMP("g_asteroid.bmp");
  int colorkey;
  colorkey = SDL_MapRGB(screen->format, 0 , 255 , 255);
  srand ( time(NULL) ); 
  for (i=0;i<nb;i++) {   
    x = rand()%640;
    if ( x < 320) {
      x -= 320;
    }
    else {
      x +=320;
    }
    list_astero =  cons(new_(x,rand()%700-40, 7, SDL_DisplayFormat(temp),
			     64 * ( rand()%35), 64 ,21,1),list_astero);
    SDL_SetColorKey(first_of(list_astero)->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL,
		    colorkey);
    SDL_BlitSurface(first_of(list_astero)->pic, &first_of(list_astero)->pic_pos,
		    screen, &first_of(list_astero)->coord);
  }
  SDL_FreeSurface(temp);
  return list_astero;
}


void move_astero(list_t l_a, SDL_Surface *screen,int * time ) {
  *time = *time + 1;
  list_t pt_tmp = l_a;
  while (!is_empty(pt_tmp)) {
    if ( *time > 90) {
      first_of(pt_tmp)->coord.x += first_of(pt_tmp)->vx*first_of(pt_tmp)->speed;
      first_of(pt_tmp)->coord.y -= first_of(pt_tmp)->vy*first_of(pt_tmp)->speed;
      if (first_of(pt_tmp)->type == 21) {
	first_of(pt_tmp)->pic_pos.x += first_of(pt_tmp)->pic_size;
	if (first_of(pt_tmp)->pic_pos.x >= 2176) {
	  first_of(pt_tmp)->pic_pos.x = 0;
	}	  
      }
      if (first_of(pt_tmp)->type == 22) {
	first_of(pt_tmp)->pic_pos.x += first_of(pt_tmp)->pic_size;
	if (first_of(pt_tmp)->pic_pos.x >= 1120) {
	  first_of(pt_tmp)->pic_pos.x = 0;
	}	  
      }
      if (first_of(pt_tmp)->type == 23) {
	first_of(pt_tmp)->pic_pos.x += first_of(pt_tmp)->pic_size;
	if (first_of(pt_tmp)->pic_pos.x >= 560) {
	  first_of(pt_tmp)->pic_pos.x = 0;
	}	  
      }
    }  
    /* collide with edges of screen */
    edges_collide(first_of(pt_tmp), SCREEN_WIDTH, SCREEN_HEIGHT);
    /* draw big asteroid */
    SDL_BlitSurface(first_of(pt_tmp)->pic, &first_of(pt_tmp)->pic_pos, screen,
		    &first_of(pt_tmp)->coord);
    
    pt_tmp = pt_tmp->next;
  }
  if (*time>90) {
    *time = 0; 
  }
  free_list(pt_tmp);
}

list_t shot_add (list_t shot_l,sprite * ship, SDL_Surface * screen) {
  SDL_Surface * temp  = SDL_LoadBMP("shot.bmp");
  int colorkey;
  shot_l =  cons(new_(ship->coord.x+16,ship->coord.y+16, 10, 
		      SDL_DisplayFormat(temp), 0, 8 , 5,50),shot_l);
  colorkey = SDL_MapRGB(screen->format, 0 , 255 , 255);
  SDL_SetColorKey(first_of(shot_l)->pic,SDL_SRCCOLORKEY | SDL_RLEACCEL,
		  colorkey);
  SDL_BlitSurface(first_of(shot_l)->pic, &first_of(shot_l)->pic_pos, screen,
		  &first_of(shot_l)->coord);
  SDL_FreeSurface(temp);
  first_of(shot_l)->angle_deg = (ship->pic_pos.x/ship->pic_size) * 10;
  first_of(shot_l)->angle_rad = (first_of(shot_l)->angle_deg/180)*3.14159;
  first_of(shot_l)->vx = cos(first_of(shot_l)->angle_rad);
  first_of(shot_l)->vy = sin(first_of(shot_l)->angle_rad);
  return shot_l;
} 

void move_shot(list_t shot_l, SDL_Surface *screen,int * time2, sprite * ship) {
  *time2 = *time2 + 1;
  list_t pt_tmp = shot_l;
  while (!is_empty(pt_tmp)) {
    if ( *time2 > 10) {
      first_of(pt_tmp)->coord.x += first_of(pt_tmp)->vx*first_of(pt_tmp)->speed;
      first_of(pt_tmp)->coord.y -= first_of(pt_tmp)->vy*first_of(pt_tmp)->speed;
      first_of(pt_tmp)->life -= 1;
    }  
    /* collide with edges of screen */
    edges_collide(first_of(pt_tmp), SCREEN_WIDTH, SCREEN_HEIGHT);
    /* draw big asteroid */
    SDL_BlitSurface(first_of(pt_tmp)->pic, &first_of(pt_tmp)->pic_pos,
		    screen, &first_of(pt_tmp)->coord);
      
    pt_tmp = pt_tmp->next;
  }
  if (*time2>10) {
    *time2 = 0; 
  }
  free_list(pt_tmp);
}


list_t delete_check(list_t sprite_l) {
  if(is_empty(sprite_l)) {	
    return empty_l();
  }	
  
  if (first_of(sprite_l)->life <= 0) {
    free(first_of(sprite_l));
    return delete_check(rest(sprite_l));
  }
  return cons(first_of(sprite_l),delete_check(rest(sprite_l)));
}

void collide_check(list_t astero_l, sprite * ship,int *gm_time, bool *god_mode) {
  list_t pt_tmp_astero;
  pt_tmp_astero = astero_l;
  while(!is_empty(pt_tmp_astero)) {
    if(collide_test(ship,first_of(pt_tmp_astero)) && !*god_mode) {
      ship->life -= 1;
      *god_mode = true;
      *gm_time = 2000;
    }
    pt_tmp_astero = pt_tmp_astero->next;
  }
  free_list(pt_tmp_astero);
}


bool collide_check_2(list_t astero_l2, list_t shot_l2,SDL_Surface * screen) {
  list_t pt_tmp_astero2;
  list_t pt_tmp_shot2;
  pt_tmp_astero2 = astero_l2;
  pt_tmp_shot2 = shot_l2;
  bool collide;
  collide = false;
  while(!is_empty(pt_tmp_shot2)) {
    while(!is_empty(pt_tmp_astero2)) {
      if(collide_test(first_of(pt_tmp_astero2),first_of(pt_tmp_shot2))) {
	first_of(pt_tmp_astero2)->life = 0;
	first_of(pt_tmp_shot2)->life = 0;
	collide = true;
      }
      pt_tmp_astero2 = pt_tmp_astero2->next;
    }
    free_list(pt_tmp_astero2);
    list_t pt_tmp_astero2;
    pt_tmp_astero2 = astero_l2;
    pt_tmp_shot2 = pt_tmp_shot2->next;
  }
  free_list(pt_tmp_astero2);
  free_list(pt_tmp_shot2);
  return collide;
}

list_t add_2_astero(list_t list_astero,SDL_Surface * screen,int *score) {
  list_t pt_tmp;
  SDL_Surface * temp;
  int colorkey;
  colorkey = SDL_MapRGB(screen->format, 0 , 255 , 255);
  pt_tmp = list_astero; 
  while(!is_empty(pt_tmp)) {
    if (first_of(pt_tmp)->life <= 0) {
      
      if ( first_of(pt_tmp)->type == 21) {
	*score += 160;
	temp = SDL_LoadBMP("m_asteroid.bmp");
	list_astero =  cons(new_(first_of(pt_tmp)->coord.x,first_of(pt_tmp)->coord.y, 7, SDL_DisplayFormat(temp), 32, 32 ,22,1),list_astero);
	first_of(list_astero)->vx = cos(first_of(pt_tmp)->angle_rad + 0.5* 3.14159);
	first_of(list_astero)->vy = sin(first_of(pt_tmp)->angle_rad + 0.5* 3.14159);
	SDL_SetColorKey(first_of(list_astero)->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	SDL_BlitSurface(first_of(list_astero)->pic, &first_of(list_astero)->pic_pos, screen, &first_of(list_astero)->coord);
	
	list_astero =  cons(new_(first_of(pt_tmp)->coord.x,first_of(pt_tmp)->coord.y,7, SDL_DisplayFormat(temp), 64, 32 ,22,1),list_astero);
	first_of(list_astero)->vx = cos(first_of(pt_tmp)->angle_rad - 0.5* 3.14159);
	first_of(list_astero)->vy = sin(first_of(pt_tmp)->angle_rad - 0.5* 3.14159);
	SDL_SetColorKey(first_of(list_astero)->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	SDL_BlitSurface(first_of(list_astero)->pic, &first_of(list_astero)->pic_pos, screen, &first_of(list_astero)->coord);
	SDL_FreeSurface(temp);
      }
      
      if (first_of(pt_tmp)->type == 22) {
	*score += 320;
	temp = SDL_LoadBMP("p_asteroid.bmp");
	list_astero =  cons(new_(first_of(pt_tmp)->coord.x,first_of(pt_tmp)->coord.y, 8, SDL_DisplayFormat(temp), 16, 16 ,23,1),list_astero);
	first_of(list_astero)->vx = cos(first_of(pt_tmp)->angle_rad + 0.5* 3.14159);
	first_of(list_astero)->vy = sin(first_of(pt_tmp)->angle_rad + 0.5* 3.14159);
	SDL_SetColorKey(first_of(list_astero)->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	SDL_BlitSurface(first_of(list_astero)->pic, &first_of(list_astero)->pic_pos, screen, &first_of(list_astero)->coord);
	
	list_astero =  cons(new_(first_of(pt_tmp)->coord.x,first_of(pt_tmp)->coord.y, 8, SDL_DisplayFormat(temp), 64, 16 ,23,1),list_astero);
	first_of(list_astero)->vx = cos(first_of(pt_tmp)->angle_rad - 0.5* 3.14159);
	first_of(list_astero)->vy = sin(first_of(pt_tmp)->angle_rad - 0.5* 3.14159);
	SDL_SetColorKey(first_of(list_astero)->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	SDL_BlitSurface(first_of(list_astero)->pic, &first_of(list_astero)->pic_pos, screen, &first_of(list_astero)->coord);
	SDL_FreeSurface(temp);
      }
      if (first_of(pt_tmp)->type == 23) {
	*score += 640;
      }
    }
    pt_tmp = pt_tmp->next;
  }
  free_list(pt_tmp);
  
  return list_astero;
  
}

void life_update(sprite * ship,sprite * life_1, sprite * life_2, sprite * life_3,SDL_Surface *screen) {
  
  if(ship->life >=1 && life_1 != NULL) {
    SDL_BlitSurface(life_1->pic, &life_1->pic_pos, screen, &life_1->coord);
  }
  if(ship->life >=2 && life_2 != NULL) {
    SDL_BlitSurface(life_2->pic, &life_2->pic_pos, screen, &life_2->coord);
  }
  if(ship->life >=3 && life_3 != NULL) {
    SDL_BlitSurface(life_3->pic, &life_3->pic_pos, screen, &life_3->coord);
  }
  
}


