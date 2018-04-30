# SSSSnake
By Vincent Portelli and Michael Smith; For Final Version


##Summary 
This game uses pipes and forks to parallelize each snake looking for the closest apple. Every frame, each snake uses a breadth-first search algorithm to find the closet apple and move towards that apple. Snakes will die when they collide with another snake or trap themselves in their own tail. The final of this version is slightly different from the proposed and the and the beta version of this game. Instead of a controlling a snake, we shifted to letting the user control the overall game. Additionally we moved away from linked-list structures and used arrays as this gave us more flexibility with our algorithm and overall structure of the game.

## Files: 
* `snake.c`
	* This is the main file. It creates the board, the user snake and all the AI snakes. Run this file to play the game.
* `makefile`
	* Compiles `snake.c` into `snake`
* `splitWorkFinal.md`
	* This file describes how we split the work for the final version.
* `designDocFinal.md`
	* This file.

## Global Variables
* `int SIZE = 60;`
	* The length of one side of the board in chars
* `int NUMSNAKES = 3;`
	* How many snakes we be on the board (this may change)
* `int NUMAPPS = 2;  `
	* How many apples are displayed at once on the board (this may change)
* `int GROW = 3;`
	* How many spots will be added to the snake for every apple eaten
* `int DELAY = 100000; `
	* Time in microseconds to wait between updates

* `char S = 'X';     `
	* Internal value for a snake body part
* `char A = '@';     `
	* Internal value for an apple 
* `char EMPTY = '.'; `
	* Internal value for an empt spot

* `char world[60][60];`
	* The world array
* `int snakes[100][60*60][2];`
	* An array for all the snakes
* `int directions[100];`
	* An array to hold snake directions
                        
* `int apples[100][2];   `
	* Array to hold apples
* `bool kill = false;`
	* True if the snakes will die after the collide or false if they respawn

## Methods/functions:
* `main()`
	* Gets user input for:
		* Number of snakes on the board
		* Speed of snake growth
		* Number of apples on the board
		* Number of frames
		* Kill vs respawn snakes
	* Populate the apple array
	* Populate the snake array
	* Draw the world
	* Places apples
	* Places snakes
	* Print the world
	* Loop through the number of frames times 300, moving the game forward each frame. 

* `snake_grow(int n)`
	* Grows snake `n` by the correct number of blocks
* `draw_world()`
	* Populates the `world[][]` array with the necessary characters 
* `reset_apple(int n)`
	* Gives apple `n` random, non-populated coordinates
	* Add the apple to the array of apples
* `reset_snake(int n)`
	* Gives snake `n` random, non-populated coordinates
	* Add the snake to the array of snakes
* `kill_snake(int n)`
	* Take the snake off the board
* `check_snakes()`
	* Check if the head of any snake is at the same spot as a wall 
	* Check if the head of any snake is at the same spot as another body part of any snake
	* Check if a snake ate an apple
	* Kill or reset the snakes a needed
* `print_world()`
	* Print out each element in the `world[][]` array
* `snake_move_forward(int n)`
	* Move snake `n` forward one spot depending on its direction
* `get_next_move(int n)`
	* Gets the next move for snake `n`
	* Completes a breadth-first search for an apple
	* Once it finds the closest apple or a dead end, return the next step
* `determine_directions()`
	* Creates a pipe to communicate with the child process it creates
	* The child process gets the next move with `get_next_move(int n)`
	* The child writes the direction to the pipe
	* The parent process reads the direction from the pipe and adds it to the `directions[]` array
* `move_snakes`
	* Determine the snake direction
	* Move each snake forward one space

## Sample output: 

* The below game is a game with 5 snakes, 5 apples, and a growth of 5. 

```
╔════════════════════════════════════════════════════════════════════════════════╗
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                                                ║
║                                                ██                              ║
║                                    🍎          ██                              ║
║                                                ██                              ║
║                                                ██                              ║
║                                                ██                              ║
║                                                ██                              ║
║                                                ██                              ║
║                ████                            ██                              ║
║                ██                        ████████                              ║
║                ██                        ██                                    ║
║                ██                        ██                                    ║
║                ██                        ██                                    ║
║                ██                  ░░    ██                                    ║
║                ██                  ██    ██                                    ║
║                ██                  ██    ██                                    ║
║        ██████████                  ██    ██              🍎                    ║
║        ██    ████                  ██    ██                                    ║
║        ██                        ░░██    ██                                    ║
║    ░░🍎░░          ████████████  ██      ██    ████                            ║
║                    ██            ██      ██████████                            ║
║                    ██            ████████████    ██                            ║
║                    ██                      ██    ██                      ████  ║
║                    ██                      ██    ██████                  ████  ║
║                    ██                      ██        ██                  ████  ║
║                    ██████████████████████████        ████████████████████████  ║
║                          🍎🍎                              ░░██████████    ██  ║
║                                                                      ██    ██  ║
║                                                                      ████████  ║
║                                                                                ║
║                                                                                ║
╚════════════════════════════════════════════════════════════════════════════════╝
