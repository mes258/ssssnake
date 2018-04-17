#include <ncurses.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h> 
#include <pthread.h>

#include "linkedlist.h"
//THREADs for snakes
void *AISnake(void* arg);
void *HumanSnake();

//HELPER FUNCTIONS
typedef enum { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 } direction;

typedef struct {
  LinkedList *apples_list;
  uint8_t max_apples;
  LinkedList *obstacles_list;
  uint8_t max_obstacles;
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
} Apple;

typedef struct {
  int posX, posY;
  int width, height;
} Obstacle;

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

bool ateApple = false; //used to determine when the apple is eaten by a snake
bool QUIT = false; //end game if true
World *world;

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

	*world = world_new();

	// Window border
	WINDOW *border = create_win(LINES, COLS, 0, 0);

	int ch;

	struct timeval start;
	struct timeval now;
	gettimeofday(&start, NULL);
	uint64_t delta;
	uint64_t const DELTA_INTERVAL = 30; // Tick rate in microseconds.

	//Create and call threads for snakes
	pthread_t tid1, tid2, tid3, tid4;
	pthread_create(&tid1, NULL, HumanSnake, NULL);
	pthread_create(&tid2, NULL, AISnake, NULL);
	pthread_create(&tid3, NULL, AISnake, NULL);
	pthread_create(&tid1, NULL, AISnake, NULL);
	
	//wait for threads to be done
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);

	//close window/game
	delwin(root);
  	delwin(border);
  	endwin();
	
}

void *HumanSnake(){
	Snake *snake = snake_new();

	while (!QUIT) {
		// Tick the world
		gettimeofday(&now, NULL);
		delta = ((now.tv_sec * 5000) + (now.tv_usec / 5000)) -
		        ((start.tv_sec * 5000) + (start.tv_usec / 5000));
		if (delta >= DELTA_INTERVAL) {
		  	tick_world(world, delta);
		  	tick_snake(snake);
		  	start = now;
		}

		// Process user input
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
		  	QUIT = true;
		}

		mvprintw(2, COLS / 16, "Score: %i", snake->points);
		mvprintw(2, COLS - COLS / 8, "Highscore: %i", snake->highscore);
		mvprintw(LINES - 2, COLS / 16, "Press Q to quit");
		refresh();
	}
}

void *AISnake(void* arg) {
	double start; //where to start 
	start = *(double*) arg;
}


