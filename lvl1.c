
#include "SDL.h"
#include "math.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "sprite_list.h"
//#include <SDL/SDL_ttf.h>

#define SPRITE_SIZE    32
#define SHOT_SIZE    8
#define PI 3.14159
#define VITESSE 10
#define NBPLAYERS 1

/*create *sprite as global*/
sprite * ship;     /*The ship*/
sprite * life_1;   /*                    */
sprite * life_2;   /* the player's lifes */
sprite * life_3;;  /*                    */

/* the condition for the main WHILE */
int gameover;
int party;
int pos_fleche;
bool init_fleche;

/* Declaration of keyboard event void */
void update_events(char* keys);
void alternative_HandleEvent(char* key, int * time_ship);

void update_events(char* keys) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        gameover = 1;
        break;
      case SDL_KEYUP:
        keys[event.key.keysym.sym] = 0;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
          case SDLK_q:
            gameover = 1;
            break;
        }
        keys[event.key.keysym.sym] = 1;
        break;
    }
  }
}

/* for an alternative keyborad management */
void alternative_HandleEvent(char* key, int * time_ship) {
  // prepare multiplayer game
  SDLKey tabkey[NBPLAYERS][5] = {{SDLK_UP,SDLK_DOWN,SDLK_LEFT,SDLK_RIGHT,
				  SDLK_SPACE}};
  int i;
  for(i=0;i<NBPLAYERS;i++) {
    if (key[tabkey[i][0]] && *time_ship >= 50 && ship->speed <8 && party == 2 ) {
      /* If key UP is pressed   */
      ship->speed +=2;
      /* increase the ship speed */
    }                         /* Max : 8 */
    if (!key[tabkey[i][0]] && ship->speed >0 && *time_ship >= 50 && party == 2) {
      /* If key UP is not pressed  but speed > 0 */
      ship->speed -= 1;
    }
    if (key[tabkey[i][0]] && pos_fleche == 2) {
      pos_fleche = 1;
    }
    if (key[tabkey[i][1]] && *time_ship >= 50 && ship->speed >-8 && party == 2) {
      /* If key DOWN is pressed   */
      ship->speed -=2;    /* Max -8 */
    }
    if (!key[tabkey[i][1]] && ship->speed <0 && *time_ship >= 50 && party == 2 ) {
      /* If key DOWN is not pressed but speed < 0 */
      ship->speed += 1;
    }
    if (key[tabkey[i][1]] && pos_fleche == 1) {
      pos_fleche = 2;
    }
    if (key[tabkey[i][2]] && *time_ship >=50 && party == 2) { /* If key LEFT is pressed   */
      ship->pic_pos.x += ship->pic_size; /* chose the next ship's picture */
      if (ship->pic_pos.x > 1152-32) {
	ship->pic_pos.x =0;
	/* if it's the end of the cycle, chose the first image */
      }
    }
    if (key[tabkey[i][3]] && *time_ship >= 50 && party == 2) { /* If key RIGHT is pressed   */
      ship->pic_pos.x -= ship->pic_size; /* chose the previous ship's picture */
      if (ship->pic_pos.x < 0) {
	ship->pic_pos.x = 1152-32;
	/* if it's the end of the cycle, chose the last image */
      }
    }
    if (key[tabkey[i][4]] && !party)              /* chose menu action */
      {
	if (pos_fleche == 1) {
	  party = 1;
	}
	if (pos_fleche == 2) {
	  gameover = 1;
	}
      }
  }


  if (*time_ship >= 50) {       /* a timer for slow the ship's animation */
    *time_ship = 0;
  }
  else {
    *time_ship += 1;
  }
}


int main(int argc, char* argv[]) {
  SDL_Surface *screen, *temp, *bg, *menu, *jouer, *fleche, *quit, *pr, *pr1, *pr2, *pr3, *pr4, *pr5, *pship;
  SDL_Rect rcGbg, rcGmenu, rcGjouer,  rcGquit, rcGfleche, rcGpr ,rcGpr1, rcGpr2, rcGpr3, rcGpr4, rcGpr5 ,rcGpship;
  int colorkey; /* a variable for stock the colorkey of sprite */
  list_t list_astero = empty_l();/* create a list for the asteroids */
  int i;                         /* just for a FOR */
  int time = 0;                  /* a timer for */
  int time2 = 0;                 /* a timer for */
  int time_ship;                 /* a timer for the ship's animation*/
  int gm_time;/* a timer for the god-mode when a asteroid touched the ship */
  int score;                     /* a variable for the score...obviously...*/
  list_t shot_l = empty_l();     /* create a list for the shots*/
  int wait = 0;                  /* a timer for slow the rate of fire */
  bool god_mode;                 /* a bool to know if the ship is in god-mode*/
  int nb_astero_start;
  god_mode = false;              /* init god-mode to false*/
  gm_time = 0;                   /* init the timer of god-mode to 0 */
  score = 0;                     /* init the score to 0 */
  time_ship = 0;                 /* init the timer of the ship to 0 */
  nb_astero_start = 5;


  /* initialize SDL */
  SDL_Init(SDL_INIT_VIDEO);

  /* set the title bar */
  SDL_WM_SetCaption("CometBuster", "CometBuster");

  /* create window */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

  /* set keyboard repeat */
  SDL_EnableKeyRepeat(70, 70);

  /*create the ship*/
  temp   = SDL_LoadBMP("ship.bmp");/* load the ship image in a temp variable */

  /* sprite constructor fonction */
  ship = new_(320, 240, 0, SDL_DisplayFormat(temp), 9*32, SPRITE_SIZE, 1, 3);

  SDL_FreeSurface(temp);

  /* setup sprite colorkey and turn on RLE */
  colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
  SDL_SetColorKey(ship->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  /* create the players life */

  temp   = SDL_LoadBMP("heart.bmp");
  colorkey = SDL_MapRGB(screen->format, 0, 255, 255);
  /* life 1 */
  life_1 = new_(0, 0, 0, SDL_DisplayFormat(temp), 0, 32, 0, 0);
  SDL_SetColorKey(life_1->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  /* life 2 */
  life_2 = new_(32, 0, 0, SDL_DisplayFormat(temp), 0, 32, 0, 0);
  SDL_SetColorKey(life_2->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  /* life 3 */
  life_3 = new_(64, 0, 0, SDL_DisplayFormat(temp), 0, 32, 0, 0);
  SDL_SetColorKey(life_3->pic, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

  SDL_FreeSurface(temp);

  /* load background */
  temp  = SDL_LoadBMP("spacelvl1.bmp");
  bg = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* load presentation */
  temp  = SDL_LoadBMP("present.bmp");
  pr = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("present1.bmp");
  pr1 = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("present2.bmp");
  pr2 = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("present3.bmp");
  pr3 = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("present4.bmp");
  pr4 = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("present5.bmp");
  pr5 = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp  = SDL_LoadBMP("pship.bmp");
  pship = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /*load menu */
  temp = SDL_LoadBMP("menu.bmp");
  menu = SDL_DisplayFormat(temp);
  SDL_SetAlpha(menu, SDL_SRCALPHA, 128);
  SDL_FreeSurface(temp);
  temp = SDL_LoadBMP("jouer.bmp");
  jouer = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp = SDL_LoadBMP("fleche.bmp");
  fleche = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);
  temp = SDL_LoadBMP("quit.bmp");
  quit = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  /* setup menu colorkey  */
  colorkey = SDL_MapRGB(screen->format, 0, 255, 0);
  SDL_SetColorKey(jouer, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  colorkey = SDL_MapRGB(screen->format, 0, 255, 0);
  SDL_SetColorKey(quit, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
  colorkey = SDL_MapRGB(screen->format, 0, 255, 255);
  SDL_SetColorKey(fleche, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);



  /*create start list of big asteroid */
  list_astero = init_list_astero(nb_astero_start,screen,list_astero);

  Uint8 *keystate;
  keystate = SDL_GetKeyState(NULL);

  char key[SDLK_LAST] = {0};
  gameover = 1;
  int time_pr1;
  time_pr1 = 0;
  int transp;
  transp = 0;
  bool bpr1, bpr2, bpr3, bpr4, bpr5;
  bpr1 = true;
  bpr2 = false;
  bpr3 = false;
  bpr4 = false;
  bpr5 = false;
  bool transt;
  transt = false;
  SDL_SetAlpha(pr1, SDL_SRCALPHA, 0);
  SDL_SetAlpha(pr2, SDL_SRCALPHA, 0);
  SDL_SetAlpha(pr3, SDL_SRCALPHA, 0);
  SDL_SetAlpha(pr4, SDL_SRCALPHA, 0);
  SDL_SetAlpha(pr5, SDL_SRCALPHA, 0);
  rcGpship.x = 0;
  rcGpship.y = 250;

  /* cinématique of presentation*/
  while (gameover) {
    /* draw the Background */
    rcGpr.x = 0;
    rcGpr.y = 0;
    SDL_BlitSurface(pr, NULL, screen, &rcGpr);
    if (transp >= 255) {
      transt = true;
    }
    if (transp <=-1) {
      transt = false;
      if (bpr5 == true) {
	bpr5 = false;
	SDL_FreeSurface(pr5);
	SDL_FreeSurface(pship);
	gameover = 0;
      }
      if (bpr4 == true) {
	bpr4 = false;
	bpr5 = true;
	SDL_FreeSurface(pr4);
      }
      if (bpr3 == true) {
	bpr3 = false;
	bpr4 = true;
	SDL_FreeSurface(pr3);
      }
      if (bpr2 == true) {
	bpr2 = false;
	bpr3 = true;
	SDL_FreeSurface(pr2);
      }
      if (bpr1 == true) {
	bpr1 = false;
	bpr2 = true;
	SDL_FreeSurface(pr1);

      }
      transp = 0;
    }
    //present 1
    if (bpr1 == true && bpr2 == false && bpr3 == false && bpr4 == false && bpr5 == false) {
      rcGpr1.x = 120;
      rcGpr1.y = 90;
      if (time_pr1 >= 10 && transp <= 255 && transt == false) {
	SDL_SetAlpha(pr1, SDL_SRCALPHA, transp);
	transp += 1;
	time_pr1 = 0;
      }
      if (time_pr1 >= 10 && transp <= 255 && transt == true) {
	SDL_SetAlpha(pr1, SDL_SRCALPHA, transp);
	transp -= 1;
	time_pr1 = 0;
      }
      SDL_BlitSurface(pr1, NULL, screen, &rcGpr1);
    }
    //present 2
    if (bpr1 == false && bpr2 == true && bpr3 == false && bpr4 == false && bpr5 == false) {
      rcGpr2.x = 120;
      rcGpr2.y = 90;
      if (time_pr1 >= 10 && transp <= 255 && transt == false) {
	SDL_SetAlpha(pr2, SDL_SRCALPHA, transp);
	transp += 1;
	time_pr1 = 0;
      }
      if (time_pr1 >= 10 && transp <= 255 && transt == true) {
	SDL_SetAlpha(pr2, SDL_SRCALPHA, transp);
	transp -= 1;
	time_pr1 = 0;
      }
      SDL_BlitSurface(pr2, NULL, screen, &rcGpr1);
    }
    //present 3
    if (bpr1 == false && bpr2 == false && bpr3 == true && bpr4 == false && bpr5 == false) {
      rcGpr3.x = 120;
      rcGpr3.y = 90;
      if (time_pr1 >= 10 && transp <= 255 && transt == false) {
	SDL_SetAlpha(pr3, SDL_SRCALPHA, transp);
	transp += 1;
	time_pr1 = 0;
      }
      if (time_pr1 >= 10 && transp <= 255 && transt == true) {
	SDL_SetAlpha(pr3, SDL_SRCALPHA, transp);
	transp -= 1;
	time_pr1 = 0;
      }
      SDL_BlitSurface(pr3, NULL, screen, &rcGpr1);
    }
    //present 4
    if (bpr1 == false && bpr2 == false && bpr3 == false && bpr4 == true && bpr5 == false) {
      rcGpr4.x = 120;
      rcGpr4.y = 90;
      if (time_pr1 >= 10 && transp <= 255 && transt == false) {
	SDL_SetAlpha(pr4, SDL_SRCALPHA, transp);
	transp += 1;
	time_pr1 = 0;
      }
      if (time_pr1 >= 10 && transp <= 255 && transt == true) {
	SDL_SetAlpha(pr4, SDL_SRCALPHA, transp);
	transp -= 1;
	time_pr1 = 0;
      }
      SDL_BlitSurface(pr4, NULL, screen, &rcGpr4);
    }
    //present 5
    if (bpr1 == false && bpr2 == false && bpr3 == false && bpr4 == false && bpr5 == true) {
      rcGpr5.x = 120;
      rcGpr5.y = 90;
      if (time_pr1 >= 10 && transp <= 255 && transt == false) {
	SDL_SetAlpha(pr5, SDL_SRCALPHA, transp);
	transp += 1;
	time_pr1 = 0;
	rcGpship.x += 1;
	rcGpship.y = 250;
      }
      if (time_pr1 >= 10 && transp <= 255 && transt == true) {
	SDL_SetAlpha(pr5, SDL_SRCALPHA, transp);
	transp -= 1;
	time_pr1 = 0;
	rcGpship.x += 2;
	rcGpship.y = 250;
      }
      SDL_BlitSurface(pr5, NULL, screen, &rcGpr5);
      SDL_BlitSurface(pship, NULL, screen, &rcGpship);
    }
    time_pr1 += 1;
    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

  SDL_FreeSurface(pr);

  gameover = 0;
  party = 0;
  pos_fleche = 1;
  init_fleche = false;


  /* main WHILE */
  while (!gameover) {

    /* event control */
    update_events(key);
    alternative_HandleEvent(key, &time_ship);


    /* if the ship life = 0 stop the main WHILE */
    if(ship->life <= 0) {
      gameover = 1;
    }
    /*if all the asteroids are dead, recreate the start list*/
    if (is_empty(list_astero)) {
      nb_astero_start += 1;
      list_astero = init_list_astero(nb_astero_start,screen,list_astero);
      if (ship->life <3) {
	ship->life += 1;
      }
    }
    /* control the god-mode whith the timer*/
    if(gm_time <= 0 && god_mode) {
      god_mode = false;
    }
    if(gm_time > 0){
      gm_time -= 1;
    }

    /* move the ship if needed */
    if (time_ship >= 50 && ship->speed != 0 ) {
      ship->angle_deg = (ship->pic_pos.x/ship->pic_size) * 10;
      ship->angle_rad = (ship->angle_deg/180)*PI;
      ship->vx = cos(ship->angle_rad);
      ship->vy = sin(ship->angle_rad);
      ship->coord.x += ship->vx*ship->speed;
      ship->coord.y -= ship->vy*ship->speed;
    }

    /* collide with edges of screen for the ship */
    edges_collide(ship, SCREEN_WIDTH, SCREEN_HEIGHT);




    /* draw the Background */
    rcGbg.x = 0;
    rcGbg.y = 0;
    SDL_BlitSurface(bg, NULL, screen, &rcGbg);


    /* if A is pressed creat a shot in the
       list and start the rate of fire timer */
    if (keystate[SDLK_a]  && wait <= 0 && party == 2) {
      shot_l = shot_add(shot_l,ship,screen);
      wait = 400;
    }
    wait -=1;

    /* check the collide astero<-->ship */
    if (!is_empty(list_astero)&& party == 2) {
      collide_check(list_astero,ship,&gm_time,&god_mode);
    }
    /* check the collide astero<-->shot */
    if (!is_empty(list_astero)) {
      if (!is_empty(shot_l)) {
	if (collide_check_2(list_astero, shot_l,screen)) {
	  list_astero = add_2_astero(list_astero,screen,&score);
	}
      }
    }
    /*Move and draw the asteroids and the shots of the lists of DEATH */
    move_shot(shot_l,screen,&time2,ship );
    move_astero(list_astero,screen,&time);

    /* check the shots life and delete the dead shot */
    if (!is_empty(shot_l)) {
      shot_l = delete_check(shot_l);
    }

    /* check the shots life and delete the dead shot */
    if (!is_empty(list_astero)) {
      list_astero = delete_check(list_astero);
    }

    /* draw the menu */
    if (!party) {
      update_events(key);
      alternative_HandleEvent(key, &time_ship);
      rcGmenu.x = 0;
      rcGmenu.y = 0;
      SDL_BlitSurface(menu, NULL, screen, &rcGmenu);
      rcGjouer.x = 220;
      rcGjouer.y = 100;
      SDL_BlitSurface(jouer, NULL, screen, &rcGjouer);
      rcGquit.x = 220;
      rcGquit.y = 300;
      SDL_BlitSurface(quit, NULL, screen, &rcGquit);
      if (init_fleche = false) {
      rcGfleche.x = 200;
      rcGfleche.y = 132;
      init_fleche = true;
      }
      if (pos_fleche == 1) {
	rcGfleche.x = 200;
	rcGfleche.y = 132;
      }
      if (pos_fleche == 2) {
	rcGfleche.x = 200;
	rcGfleche.y = 332;
      }
      SDL_BlitSurface(fleche, NULL, screen, &rcGfleche);
    }
      if (party == 1) {
      SDL_FreeSurface(menu);
      SDL_FreeSurface(jouer);
      SDL_FreeSurface(fleche);
      SDL_FreeSurface(quit);
      party = 2;
      god_mode = true;
      gm_time = 2000;
      }
    /* draw the sprite of the ship*/
      if ((!god_mode || gm_time % 200 <= 100) && party == 2) {
      SDL_BlitSurface(ship->pic, &ship->pic_pos, screen, &ship->coord);
    }

    /* draw life */
    life_update(ship,life_1,life_2,life_3,screen);

    /* update the screen */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }
  /* print the score */
  printf("\n\n");
  printf("-----------------------------\n");
  printf("---PARTIE TERMINEE\n");
  printf("---Votre score : %d\n",score);
  printf("-----------------------------\n\n");
  /* clean up */
  SDL_FreeSurface(ship->pic);
  SDL_FreeSurface(bg);
  free_list (list_astero);
  free_list (shot_l);
  free(life_1);
  free(life_2);
  free(life_3);
  SDL_Quit();

  return 0;
}
