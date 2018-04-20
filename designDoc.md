# SSSSnake
By Vincent Portelli and Michael Smith; For Beta V0

TODO: 
- [ ] semaphore 
- [ ] fix snake threads
- [ ] debug
	- [ ] debug the new errors 
		- [ ] continue to recursively debug stuff


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






## Files: 
* `snake.c`
	* This is the main file. It creates the board, the user snake and all the AI snakes. Run this file to play the game. /Note: As of 04/19/2018, this compiles but is very broken. After running, press "q" to quit./
* `linkedlist.c`
	* This file has all methods for the linkedlist object, linked via `linkedlist.h`. Each snake in the game is a linked list
* `linkedlist.h`
	* This file has the abstractions for `linkedlist.c`
* `makefile`
	* Compiles `snake.c` into `snake`
* `designDoc.md`
	* This file 

## Structs:
* World
	* This struct is the board that the snakes exist in. It contains a list of apples in the world. 
* Snake
	* This struct is the framework for a snake. Each snake has: 
		* `int chartype;` -> The character that makes up the body. 
  		* `int prev_posX, prev_posY;` -> Previous location of the head
  		* `int posX, posY;` -> Current location of the head
  		* `direction facing_direction;` -> Direction the snake is moving
  		* `uint16_t points;` -> Current points this game
  		* `uint16_t highscore;` -> Highscore from all games this session
  		* `LinkedList *body_list;` -> The body
  		* `World *world;` -> The world the snake is in
* Snake Body Part
	* Each body part has a character and a location (x,y)
* Apple
	* Each apple has a character, a value (in points), and an x and y location

## Threads: 
* HumanSnake
	* Makes a new snake, the player controls this snake
* AISnake
	* This makes a new snake, our "AI" algorithm controls the movement of this snake. 

## Methods/functions:
* Main
	* Sets up game
	* Creates board
	* Creates four snakes, one human snake, three AI snakes
	* Joins the threads and closes the world when done
* tick_snake
	* Moves the snake forward one space
	* If the direction changes, change the head direction
	* Checks for collisions with walls and self/others. If there is a collision, update highscore and reset snake
	* If snake eats the apple, add a body part, increase points, give permission for a new apple to appear
* snake_append_body_part
	* Adds a frame to the linked list snake
* snake_new
	* creates a new snake 
	* Calls snake_reset to place snake on board
* snake_reset
	* Centers snake on board
	* Resets points
	* Removes old body parts
	* Adds a single body frame
* tick_world
	* Moves the world forward one step
	* If an apple has been eaten, make a new one and random coordinates 
* world_new
	* Makes a new world
	* Add apples to list
	* Add a new apple to the world
* apple_new
	* Makes a new apple and assigns it a character and a points value
* create_win
	* makes the window based on terminal size
* destroy_win
	* Remove the window
## Sample output: 
In the game below, the user snake (the one with the 's' body) has eaten two apples ('@') and has a score of 20. The other three snakes ('z' body) have different sizes depending on how many apples they have eaten. 

```


     Score: 20							Highscore: 50

                       	@

								<z

			^		
			z		
                        z                                              
 			z		
             <sss	z		


				<zzzzzzzz






     Press Q to quit


