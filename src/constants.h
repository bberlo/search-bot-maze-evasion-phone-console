//
//  constants.h
//
//  Created by David Stritzl on 08/11/15.
//
//

#ifndef config_h
#define config_h

#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       800

#define GRID_SIZE           40

#define GRID_ELEMENT_WIDTH  ((float)WINDOW_WIDTH  / GRID_SIZE)
#define GRID_ELEMENT_HEIGHT ((float)WINDOW_HEIGHT / GRID_SIZE)

//The lower the numbers, the more difficult the game becomes
//MAZE_DIFFICULTY will add more walls to the maze, and BOT_SPEED will make the search bots move faster
#define MAZE_DIFFICULTY		500
#define BOT_SPEED			(float) 500

#endif /* config_h */
