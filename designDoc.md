# SSSSnake
By Vincent Portelli and Michael Smith; For Beta V0


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
* direction
	* This gives direction words a meaning
* Snake
	* This struct is the framework for a snake. Each snake has: 
		* `int chartype;` -> The character that makes up the body. 
  		* `int prev_posX, prev_posY;` -> Previous location of the head
  		* `int posX, posY;` -> Current location of the head
  		* `direction facing_direction;` -> Direction the snake is moving
  		* `uint16_t points;` -> Current points this game
  		* `LinkedList *body_list;` -> The body
* World
	* This struct is the framework for the board that the snakes exist in it. It contains:
		* `LinkedList *apples_list;` -> A list of apples in the world. 
		* `uint8_t max_apples;` -> Total number of apples
		* `LinkedList *snake_list;` -> A linked list of snakes
		* `char *winner;` -> A String that displays the current winner 

* Snake Body Part
	* Each body part has a character and a location (x,y)
* Apple
	* Each apple has a character, a value (in points), and an x and y location

## Threads: 
* HumanSnake
	* Makes a new snake, adds it to the world, and lets the player controls this snake
* AISnake
	* This makes a new snake and adds it to the world our "AI" algorithm controls the movement of this snake. Right now this algorithm is just "Go the apple's x coordinate, then go to its y coordinate". This will more complicated in the final version and may include snake avoidance mechanisms to prevent the AISnakes from crashing into each other.  

## Methods/functions:
* Main
	* Sets up game
	* Creates board
	* Creates four snakes, one human snake, three AI snakes
	* Joins the threads and closes the world when done
* create_win
	* makes the window based on terminal size
* destroy_win
	* Remove the window
* tick_snake
	* Moves the snake forward one space
	* If the direction changes, change the head direction
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
	* If the first element of a snake has:
		* Eaten an apple -> change points, add a body part, and spawn a new apple
		* Hit a wall -> reset snake
	* Check each snake's scores to figure out the winner or if there is a tie
* world_new
	* Makes a new world
	* Add apples to list
	* Add a new apple to the world
* apple_new
	* Makes a new apple and assigns it a character and a points value

## Sample output: 
In the game below, the user snake (the one with the 'z' body) has eaten two apples ('@') and has a score of 20. The other three snakes ('s' body) have different sizes depending on how many apples they have eaten. 

```


     Your Score: 20				Current Winner: AISnake3

                       	@

								<s

			^		
			s		
                        s                                              
 			s		
             <zzz	s		


				<ssssssss






     Press Q to quit


