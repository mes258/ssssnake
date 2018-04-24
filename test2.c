#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

#include "linkedlist.h"

int SIZE = 20;    // size of world
int NUMSNAKES = 2;// number of snakes
char S = 'X';     // character that represents snake
char A = '@';     // character that represents apple
char EMPTY = '.'; // character that represents empty


/*
typedef enum { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 } direction;

typedef struct {
  char chartype;
  int posX, posY;
} SnakeBodyPart;

typedef struct {
  char chartype;
  int prev_posX, prev_posY;
  int posX, posY;
  direction facing_direction;
  uint16_t points;
  LinkedList *body_list;
} Snake;

void snake_reset(Snake *snake) {
  snake->posX = SIZE / 2;
  snake->posY = SIZE / 2;
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

Snake *snake_new() {
  Snake *snake = calloc(1, sizeof(Snake));
  snake_reset(snake);
  return snake;
}

SnakeBodyPart *snake_body_part_new() {
  SnakeBodyPart *sbp = calloc(1, sizeof(SnakeBodyPart));
  sbp.chartype = 'X'
  sbp.posX = 0;
  sbp.posY = 0;
  return sbp;
}
*/

void print_world(char world[SIZE][SIZE]){
  int i;
  int j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if(world[i][j] == EMPTY)
        printf(" .");
      else if(world[i][j] == S)
        printf(" X");
      else if(world[i][j] == A)
        printf(" @");
    }
    printf("\n");
  }
  printf("--------- --------- --------- ---------\n");
  fflush(stdout);
}

void draw_world(char world[SIZE][SIZE], int snakes[NUMSNAKES][SIZE*SIZE][2] ){
  
  int i;
  int j;

  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      world[i][j] = EMPTY;
    }
  }

  for(i = 0; i < NUMSNAKES; i++){
    j = 0;
    while(snakes[i][j][0] < SIZE){
      int x = snakes[i][j][0];
      int y = snakes[i][j][1];
      printf("%d, %d, %d\n",i,j,x);
      world[x][y] = S;
      j++;
    }
  }

}




int main(int argc, char *argv[]) {
  char world[SIZE][SIZE];

  int snakes[NUMSNAKES][SIZE*SIZE][2];

  snakes[0][0][0] = 3;
  snakes[0][0][1] = 3;

  snakes[0][1][0] = 3;
  snakes[0][1][1] = 4;

  snakes[0][2][0] = 3;
  snakes[0][2][1] = 5;

  snakes[0][3][0] = 2;
  snakes[0][3][1] = 5;

  snakes[0][4][0] = 1;
  snakes[0][4][1] = 5;

  snakes[0][5][0] = 1;
  snakes[0][5][1] = 6;

  snakes[0][6][0] = SIZE;
  snakes[0][6][1] = SIZE;

  snakes[1][0][0] = 10;
  snakes[1][0][1] = 8;

  snakes[1][1][0] = 10;
  snakes[1][1][1] = 7;

  snakes[1][2][0] = SIZE;
  snakes[1][2][1] = SIZE;

  draw_world(world, snakes);

  print_world(world);

  int fd1[2];
  pid_t p;

  if (pipe(fd1)==-1)
  {
    fprintf(stderr, "Pipe Failed" );
    return 1;
  }

  p = fork();

  if (p < 0)
  {
    fprintf(stderr, "fork Failed" );
    return 1;


  }else if (p > 0){

    int i;
    int n;
    for(i = 0; i < 10; i++){
      read(fd1[0],&n,sizeof(n));
      printf("%d\n",n);
    }
    
  }
  else
  {
    int i;
    for(i = 0; i < 10; i++){
      write(fd1[1],&i,sizeof(i));
    }

    exit(0);
  }


  
}
