#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>


int SIZE = 20;    // size of world
int NUMSNAKES = 3;// number of snakes
char S = 'X';     // character that represents snake
char A = '@';     // character that represents apple
char EMPTY = '.'; // character that represents empty

char world[20][20];        // needs to be [SIZE][SIZE]
int snakes[3][20*20][2];  // needs to be [NUMSNAKES][SIZE*SIZE][2]
int directions[3];       // needs to be [NUMSNAKES]
                        // also: directions are: { 0 = +x ; 1 = +y ; 2 = -x ; 3 = -y }


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
      printf("%d, %d, %d\n",i,j,x);
      world[x][y] = S;
      j++;
    }
  }

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
            collided[j] = 1;
          }
        }
        k++;
      }
    }
    
  }

  for(i = 0; i < NUMSNAKES; i++){
    if(collided[i] == 1){
      reset_snake(i);
    }
  }

}

void print_world(){
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

    printf("sn:%d - %d,%d\n",n,newx,newy);
    tempx = snakes[n][j][0];
    tempy = snakes[n][j][1];

    snakes[n][j][0] = newx;
    snakes[n][j][1] = newy;

    newx = tempx;
    newy = tempy;

    j++;
  }
}

void snake_grow(int n){
  int i = 0;
  int j = 0;

  while(snakes[n][j][0] > -1){ // find the end of the snake
    j++;
  }

  for(i = 0; i < 3; i++){
    snakes[n][j+i][0] = SIZE; // add an 3 available segments, but put them outside of the map (they'll get used later)
    snakes[n][j+i][1] = SIZE;
  }

  snakes[n][j+i][0] = -1; // remember to mark the new end of the snake
  snakes[n][j+i][1] = -1;
}



int main(int argc, char *argv[]) {

  directions[0] = 0;
  directions[1] = 2;
  directions[2] = 0;

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

  snakes[0][6][0] = -1; // -1 means end of snake
  snakes[0][6][1] = -1;

  snakes[1][0][0] = 10;
  snakes[1][0][1] = 8;

  snakes[1][1][0] = 10;
  snakes[1][1][1] = 7;

  snakes[1][2][0] = -1; // -1 means end of snake
  snakes[1][2][1] = -1;

  snakes[2][0][0] = 10;
  snakes[2][0][1] = 7;

  snakes[2][1][0] = 10;
  snakes[2][1][1] = 6;

  snakes[2][2][0] = -1; // -1 means end of snake
  snakes[2][2][1] = -1;

  draw_world();

  print_world();

  check_snakes();

  draw_world();
  print_world();

  int i;
  int j;

  for(i = 0; i < 30; i++){
    for(j = 0; j < NUMSNAKES; j++){
      snake_move_foreward(j);
    }
    check_snakes();
    draw_world();
    print_world();
    sleep(1);

  }

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