//EECS338 Final Project
//Vincent Portelli and Michael Smith
//April 30, 2018

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>
#include <stdbool.h>

// when you change these number/size variables,
// you also have to change the size of the global variables below
// (as described in the comments)

int SIZE = 40;    // size of world
int NUMSNAKES = 3;// number of snakes
int NUMAPPS = 2;  // number of apples
int GROW = 3;     // number of segments added per apple
int DELAY = 100000; // pause in microseconds

char S = 'X';     // character that represents snake
char A = '@';     // character that represents apple
char EMPTY = '.'; // character that represents empty

char world[40][40];        // needs to be [SIZE][SIZE]
int snakes[100][40*40][2];  // needs to be [NUMSNAKES][SIZE*SIZE][2]
int directions[100];       // needs to be [NUMSNAKES]
                        // also: directions are: { 0 = +x ; 1 = +y ; 2 = -x ; 3 = -y }
int apples[100][2];   //  needs to be [NUMAPPS][2]
bool kill = false;
int snakesleft = 100;

void snake_grow(int n){
  int i = 0;
  int j = 0;

  while(snakes[n][j][0] > -1){ // find the end of the snake
    j++;
  }

  for(i = 0; i < GROW; i++){
    snakes[n][j+i][0] = SIZE; // add GROW available segments, but put them outside of the map (they'll get used later)
    snakes[n][j+i][1] = SIZE;
  }

  snakes[n][j+i][0] = -1; // remember to mark the new end of the snake
  snakes[n][j+i][1] = -1;
}

void draw_world(){
  
  int i;
  int j;

  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      world[i][j] = EMPTY;
    }
  }

  for(i = 0; i < NUMSNAKES; i++){
    j = 0;
    while(snakes[i][j][0] > -1){
      int x = snakes[i][j][0];
      int y = snakes[i][j][1];
      world[x][y] = S;
      j++;
    }
  }

  for(i = 0; i < NUMAPPS; i++){
    int x = apples[i][0];
    int y = apples[i][1];
    world[x][y] = A;
  }
}

void reset_apple(int n){
  int x = rand() % SIZE;
  int y = rand() % SIZE;

  while(world[x][y] != EMPTY){
    x = rand() % SIZE;
    y = rand() % SIZE;
  }
  
  apples[n][0] = x;
  apples[n][1] = y;
}

void reset_snake(int n){
  
  snakes[n][0][0] = -1;
  snakes[n][0][1] = -1;
  snakes[n][1][0] = -1;
  snakes[n][1][1] = -1;
  draw_world();

  int x = rand() % SIZE;
  int y = rand() % SIZE;

  while(world[x][y] != EMPTY){
    x = rand() % SIZE;
    y = rand() % SIZE;
  }
  
  snakes[n][0][0] = x;
  snakes[n][0][1] = y;

}

void kill_snake(int n){
  snakes[n][0][0] = -1;
  snakes[n][0][1] = -1;
}

void check_snakes(){
  
  int collided[NUMSNAKES];
  
  int i;
  int j;
  int k;

  int x1;
  int x2;
  int y1;
  int y2;

  for(i = 0; i < NUMSNAKES; i++){
    collided[i] = 0;
  }

  for(i = 0; i < NUMSNAKES; i++){
    x1 = snakes[i][0][0];
    y1 = snakes[i][0][1];
    if(x1 >= SIZE || y1 >= SIZE || x1 < 0 || y1 < 0){ // check if snake hits wall
      collided[i] = 1;
    }

    for(j = 0; j < NUMSNAKES; j++){
      k = 0;
      while(snakes[j][k][0] > -1){
        if(i != j || k != 0){ // make sure to not check if the snake head is colliding with itself
          x2 = snakes[j][k][0];
          y2 = snakes[j][k][1];
          if(x1 == x2 && y1 == y2){ // check if snake hits another snake
            collided[i] = 1;
          }
        }
        k++;
      }
    }

    for(j = 0; j < NUMAPPS; j++){
      if(x1 == apples[j][0] && y1 == apples[j][1]){
        snake_grow(i);
        reset_apple(j);
      }
    }
    
  }

  for(i = 0; i < NUMSNAKES; i++){
    if(collided[i] == 1){
      if(snakes[i][0][0] != -1){
        snakesleft--;
      }
      if(snakesleft == 0){
        printf("Game Over!\n");
        exit(0);
      }
      if(kill){
        kill_snake(i);
      }else{
        reset_snake(i);
      }
    }
  }
}

void print_world(){
  int i;
  int j;
  int k;
  int head;

  printf("╔");
  for(i = 0; i < SIZE+SIZE; i++){
    printf("═");
  }
  printf("╗\n");

  for(i = 0; i < SIZE; i++){
    printf("║");
    for(j = 0; j < SIZE; j++){
      if(world[i][j] == EMPTY)
        printf("  ");
      else if(world[i][j] == S){

        head = 4;
        for(k = 0; k < NUMSNAKES; k++){
          if(snakes[k][0][0] == i && snakes[k][0][1] == j){
            head = directions[k];
          }
        }

        if(head == 0){
          //printf("▩ ");
          printf("░░");
        }else if(head == 1){
          //printf("▩ ");
          printf("░░");
        }else if(head == 2){
          //printf("▩ ");
          printf("░░");
        }else if(head == 3){
          //printf("▩ ");
          printf("░░");
        }else{
          printf("██");
        }
      }else if(world[i][j] == A)
        printf("🍎");
    }
    printf("║\n");
  }

  printf("╚");
  for(i = 0; i < SIZE+SIZE; i++){
    printf("═");
  }
  printf("╝\n");

  fflush(stdout);
}

void snake_move_foreward(int n){
  int newx = snakes[n][0][0];
  int newy = snakes[n][0][1];

  int tempx;
  int tempy;

  if(directions[n] == 0){
    newx = (newx+1);
  }else if(directions[n] == 1){
    newy = (newy+1);
  }else if(directions[n] == 2){
    newx = (newx-1);
  }else{
    newy = (newy-1);
  }

  int j = 0;

  while(snakes[n][j][0] > -1){

    tempx = snakes[n][j][0];
    tempy = snakes[n][j][1];

    snakes[n][j][0] = newx;
    snakes[n][j][1] = newy;

    newx = tempx;
    newy = tempy;

    j++;
  }
}

int get_next_move(int n){
  
  int path[SIZE][SIZE];
  int queue[SIZE*SIZE][2];
  /*
    in this array we will store all of the information needed for a depth first search:
    1. mark all locations as 0
    2. add the (empty) locations around the starting position to queue + mark start as visited
    3. mark each of the adacent locations with the direction to get back to the current spot:

          4
        3 x 1
          2

    4. for each item in the queue, repeat step 2 and 3 until you find an apple
    5. when you find an apple, trace your way back to the start, then move in the direction that
       you need to go to get to the apple
  */
  int i;
  int j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      path[i][j] = 0;
    }
  }

  int startx = snakes[n][0][0];
  int starty = snakes[n][0][1];

  int x = startx;
  int y = starty;

  int nextx;
  int nexty;

  queue[0][0] = startx;
  queue[0][1] = starty;

  int ql = 0; // queue length: the last entry in the queue
  int qp = 0; // the current position in the queue

  while(qp <= ql && world[x][y] != A){ // build paths until an apple is found or a dead end is reached
    if(x > 0){
      if(world[x-1][y] != S && path[x-1][y] == 0){
        path[x-1][y] = 4;
        ql += 1;
        queue[ql][0] = x-1;
        queue[ql][1] = y;
      }
    }
    if(x < SIZE-1){
      if(world[x+1][y] != S && path[x+1][y] == 0){
        path[x+1][y] = 2;
        ql += 1;
        queue[ql][0] = x+1;
        queue[ql][1] = y;
      }
    }
    if(y > 0){
      if(world[x][y-1] != S && path[x][y-1] == 0){
        path[x][y-1] = 3;
        ql += 1;
        queue[ql][0] = x;
        queue[ql][1] = y-1;
      }
    }
    if(y < SIZE-1){
      if(world[x][y+1] != S && path[x][y+1] == 0){
        path[x][y+1] = 1;
        ql += 1;
        queue[ql][0] = x;
        queue[ql][1] = y+1;
      }
    }
    qp+=1;
    x = queue[qp][0];
    y = queue[qp][1];
  }

  if(qp > ql){ // if the loop was quit because we reached the end of the queue (no apple found),
    qp-=1;     // then just move towards the last position in the queue (longish path)
    x = queue[qp][0];
    y = queue[qp][1];
  }

  nextx = x;
  nexty = y;

  while(nextx != startx || nexty != starty){
    x = nextx;
    y = nexty;
    if(path[x][y] == 2){
      nextx = x-1;
    }
    if(path[x][y] == 4){
      nextx = x+1;
    }
    if(path[x][y] == 1){
      nexty = y-1;
    }
    if(path[x][y] == 3){
      nexty = y+1;
    }
  }

  if(path[x][y] == 2){
    return 0;
  }else if(path[x][y] == 4){
    return 2;
  }else if(path[x][y] == 1){
    return 1;
  }else if(path[x][y] == 3){
    return 3;
  }else{
    return 0;
  }
}

void determine_directions(){
  int i;
  int pipes[NUMSNAKES][2];
  pid_t pid;

  for(i = 0; i < NUMSNAKES; i++){ // set up pipes
    if (pipe(pipes[i])==-1){
      fprintf(stderr, "Pipe Failed #%d\n",i );
      exit(0);
    }
  }

  for(i = 0; i < NUMSNAKES; i++){ // make threads
    pid = fork();

    if (pid < 0){
      fprintf(stderr, "Fork Failed" );
      exit(0);
    }

    if(pid != 0){
      directions[i] = get_next_move(i);

      write( pipes[i][1], &directions[i], sizeof(directions[i]) ); // write new direction to pipe
      close( pipes[i][1] ); // it is important to close the pipes
      close( pipes[i][0] );

      exit(0); // destroy thread
    }
  }

  for(i = 0; i < NUMSNAKES; i++){
    read( pipes[i][0], &directions[i], sizeof(directions[i]) ); // read and put in directions
    close( pipes[i][0] ); // it is important to close the pipes
    close( pipes[i][1] );
  }

}

void move_snakes(){
  
  determine_directions();

  int j;
  for(j = 0; j < NUMSNAKES; j++){
    snake_move_foreward(j);
  }
}

int main(int argc, char *argv[]) {
  int input = 1;
  printf("Hi, welcome to the game. Enter a value between 1 and 99 \nfor each of the questions below.\n\n");
  printf("How many snakes? \n");
  scanf("%d", &input);
  NUMSNAKES = input;
  snakesleft = input;

  printf("How much should the snakes grow per apple? \n");
  scanf("%d", &input);
  GROW = input;

  printf("How many apples? \n");
  scanf("%d", &input);
  NUMAPPS = input;

  int frames = 0;
  printf("How many frames?\n");
  scanf("%d", &input);
  frames = input;

  printf("Do you want to kill the snakes? Enter 0 for kill, 1 for respawn.\n");
  scanf("%d", &input);
  int c = input;
  if(c == 0){
    kill = true;
  }else{
    kill = false;
  }


  int i;

  for(i = 0; i < NUMAPPS; i++){ // place apples
    apples[i][0] = 0;
    apples[i][1] = 0;
  }
  for(i = 0; i < NUMSNAKES; i++){ // place snakes
    snakes[i][1][0] = -1;
    snakes[i][1][1] = -1;
    directions[i] = 0;
  }
  draw_world();
  for(i = 0; i < NUMAPPS; i++){ // place apples
    reset_apple(i);
  }
  for(i = 0; i < NUMSNAKES; i++){ // place snakes
    reset_snake(i);
  }

  for(i = 0; i < frames; i++){ // run the game for 90 frames
    move_snakes();
    check_snakes();
    draw_world();
    print_world();
    usleep(DELAY);
  }
}
