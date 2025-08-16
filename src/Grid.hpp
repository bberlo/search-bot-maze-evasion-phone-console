//
//  Grid.hpp
//
//  Created by David Stritzl on 08/11/15.
//
//

#ifndef Grid_hpp
#define Grid_hpp

#include "GridElement.hpp"
#include "constants.h"

class Grid {
public:
    Grid();
    
    void reset();
	void startAgain();
    
    void draw();
    
    void clearRect(int, int, int, int);
    void drawArc(int, int, double, double, double);
        
    void generateMaze();
    void generateRooms();
    void generateObstacles();
    
    int breadthFirstSearch(int, int);
	int breadthFirstxPos = GRID_SIZE - 1, breadthFirstyPos = GRID_SIZE - 1;
    int greedySearch(int, int);
	int greedyxPos = GRID_SIZE /2, greedyyPos = 0;
    int aStarSearch(int, int);
	int aStarxPos = 0, aStaryPos = GRID_SIZE - 1;
	int a = GRID_SIZE /2;
	int b = GRID_SIZE /2;

	void breadthFirstBot();
	void greedyBot();
	void aStarBot();
	void playerU();
	void playerD();
	void playerL();
	void playerR();
    
    GridElement grid[GRID_SIZE][GRID_SIZE];

	int visitCount, markedCount;
};

#endif /* Grid_hpp */
