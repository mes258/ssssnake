#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

#include "linkedlist.h"
#include "linkedlist.c"


int SIZE = 20;    // size of world
int NUMSNAKES = 2;// number of snakes
char S = 'X';     // character that represents snake
char A = '@';     // character that represents apple
char EMPTY = '.'; // character that represents empty


void snake_append_body_part(Snake *snake) {
  printf("TEST1");
  SnakeBodyPart *body_part = calloc(1, sizeof(SnakeBodyPart));
  body_part->chartype = 's';
  body_part->posX = snake->prev_posX;
  body_part->posY = snake->prev_posY;
  linked_list_add_front(snake->bodylist, body_part);
}

void snake_reset(Snake *snake) {
  printf("TEST2");
  snake->posX = SIZE / 2;
  snake->posY = SIZE / 2;
  snake->points = 0;
  snake->facing_direction = rand() % 4;
  /*if (snake->bodylist != NULL) {
    // Remove all the snakes body parts from the screen if there are any
    if (snake->bodylist->length > 0) {
      uint32_t body_length = snake->bodylist->length;
      for (size_t i = 0; i < body_length; i++) {
        SnakeBodyPart *body_part = linked_list_pop_last(snake->bodylist);
        body_part->posY = ' ';
        body_part->posX = ' ';
      }
    }
    printf("TEST3");
    linked_list_dealloc(snake->bodylist);
  }*/
  snake->bodylist = linked_list_new();
  snake_append_body_part(snake);
  printf("TEST4");
}



Snake *snake_new() {
  Snake *snake = calloc(1, sizeof(Snake));
  snake_reset(snake);
  return snake;
}


void print_world(char world[SIZE][SIZE]){
  printf("TEST5");
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

void draw_world(char world[SIZE][SIZE], LinkedList* snakes ){
  int i;
  int j;

  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      world[i][j] = EMPTY;
    }
  }

  int x,y;
  for(i = 0; i < snakes->length; i++){
    Snake* snake = linked_list_get_value(snakes, i);
    j = 0;
    for(j = 0; j < snake->bodylist->length; j++){
      int k = 0;
      for(k = 0; k < snake->bodylist->length; k++){
        if (snake->bodylist != NULL) {
          x = linked_list_get_BL_value(snake->bodylist, k)->posX;
          y = linked_list_get_BL_value(snake->bodylist, k)->posY;
        }
      }
    }
      
      //printf("%d, %d, %d\n",i,j,x);
      world[x][y] = S;
      j++;
    
  }

}

LinkedList *snake_List; 

int main(int argc, char *argv[]) {
  printf("running!");

  char world[SIZE][SIZE];

  snake_List = calloc(1,sizeof(Snake));

  Snake a = *snake_new();

  linked_list_add_snake_back(snake_List, &a);


  /*
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
  */
  

  draw_world(world, snake_List);

  print_world(world);
  /*

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
    
  }else{
    int i;
    for(i = 5; i < 10; i++){
      write(fd1[1],&i,sizeof(i));
    }
    return 0;
  }


  */
}
