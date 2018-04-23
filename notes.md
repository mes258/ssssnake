
Order of stuff in `snake.c`
1. Imports
2. Structs
	1. direction
	2. Snake
	3. World
	4. SnakeBodyPart
	5. Apple
3. Declarations
	1. Window
		1. Window
		2. destroy window
	2. Snake
		1. tick_new_snake
		2. snake_append_body_part
		3. Snake *snake_new()
		4. Snake reset
	3. World
		1. tick new world
		2. world reset
		3. World new world
	4. Apple 
		1. Apple new Apple 
	5. Global Vars
		1. ate
		2. QUITGAME
		3. mainApple
	6. Monitor
	7. cwait
	8. cpost 
	9. Threads
		1. AIsnake
		2. Human snake
4. Main
	


new idea: use semaphore to update world, make sure all snakes update only once, then update world again, etc

## New stuff: 

World: 
	* resolve snake collisions 
	* resolve apple eating
	* gets new apples
	* determines winning snake with scores in SHM
	* draw world
Snakes: 
	* determines next step
	* add points to SHM
	* send direction 


## New structs 
world: 
	* Make in main function
	* pass to snake threads
	* update in main function
	* In world: 
		* has list of four snakes
		* has list of apples
		* has winning snake
```
typedef struct {
  LinkedList *apples_list;
  uint8_t max_apples;
  Snake snakes[4];
  uint8_t winningSnake; //number 1,2,3,4 for winning snake
} World;
```

snakes: 
	* Make in threads, add to world (passed to thread)
	* Tick snakes in threads, with semaphores
	* In snake: 
		* has chartype
		* has previous head position 
		* has current head position
		* has direction (UP,DOWN,LEFT,RIGHT)
		* has points
		* has linked list of body coordinates
		* doesn't have world (?)
	
tick world:  
	* go through array of snakes and check each snakes head against every other coordinate in all snakes
		* if overlap, kill snake
		* if head coordinate == apple coordinate, increase points, eat apple, etc
			* change score
			* add new apple and rand(x) and rand(y)
	* Check for collisions with the wall? 
tick snakes: 
	* move forward 1 space
	* return the direction to world (?)



### order: 
world starts
world stops
snake1 starts
snake1 stops
snake2 starts
snake2 stops
snake3 starts
snake3 stops
snake4 starts
snake4 stops
world starts
etc



