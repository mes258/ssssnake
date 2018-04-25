#include "linkedlist.h"
#include <stdlib.h> // calloc, free

LinkedList *linked_list_new() {
  LinkedList *list = calloc(1, sizeof(LinkedList));
  return list;
}

uint32_t linked_list_length(LinkedList *list){
  return list->length;
}

void linked_list_dealloc(LinkedList *list) {
  printf("TEST6\n");
  Node *node;
  node = list->root_node;
  for (uint32_t i = 0; i < list->length; i++) {
    Node *next_node = node->next_node;
    free(node->value);
    free(node);
    node = next_node;
  }
  free(list);
}

void linked_list_add_front(LinkedList *list, void *value) {
  Node *new_root = calloc(1, sizeof(Node));
  new_root->value = value;
  new_root->next_node = list->root_node;
  list->root_node = new_root;
  list->length++;
}

void linked_list_add_snake_back(LinkedList *list, Snake *value) {
  Node *node;
  node = list->root_node;
  if (node == NULL) { // If list is empty add from the front to get a root node
    linked_list_add_front(list, value);
    return;
  }
  for (uint32_t i = 0; i < list->length - 1;
       i++) { // Go to the back of the list
    node = node->next_node;
  }
  Node *new_node = calloc(1, sizeof(Node));
  new_node->value = value;
  node->next_node = new_node;
  list->length++;
}

void linked_list_add_back(LinkedList *list, void *value) {
  Node *node;
  node = list->root_node;
  if (node == NULL) { // If list is empty add from the front to get a root node
    linked_list_add_front(list, value);
    return;
  }
  for (uint32_t i = 0; i < list->length - 1;
       i++) { // Go to the back of the list
    node = node->next_node;
  }
  Node *new_node = calloc(1, sizeof(Node));
  new_node->value = value;
  node->next_node = new_node;
  list->length++;
}

void *linked_list_get_value(LinkedList *list, uint32_t index) {
  printf("TEST7\n");
  if (index >= list->length) {
    return NULL;
  }
  Node *node;
  node = list->root_node;
  for (uint32_t i = 0; i <= index; i++) {
    node = node->next_node;
  }
  return node->value;
}

SnakeBodyPart *linked_list_get_BL_value(LinkedList *list, uint32_t index) {
  if (index >= list->length) {
    return NULL;
  }
  Node *node;
  node = list->root_node;
  for (uint32_t i = 0; i <= index; i++) {
    node = node->next_node;
  }
  return node->value;
}

void *linked_list_get_first(LinkedList *list) { return list->root_node->value; }

void *linked_list_get_last(LinkedList *list) {
  Node *node;
  node = list->root_node;
  for (uint32_t i = 0; i < list->length; i++) {
    node = node->next_node;
  }
  return node->value;
}

void *linked_list_pop_first(LinkedList *list) {
  if (list->root_node == NULL) {
    return NULL;
  }
  Node *old_root = list->root_node;
  list->root_node = old_root->next_node;
  void *old_root_value = old_root->value;
  free(old_root);
  list->length--;
  return old_root_value;
}

void *linked_list_pop_last(LinkedList *list) {
  if (list->root_node == NULL) {
    return NULL;
  }
  Node *last_node = list->root_node;
  for (uint32_t i = 0; i < list->length - 1; i++) {
    last_node = last_node->next_node;
  }
  void *last_node_value = last_node->value;
  free(last_node);
  list->length--;
  return last_node_value;
}

void *linked_list_next(LinkedList *list) {
  if (list->root_node == NULL) {
    return NULL;
  }
  Node *iterator_node = list->iterator_node;
  if (iterator_node == NULL) {
    iterator_node = list->root_node;       // Return the list root node's value
    list->iterator_node = list->root_node; // Reset to root node for the list
  } else {                                 // Walk forward with iterator node
    Node *next_node = iterator_node->next_node;
    list->iterator_node = next_node;
  }
  return iterator_node->value;
}

void linked_list_reset(LinkedList *list) {
  list->iterator_node = list->root_node;
}



//------------------------------------------------------------

int SIZE = 20;    // size of world
int NUMSNAKES = 2;// number of snakes
char S = 'X';     // character that represents snake
char A = '@';     // character that represents apple
char EMPTY = '.'; // character that represents empty


LinkedList *snake_List;

void snake_append_body_part(Snake *snake) {
  printf("TEST1\n");
  SnakeBodyPart *body_part = calloc(1, sizeof(SnakeBodyPart));
  body_part->chartype = 's';
  body_part->posX = snake->prev_posX;
  body_part->posY = snake->prev_posY;
  linked_list_add_front(snake->bodylist, body_part);
}

void snake_reset(Snake *snake) {
  printf("TEST2\n");
  snake->posX = SIZE / 2;
  snake->posY = SIZE / 2;
  snake->points = 0;
  snake->facing_direction = rand() % 4;
  if (snake->bodylist != NULL) {
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
  }
  snake->bodylist = linked_list_new();
  snake_append_body_part(snake);
  printf("TEST4\n");
}

Snake *snake_new() {
  Snake *snake = calloc(1, sizeof(Snake));
  snake_reset(snake);
  return snake;
} 

//--------------------------------------------------------------
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

int main(){
  printf("hi\n");
  snake_List = calloc(1,sizeof(Snake));

  Snake a = *snake_new();

  linked_list_add_snake_back(snake_List, &a);
  char world[SIZE][SIZE];
  draw_world(world, snake_List);
  return 0;
}
