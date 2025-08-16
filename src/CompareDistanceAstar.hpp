//
//  PriorityQueueExample.cpp
//
//  Created by David Stritzl on 18/11/15.
//
//

// NOTE: this file does NOT compile
#ifndef DistanceCompareAstar_h
#define DistanceCompareAstar_h

#include "PriorityQueue.hpp"
#include "GridElement.hpp"
#include "constants.h"
#include "Direction.hpp"

// calculates the manhattan distance from and arbitrary element to the end
int manhattanDistanceSum(GridElement *element) {
	int xEnd = GRID_SIZE - 1;
	int yEnd = GRID_SIZE - 1;
	int xElement = element->x;
	int yElement = element->y;

	int DistanceXtoEnd = abs(xElement - xEnd);
	int DistanceYtoEnd = abs(yElement - yEnd);

	int manhattanDistanceToEnd = DistanceXtoEnd + DistanceYtoEnd;

	int manhattanDistanceSum = manhattanDistanceToEnd + element->stepCount;

	return manhattanDistanceSum;
}

// distance comparison class for priority queue
class DistanceCompareAstar {
public:

	bool operator()(GridElement *a, GridElement *b) {
		// manhattanDistanceToEnd is a function to calculate the manhattan distance from and arbitrary element to the end
		return manhattanDistanceSum(a) > manhattanDistanceSum(b);
	}
};

#endif