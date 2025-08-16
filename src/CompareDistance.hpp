 //
//  PriorityQueueExample.cpp
//
//  Created by David Stritzl on 18/11/15.
//
//

// NOTE: this file does NOT compile
#ifndef DistanceCompare_h
#define DistanceCompare_h

#include "PriorityQueue.hpp"
#include "GridElement.hpp"
#include "constants.h"
#include "Direction.hpp"

// calculates the manhattan distance from and arbitrary element to the end
int manhattanDistanceToEnd(GridElement *element) {
	int xEnd = GRID_SIZE - 1;
	int yEnd = GRID_SIZE - 1;
	int xElement = element->x;
	int yElement = element->y;
	int DistanceX = abs(xElement - xEnd);
	int DistanceY = abs(yElement - yEnd);
	int manhattanDistance = DistanceX + DistanceY;
	return manhattanDistance;
}

// distance comparison class for priority queue
class DistanceCompare {
public:

	bool operator()(GridElement *a, GridElement *b) {
		// manhattanDistanceToEnd is a function to calculate the manhattan distance from and arbitrary element to the end
		return manhattanDistanceToEnd(a) > manhattanDistanceToEnd(b);
	}
};

#endif