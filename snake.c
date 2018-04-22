#include <ncurses.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h> 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>     
#include <sys/types.h>  
#include <semaphore.h>

#include "linkedlist.h"



//Set up structs 
typedef enum { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 } direction;

typedef struct {
  int chartype;
  int prev_posX, prev_posY; // Previous position of the snake's head
  int posX, posY;
  direction facing_direction;
  uint16_t points;
  LinkedList *body_list;
} Snake;

typedef struct {
  LinkedList *apples_list;
  uint8_t max_apples;
  Snake snakes[4];
  char *winner;
} World;

typedef struct {
  int chartype;
  int posX, posY;
} SnakeBodyPart;

typedef struct {
  int chartype;
  uint16_t points_value;
  int x;
  int y;
} Apple;

/** Declarations **/
// Window handling
WINDOW *create_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

// Snake handling
void tick_new_snake(Snake *snake);
void snake_append_body_part(Snake *snake);
Snake *snake_new();
void snake_reset(Snake *snake);

// World management
void tick_new_world(World *world, uint64_t delta);
void world_reset(World *world);
World *world_new();

// Apple handling
Apple *apple_new();

//GLOBAL VARS
bool ate = false; //used to determine when the apple is eaten by a snake
bool QUITGAME = false; //end game if true
//World *world = world_new();
Apple *mainApple;
//WINDOW *border;
//snakes are made in threads

//monitor stuff:
//Set initial values and declare variables
#define BUFFER_SIZE 20

struct condition {
  sem_t sem;
  int count;
};
char buffer[BUFFER_SIZE];
sem_t mutex, next, readers;
struct condition notReading;
int next_count = 0;
int count = 0;
int readcount = 0;

void cwait(struct condition *c);
void cpost(struct condition *c);

//THREADs for snakes
void *AISnake(void *world);
void *HumanSnake(void *world);

//MAIN FUNCTION
int main() {
  notReading.count = 0;
  //set up semaphores
  sem_init(&mutex, 0, 1);
  sem_init(&next, 0, 0);
  sem_init(&(notReading.sem), 0, 0);
  sem_init(&readers, 0, 1);


  //set up world
  srand(time(NULL));
  WINDOW *root = initscr(); /* initialize the curses library */

  cbreak();             /* Line buffering disabled pass on everything to me*/
  keypad(stdscr, true); /* For keyboard arrows  */
  noecho();             /* Do not echo out input */
  nodelay(root, true);  /* Make getch non-blocking */

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color(); /* Start color */

  World *world = world_new();

  // Window border
  WINDOW *border = create_win(LINES, COLS, 0, 0);

  //make threads
  //Create and call threads for snakes
  //array of tids
  pthread_t *tids;
  tids = malloc(sizeof(pthread_t)*4);
  int i = 0;
  int numthreads = 4;
  //create threads, one per snake; the snakes are made in the threads
  for(i = 0; i < numthreads; i++){
    if(i == 0){//create one human snake
      pthread_create(&tids[i], NULL, HumanSnake, &world);
    }else{//the rest are AISnakes
      pthread_create(&tids[i], NULL, AISnake, &world);
    }
  } 

  //run it

  struct timeval start;
  struct timeval now;
  gettimeofday(&start, NULL);
  uint64_t delta;
  uint64_t const DELTA_INTERVAL = 30; // Tick rate in microseconds.

  while (!QUITGAME) {
    //check if all snakes have gone
    sem_wait(&mutex);
    if(readcount != 0){
      cwait(&notReading);
    }
    // Tick the world
    gettimeofday(&now, NULL);
    delta = ((now.tv_sec * 5000) + (now.tv_usec / 5000)) -
            ((start.tv_sec * 5000) + (start.tv_usec / 5000));
    if (delta >= DELTA_INTERVAL) {
      tick_new_world(world, delta);
      start = now;
    }
    //leave so snakes can update
    if(next_count > 0){
      sem_post(&next);
    }else{
      sem_post(&mutex);
    }
  }

	//join threads
	for(i = 0; i < numthreads; i++){
		pthread_join(tids[i], NULL);
	}
	
	//close window/game
	delwin(root);
  delwin(border);
  endwin();
	
}

void *HumanSnake(void *world){
  Snake *snake = snake_new();
  int ch;
  int i = 0;
  for (i = 0; i < 4; i++){
    if(!(world->snakes[i]) == 0){
      world->snakes[i] = snake;
    }
  }

  while (!QUITGAME){
    sem_wait(&mutex);
    sem_wait(&readers);
    readcount++;
    sem_post(&readers);

    tick_new_snake(snake);
    //get User input and figure out direction with it
    ch = getch();
    switch (ch) {
    case KEY_LEFT:
      if (snake->facing_direction != RIGHT) {
          snake->facing_direction = LEFT;
        }
        break;
    case KEY_RIGHT:
        if (snake->facing_direction != LEFT) {
          snake->facing_direction = RIGHT;
        }
        break;
    case KEY_UP:
        if (snake->facing_direction != DOWN) {
          snake->facing_direction = UP;
        }
        break;
    case KEY_DOWN:
        if (snake->facing_direction != UP) {
          snake->facing_direction = DOWN;
        }
        break;
    case 'q':
        QUITGAME = true;
    }

    sem_wait(&readers);
    readcount--;
    sem_post(&readers);

    if(readcount == 0){
      cpost(&notReading);
    }

    if(next_count > 0)
      sem_post(&next);
    else
      sem_post(&mutex);

  }
      
		//mvprintw(2, COLS / 16, "Score: %i", snake->points);
		//mvprintw(2, COLS - COLS / 8, "Highscore: %i", snake->highscore);
		//mvprintw(LINES - 2, COLS / 16, "Press Q to quit");
		//refresh();
	
	return 0;
}

void *AISnake(void *world) {
  Snake *snake = snake_new();
  int i = 0;
  for (i = 0; i < 4; i++){
    if(linked_list_length(world->snakes[i]) == 0){
      world->snakes[i] = snake;
    }
  }
	//figure out where to go.
	while (!QUITGAME){
    sem_wait(&mutex);
    sem_wait(&readers);
    readcount++;
    sem_post(&readers);

		//update snake location
		tick_new_snake(snake);

		//change snake direction
		if(snake->posX < mainApple->x){
			snake->facing_direction = RIGHT;
		}else if(snake->posX > mainApple->x){
			snake->facing_direction = LEFT;
		}else if(snake->posX == mainApple->x){
			//once the x is correct, go to y
			if(snake->posY < mainApple->y){
				snake->facing_direction = UP;
			}else if(snake->posY > mainApple->y){
				snake->facing_direction = DOWN;
			}else if(snake->posY == mainApple->y){
				//tick_snake will take care of this
			}
		}

    sem_wait(&readers);
    readcount--;
    sem_post(&readers);

    if(readcount == 0){
      cpost(&notReading);
    }

    if(next_count > 0)
      sem_post(&next);
    else
      sem_post(&mutex);
		//mvprintw(3, COLS / 16, "Score: %i", snake->points);
	}
	return 0;
}

//METHODS:
void tick_new_snake(Snake *snake){
  mvaddch(snake->posY, snake->posX, ' '); // Clear the old head charachter

  // Save previous position coordinates, useful when rendering the body/tail.
  snake->prev_posY = snake->posY;
  snake->prev_posX = snake->posX;
  // FIXME: Change moving speed depending on vertical/horizontal movement
  // direction
  // Move snake in the facing direction
  switch (snake->facing_direction) {
  case UP:
    snake->posY--;
    snake->chartype = '^';
    break;
  case DOWN:
    snake->posY++;
    snake->chartype = 'v';
    break;
  case LEFT:
    snake->posX--;
    snake->chartype = '<';
    break;
  case RIGHT:
    snake->posX++;
    snake->chartype = '>';
    break;
  }

  SnakeBodyPart *body_part =
      (SnakeBodyPart *)linked_list_pop_last(snake->body_list);
  mvaddch(body_part->posY, body_part->posX, ' '); // Clear the character
  linked_list_add_front(snake->body_list, body_part);
  body_part->posX = snake->prev_posX;
  body_part->posY = snake->prev_posY;
  mvaddch(body_part->posY, body_part->posX, body_part->chartype);

  // Draw head
  mvaddch(snake->posY, snake->posX, snake->chartype);
}


void tick_new_world(World *world, uint64_t delta){
  int i = 0;
  int j = 0;
  int score = 0;
  int winInt = 0; //number of snake, 0 if tie
  for(i = 0; i < 4; i++){
    for(j = 1; j < linked_list_length(world->snakes[i]); j++){
      if(linked_list_get_value(world->snakes[i], 0) == linked_list_get_value(world->snakes[i], j)){
        snake_reset(world->snakes[i]);
      }
    }
    //see if apple is eaten
    if (mvinch(world->snakes[i]->posY, world->snakes[i]->posX) == '@') {
      ate = true;
      world->snakes[i]->points += 10;
      snake_append_body_part(world->snakes[i]);
    }
    //check for collisions with walls
    if (world->snakes[i]->posX >= COLS - 2 || world->snakes[i]->posX <= 0) {
      //kill snake
      snake_reset(world->snakes[i]);
      return;
    } else if (world->snakes[i]->posY >= LINES - 2 || world->snakes[i]->posY <= 0) {
      //kill snake
      snake_reset(world->snakes[i]);
      return;
    }
    //figure out winner
    if(world->snakes[i]->points > score){
      winInt = i+1;
    } else if(world->snakes[i]->points == score){
      winInt = 0;
    }
  }
  //add new apple
  if(ate){
    Apple *apple = apple_new();
    int rand_y = (rand() % (LINES - 2)) + 1;
    int rand_x = (rand() % (COLS - 4)) + 2;
    apple->x = rand_x;
    apple->y = rand_y;
    mvaddch(rand_y, rand_x, apple->chartype);
    linked_list_add_front(world->apples_list, apple);
    mainApple = apple;
    ate = false;
  }
  //update winner
  switch(winInt){
    case 0: 
      world->winner = "It's a tie!";
      break;
    case 1: 
      world->winner = "You!";
      break;
    case 2: 
      world->winner = "AISnake";
      break;
    case 3: 
      world->winner = "AISnake2";
      break;
    case 4: 
      world->winner = "AISnake3";
      break;
  }
}


void snake_append_body_part(Snake *snake) {
  SnakeBodyPart *body_part = calloc(1, sizeof(SnakeBodyPart));
  body_part->chartype = 's';
  body_part->posX = snake->prev_posX;
  body_part->posY = snake->prev_posY;
  linked_list_add_front(snake->body_list, body_part);
}

Snake *snake_new() {
  Snake *snake = calloc(1, sizeof(Snake));
  snake_reset(snake);
  return snake;
}

void snake_reset(Snake *snake) {
  snake->posX = COLS / 2;
  snake->posY = LINES / 2;
  snake->points = 0;
  snake->facing_direction = rand() % 4;
  if (snake->body_list != NULL) {
    // Remove all the snakes body parts from the screen if there are any
    if (snake->body_list->length > 0) {
      uint32_t body_length = snake->body_list->length;
      for (size_t i = 0; i < body_length; i++) {
        SnakeBodyPart *body_part = linked_list_pop_last(snake->body_list);
        mvaddch(body_part->posY, body_part->posX, ' ');
      }
    }
    linked_list_dealloc(snake->body_list);
  }
  snake->body_list = linked_list_new();
  snake_append_body_part(snake);
}


/** World management **/





World *world_new() {
  World *world = calloc(1, sizeof(World));
  world->winner = "It's a tie";
  world->apples_list = linked_list_new();
  world->max_apples = 255;
  Apple *apple = apple_new();
  int rand_y = (rand() % (LINES - 1)) + 1;
  int rand_x = (rand() % (COLS - 2)) + 2;
  apple->x = rand_x;
  apple->y = rand_y;
  mvaddch(rand_y, rand_x, apple->chartype);
  linked_list_add_front(world->apples_list, apple);
  mainApple = apple;
  return world;
}

/** Apple handling **/
//DIFFERENT VALUES FOR APPLES? 
Apple *apple_new() {
  Apple *apple = calloc(1, sizeof(Apple));
  apple->chartype = '@';
  apple->points_value = 10;
  return apple;
}

/** Window handling **/
// Allocs a new window and sets a box around it plus displays it
WINDOW *create_win(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}

// Deallocs the window and removes leftover artefacts
void destroy_win(WINDOW *local_win) {
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners
   * and so an ugly remnant of window.
   */
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}

void cwait(struct condition *c) {
  c->count++;
  if (next_count > 0)
    sem_post(&next);
  else
    sem_post(&mutex);
  sem_wait(&(c->sem));
  c->count--;
}

void cpost(struct condition *c) {
  if (c->count > 0) {
    next_count++;
    sem_post(&(c->sem));
    sem_wait(&next);
    next_count--;
  }
}



