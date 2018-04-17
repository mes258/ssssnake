# ssssnake
One "s" per snake

Simple snake AI from https://gamedev.stackexchange.com/questions/33865/designing-a-simple-snake-a-i

New piece of food shows up. Snake figures if it needs to go + or - X, and the same for Y. On the first turn snake starts heading to the food's X (by moving along the Y axis).

If the snake encounters itself while moving Y, it makes a turn in the correct X direction (it remembered whether it needs to go +X or -X), and heads straight until it reaches a square and doesn't detect its tail to the side anymore. If it detects more body ahead, it turns to the free direction, continuing until it detects an opening to the side and can return back to its course. (It "wall hugs" its tail until the tail runs out and it can get back on course.)

Once the snake reaches the correct X coordinate, it begins moving along the X to get to the correct Y coordinate.

If the snake encounters itself while moving X, it makes a turn toward the Y direction it first remembered for this piece, and checks for open space similarly to the Y movement.

Now the snake will only suicide if it runs into its tail and turns in the same direction the tail is "moving" (causing the snake to spiral until it runs out of space). This however will never happen if the snake makes sure to turn toward the direction it originally needed to, to get the current piece of food. Why? Because of where the previous piece of food was - the snake will only run into its tail if it has to double-back along the X or the Y to get the piece, so you can usually be assured the snake was previously moving in the opposite X or Y to get to the piece you just picked up.

I believe this will break down if the snake has to double back for multiple pieces in a row, and the distance between all the multiple pieces happens to be shorter than the total length of the snake. But hey, you said "simple" and "relatively" well.