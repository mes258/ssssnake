#include <ncurses.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h> 
#include <pthread.h>

#include "linkedlist.h"
//THREADs for snakes
void *AISnake();
void *HumanSnake();

//Set up structs 
typedef enum { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 } direction;

typedef struct {
  LinkedList *apples_list;
  uint8_t max_apples;
} World;

typedef struct {
  int chartype;
  int prev_posX, prev_posY; // Previous position of the snake's head
  int posX, posY;
  direction facing_direction;
  uint16_t points;
  uint16_t highscore;
  LinkedList *body_list;
  World *world;
} Snake;

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
void tick_snake(Snake *snake);
void snake_append_body_part(Snake *snake);
Snake *snake_new();
void snake_reset(Snake *snake);

// World management
void tick_world(World *world, uint64_t delta);
void world_reset(World *world);
World *world_new();

// Apple handling
Apple *apple_new();

//GLOBAL VARS
bool ate = false; //used to determine when the apple is eaten by a snake
bool QUIT = false; //end game if true
//World *world = world_new();
Apple *mainApple;
//WINDOW *border;
//snakes are made in threads


//MAIN FUNCTION
int main() {
	//SET UP BOARD AND TIME AND GAME
	srand(time(NULL));
  WINDOW *root = initscr(); /* initialize the curses library */

  cbreak();             /* Line buffering disabled pass on everything to me*/
  keypad(stdscr, true); /* For keyboard arrows 	*/
  noecho();             /* Do not echo out input */
  nodelay(root, true);  /* Make getch non-blocking */

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  start_color(); /* Start color */

  

  // Window border
  WINDOW *border = create_win(LINES, COLS, 0, 0);


	//Create and call threads for snakes
	//array of tids
	pthread_t *tids;
	tids = malloc(sizeof(pthread_t)*4);
	int i = 0;
	//create threads, one per snake; the snakes are made in the threads
	for(i = 0; i < 1; i++){
		if(i == 0){//create one human snake
			pthread_create(&tids[i], NULL, HumanSnake, NULL);
		}else{//the rest are AISnakes
			pthread_create(&tids[i], NULL, AISnake, NULL);
		}
	} 

	//join threads
	for(i = 0; i < 2; i++){
		pthread_join(tids[i], NULL);
	}
	
	//close window/game
	delwin(root);
  	delwin(border);
  	endwin();
	
}

void *HumanSnake(){

	World *world = world_new();


	struct timeval start;
  	struct timeval now;
  	gettimeofday(&start, NULL);
  	uint64_t delta;
  	uint64_t const DELTA_INTERVAL = 30; // Tick rate in microseconds.
  	Snake *snake0 = snake_new();

  	int ch;
	while (!QUIT) {
		// Tick the world
		gettimeofday(&now, NULL);
		delta = ((now.tv_sec * 5000) + (now.tv_usec / 5000)) -
		        ((start.tv_sec * 5000) + (start.tv_usec / 5000));
		if (delta >= DELTA_INTERVAL) {
		  	tick_world(world, delta);
		  	tick_snake(snake0);
		  	start = now;
		}


		//get User input and figure out direction with it
		ch = getch();
		switch (ch) {
		case KEY_LEFT:
			if (snake0->facing_direction != RIGHT) {
		    	snake0->facing_direction = LEFT;
		  	}
		  	break;
		case KEY_RIGHT:
		  	if (snake0->facing_direction != LEFT) {
		    	snake0->facing_direction = RIGHT;
		  	}
		  	break;
		case KEY_UP:
		  	if (snake0->facing_direction != DOWN) {
		    	snake0->facing_direction = UP;
		  	}
		  	break;
		case KEY_DOWN:
		  	if (snake0->facing_direction != UP) {
		    	snake0->facing_direction = DOWN;
		  	}
		  	break;
		case 'q':
		  	QUIT = true;
		}

		//mvprintw(2, COLS / 16, "Score: %i", snake0->points);
		//mvprintw(2, COLS - COLS / 8, "Highscore: %i", snake0->highscore);
		//mvprintw(LINES - 2, COLS / 16, "Press Q to quit");
		refresh();
	}
	return 0;
}

void *AISnake() {
	Snake *snake1 = snake_new();

	//figure out where to go.
	while (!QUIT){
		//update snake location
		tick_snake(snake1);

		//change snake direction
		if(snake1->posX < mainApple->x){
			snake1->facing_direction = RIGHT;
		}else if(snake1->posX > mainApple->x){
			snake1->facing_direction = LEFT;
		}else if(snake1->posX == mainApple->x){
			//once the x is correct, go to y
			if(snake1->posY < mainApple->y){
				snake1->facing_direction = UP;
			}else if(snake1->posY > mainApple->y){
				snake1->facing_direction = DOWN;
			}else if(snake1->posY == mainApple->y){
				//tick_snake will take care of this
			}
		}
		//mvprintw(3, COLS / 16, "Score: %i", snake1->points);
	}
	return 0;
}

void tick_snake(Snake *snake) {
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

  // Check collision with the screen bounds
  if (snake->posX >= COLS - 2 || snake->posX <= 0) {
    // Update highscore
    if (snake->points > snake->highscore) {
      snake->highscore = snake->points;
    }
    snake_reset(snake);
    return;
  } else if (snake->posY >= LINES - 2 || snake->posY <= 0) {
    // Update highscore
    if (snake->points > snake->highscore) {
      snake->highscore = snake->points;
    }
    snake_reset(snake);
    return;
  }
  // TODO: Check collision with the tail

  // Check collsion with apples
  if (mvinch(snake->posY, snake->posX) == '@') {
    ate = true;
    snake->points += 10;
    snake_append_body_part(snake);
  }

  //check if it crashes with another snake
  if(mvinch(snake->posY, snake->posY) == 's'){
  	if (snake->points > snake->highscore) {
      snake->highscore = snake->points;
    }
    snake_reset(snake);
    return;
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
//CAN WE REMOVE THIS? JUST TICK SNAKES AND KEEP APPLES GLOBAL?
void tick_world(World *world, uint64_t delta) {
  static uint64_t tick_steps = 0;
  tick_steps += delta;

  if (tick_steps >= 3000) {
    // Spawn Apple at random location every 3rd
    // untill there are world->max_apples apple on screen.
    //if (world->apples_list->length < world->max_apples) {
    if (ate){
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
    tick_steps = 0; // Reset when used
  }
}

World *world_new() {
  World *world = calloc(1, sizeof(World));
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


