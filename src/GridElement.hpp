//
//  GridElement.hpp
//
//  Created by David Stritzl on 08/11/15.
//
//

#ifndef GridElement_hpp
#define GridElement_hpp

#include "Direction.hpp"

class GridElement {
public:
    GridElement();
    
    void reset();
    
    void draw();
    void drawWall(Direction);
    
    void setWall(Direction, bool);
    
    int x, y;
	int stepCount = -1;
    
    GridElement *neighbours[N_DIRECTIONS];
    bool walls[N_DIRECTIONS];
    
    // variables for maze generation and path finding
	bool visited, marked, markedB, markedG, markedA, playerPos, APos, GPos, BPos; 
    
    GridElement *parent, *child;
    int length_of_path;
    int score;
};

#endif /* GridElement_hpp */
