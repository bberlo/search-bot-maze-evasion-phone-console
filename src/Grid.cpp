//
//  Grid.cpp
//
//  Created by David Stritzl on 08/11/15.
//
//
#define _USE_MATH_DEFINES

#include "Grid.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>

#include "Direction.hpp"
#include "PriorityQueue.hpp"
#include "CompareDistance.hpp"
#include "CompareDistanceAstar.hpp"

Grid::Grid() {
	// set positions and neighbours of all elements
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].x = x;
			grid[x][y].y = y;

			if (x > 0) grid[x][y].neighbours[DirectionWest] = &grid[x - 1][y];
			if (x < GRID_SIZE - 1) grid[x][y].neighbours[DirectionEast] = &grid[x + 1][y];
			if (y > 0) grid[x][y].neighbours[DirectionNorth] = &grid[x][y - 1];
			if (y < GRID_SIZE - 1) grid[x][y].neighbours[DirectionSouth] = &grid[x][y + 1];
		}
	}
}

void Grid::reset() {
	// reset all elements
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].reset();
		}
	}
}

void Grid::startAgain() {
	// reset all elements
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			a = GRID_SIZE / 2;
			b = GRID_SIZE / 2;
			breadthFirstxPos = GRID_SIZE - 1;
			breadthFirstyPos = GRID_SIZE - 1;
			greedyxPos = GRID_SIZE /2;
			greedyyPos = 0;
			aStarxPos = 0;
			aStaryPos = GRID_SIZE - 1;
			grid[x][y].APos = false;
			grid[x][y].GPos = false;
			grid[x][y].BPos = false;
			grid[x][y].playerPos = false;
			grid[a][b].playerPos = true;
		}
	}
}

void Grid::draw() {
	// draw all elements
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].draw();
		}
	}
}

void Grid::clearRect(int first_x, int first_y, int width, int height) {
	int last_x = first_x + width - 1;
	int last_y = first_y + height - 1;

	for (int y = first_y; y <= last_y; y++) {
		for (int x = first_x; x <= last_x; x++) {
			if (x != first_x) {
				grid[x][y].walls[DirectionWest] = false;
			}
			if (x != last_x) {
				grid[x][y].walls[DirectionEast] = false;
			}
			if (y != first_y) {
				grid[x][y].walls[DirectionNorth] = false;
			}
			if (y != last_y) {
				grid[x][y].walls[DirectionSouth] = false;
			}
		}
	}
}

void Grid::drawArc(int x_center, int y_center, double radius, double first_alpha, double last_alpha) {
	assert(first_alpha >= 0.0 && first_alpha <= 2 * M_PI);
	assert(last_alpha >= 0.0 && last_alpha <= 2 * M_PI);

	for (int y_relative = -radius; y_relative <= radius; y_relative++) {
		int y = y_center + y_relative;

		// stop if outside of grid
		if (y < 0 || y >= GRID_SIZE) {
			continue;
		}

		for (int x_relative = -radius; x_relative <= radius; x_relative++) {
			int x = x_center + x_relative;

			// stop if outside of grid
			if (x < 0 || x >= GRID_SIZE) {
				continue;
			}

			double alpha = std::atan2(y_relative, x_relative);
			if (alpha < 0) {
				alpha += 2 * M_PI;
			}

			// stop if outside of arc angle range
			if (last_alpha >= first_alpha) {
				if (alpha < first_alpha || alpha > last_alpha) {
					continue;
				}
			}
			else {
				if (alpha < first_alpha && alpha > last_alpha) {
					continue;
				}
			}

			// stop if outside of arc radius
			double distance = std::sqrt(std::pow(x_relative, 2) + std::pow(y_relative, 2));
			if (distance >= radius) {
				continue;
			}

			for (int direction = 0; direction < N_DIRECTIONS; direction++) {
				GridElement *neighbour = grid[x][y].neighbours[direction];
				if (neighbour == NULL) {
					continue;
				}

				int x_neighbour_relative = neighbour->x - x_center;
				int y_neighbour_relative = neighbour->y - y_center;
				double neighbour_distance = std::sqrt(std::pow(x_neighbour_relative, 2) + std::pow(y_neighbour_relative, 2));

				// mark wall if neighbour is outside of arc radius
				if (neighbour_distance >= radius) {
					grid[x][y].setWall((Direction)direction, true);
				}
			}
		}
	}
}

/*** Maze Generator ***/

void Grid::generateMaze() {
	std::stack<GridElement *> stack;

	// push first element (0, 0) on stack
	stack.push(&grid[0][0]);

	while (!stack.empty()) {
		// get next element
		GridElement *current_element = stack.top();

		current_element->visited = true;

		// find all unvisited directions
		int n_unvisited_directions = 0;
		Direction unvisited_directions[N_DIRECTIONS];
		for (int direction = 0; direction < N_DIRECTIONS; direction++) {
			if (current_element->neighbours[direction] != NULL
				&& !current_element->neighbours[direction]->visited) {
				unvisited_directions[n_unvisited_directions++] = (Direction)direction;
			}
		}

		// backtrack if no possible directions are found
		if (n_unvisited_directions == 0) {
			stack.pop();
			continue;
		}

		// choose random direction from unvisited directions
		Direction next_direction = unvisited_directions[std::rand() % n_unvisited_directions];

		// remove wall in this direction
		current_element->setWall(next_direction, false);

		// push next element on stack
		GridElement *next_element = current_element->neighbours[next_direction];
		stack.push(next_element);
	}

	{ // randomly remove some walls
		int n_walls_removed = 0;
		while (n_walls_removed < MAZE_DIFFICULTY) {
			// choose random element and direction
			int x = std::rand() % GRID_SIZE;
			int y = std::rand() % GRID_SIZE;

			GridElement *current_element = &grid[x][y];

			Direction direction = (Direction)(std::rand() % N_DIRECTIONS);

			// remove wall if possible
			if (current_element->neighbours[direction] != NULL && current_element->walls[direction]) {
				current_element->setWall(direction, false);
				n_walls_removed++;
			}
		}
	}

	{ // reset visited flags
		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				grid[x][y].visited = false;
				grid[x][y].marked = false;
				grid[x][y].markedB = false;
				grid[x][y].BPos = false;
				grid[x][y].markedG = false;
				grid[x][y].GPos = false;
				grid[x][y].markedA = false;
				grid[x][y].APos = false;
				grid[a][b].playerPos = true;

			}
		}
	}
}

/*** Room Generator ***/

#define N_ROOMS 5

void Grid::generateRooms() {
	double room_width = (double)GRID_SIZE / N_ROOMS;
	int room_height = GRID_SIZE / 3;

	for (int i = 0; i < N_ROOMS; i++) {
		int first_x = std::floor(i * room_width);
		int last_x = std::floor((i + 1) * room_width) - 1;
		int width = last_x - first_x + 1;

		double door_center_x = (first_x + last_x) / 2.0;
		int door_x = std::floor(door_center_x);
		int door_width = std::ceil(door_center_x + 1) - door_x;

		// clear room and door
		clearRect(first_x, 0, width, room_height);
		clearRect(door_x, room_height - 1, door_width, 2);

		// repeat for bottom
		clearRect(first_x, GRID_SIZE - room_height, width, room_height);
		clearRect(door_x, GRID_SIZE - room_height - 1, door_width, 2);
	}

	// clear center area
	clearRect(0, room_height, GRID_SIZE, GRID_SIZE - 2 * room_height);
}

/*** Obstacle Generator ***/

#define N_OBSTACLES 5
#define MIN_OBSTACLE_RADIUS 2
#define MAX_OBSTACLE_RADIUS 10

void Grid::generateObstacles() {
	// NOTE: this algorithm is not deterministic

	clearRect(0, 0, GRID_SIZE, GRID_SIZE);

	// helper structure to save obstacles in a vector
	struct Obstacle {
		int center_x, center_y;
		int radius;
	};

	std::vector<Obstacle> obstacles;

	while (obstacles.size() < N_OBSTACLES) {
		int center_x = std::rand() % GRID_SIZE;
		int center_y = std::rand() % GRID_SIZE;

		int radius = std::rand() % (MAX_OBSTACLE_RADIUS - MIN_OBSTACLE_RADIUS) + MIN_OBSTACLE_RADIUS;

		bool too_close = false;
		for (Obstacle obstacle : obstacles) {
			int distance = (int)std::sqrt(std::pow(obstacle.center_x - center_x, 2) + std::pow(obstacle.center_y - center_y, 2));
			if (distance < obstacle.radius + radius) {
				too_close = true;
				break;
			}
		}

		if (too_close) {
			continue;
		}

		double first_alpha = std::rand() % (int)(2 * M_PI * 1000) / 1000.0;
		double last_alpha = first_alpha + std::rand() % (int)(M_PI * 1000) / 1000.0 + M_PI;
		if (last_alpha > 2 * M_PI) {
			last_alpha -= 2 * M_PI;
		}

		drawArc(center_x, center_y, radius, first_alpha, last_alpha);
		obstacles.push_back({ center_x, center_y, radius });
	}
}

/*
Authors of the next section: Bram van Berlo, s1538233 and Calvin Kho, s1601288
The following functions: depthFirstSearch and BreadthFirstSearch have been constructed with help of the code above written by David Stritzl.
The functions generate a path through a maze with the BFS and DFS algorithm.
*/


// Function for moving the player 'Up'. 
void Grid::playerU() {

	GridElement *current_element = &grid[a][b];			//every time the function is run, it checks the values for its updated x and y values.		

	if (current_element->walls[0] == false) {		//checks for walls 'North' of the current node.
		current_element->playerPos = false;			//clears the previous node
		b--;										// changes the coordinate so that player moves UP one space
		current_element = &grid[a][b];				// set current element
		current_element->playerPos = true;			// show player position
	}

}

//Function for moving the player 'Down'. Code is practically the same but adjusted to change the coordinates accordingly.
void Grid::playerD() {

	GridElement *current_element = &grid[a][b];

	if (current_element->walls[2] == false) {
		current_element->playerPos = false;
		b++;
		current_element = &grid[a][b];
		current_element->playerPos = true;
	}
}

//Function for moving the player 'Left'. Code is practically the same but adjusted to change the coordinates accordingly.
void Grid::playerL() {

	GridElement *current_element = &grid[a][b];

	if (current_element->walls[3] == false) {
		current_element->playerPos = false;
		a--;
		current_element = &grid[a][b];
		current_element->playerPos = true;
	}
}

//Function for moving the player 'Right'. Code is practically the same but adjusted to change the coordinates accordingly.
void Grid::playerR() {

	GridElement *current_element = &grid[a][b];

	if (current_element->walls[1] == false) {
		current_element->playerPos = false;
		a++;
		current_element = &grid[a][b];
		current_element->playerPos = true;
	}
}

// Function to move the breadth first search bot. 
void Grid::breadthFirstBot() {

	int direction = breadthFirstSearch(breadthFirstxPos, breadthFirstyPos); //Sets the coordinates for the breadth first search bot.

	if (direction == 0) {			//if direction value is 0 (North), move the bot up.
		breadthFirstyPos--;
	}
	if (direction == 1) {			//if direction value is 1 (East), move the bot right.
		breadthFirstxPos++;
	}
	if (direction == 2) {			//if direction value is 2 (South), move the bot down.
		breadthFirstyPos++;
	}
	if (direction == 3) {			//if direction value is 3 (West), move the bot left.
		breadthFirstxPos--;
	}

}

int Grid::breadthFirstSearch(int xPos, int yPos) {

	//Reset of the visitid and marked booleans to false
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].visited = false;
			grid[x][y].markedB = false;
		}
	}

	//Initialisation of the queue
	std::queue<GridElement *> BFSqueue;

	// push first element on queue
	//Spawn blue bot on the initial state, mark it visited and pop the queue afterwards
	BFSqueue.push(&grid[breadthFirstxPos][breadthFirstyPos]);
	GridElement *current_element = &grid[breadthFirstxPos][breadthFirstyPos];
	current_element->visited = true;

	//While the end condition has not been reached, keep looking for possible directions
	while (!grid[a][b].visited) {

		//Pop the first element of the queue
		current_element = BFSqueue.front();
		BFSqueue.pop();

		// find all unvisited directions related to the first element of the queue
		int n_unvisited_directions = 0;
		Direction unvisited_directions[N_DIRECTIONS];
		for (int direction = 0; direction < N_DIRECTIONS; direction++) {
			if (current_element->walls[direction] == false
				&& !current_element->neighbours[direction]->visited) {
				unvisited_directions[n_unvisited_directions++] = (Direction)direction;
			}
		}

		//Put all the directions related to the front element in the queue
		for (int k = 0; k < n_unvisited_directions; k++) {
			Direction next_direction = unvisited_directions[k];
			GridElement *next_element = current_element->neighbours[next_direction];
			//Push all the elements found in the for loop in the queue
			next_element->parent = current_element;
			BFSqueue.push(next_element);
			//Mark all the elements in the stack to visited
			next_element->visited = true;
		}
	}

	/*
	When the end condition has been found,
	backtrack the path from the end condition to the intial condition and move the bot to the next element.
	*/

	// This part sends the direction in which the bot should move according to the breadth first search algorithm.
	while (!grid[breadthFirstxPos][breadthFirstyPos].markedB) {

		current_element->parent->markedB = true;				//sets the current path for the breadth first search bot
		current_element->parent->child = current_element;
		current_element = current_element->parent;
	}

	current_element->BPos = false;								//removes the previous position for the breadth first search bot
	current_element->child->BPos = true;						//adds the new position for the bot

	current_element = current_element->child;

	//compares the previous element with the next element and returns the direction it has moved accordingly.
	if (current_element->x > current_element->parent->x) {
		return 1;
	}
	if (current_element->x < current_element->parent->x) {
		return 3;
	}
	if (current_element->y > current_element->parent->y) {
		return 2;
	}
	if (current_element->y < current_element->parent->y) {
		return 0;
	}
}


/*** Greedy Search ***/

// score comparison class for inserting elements into the right place inside the priority queue
class ScoreCompare {
public:
	bool operator()(GridElement *a, GridElement *b) {
		return a->score > b->score;
	}
};

/*
Authors of the next section: Bram van Berlo, s1538233 and Calvin Kho, s1601288
The following functions: greedySearch and aStarSearch have been constructed with help of the code above written by David Stritzl.
The functions generate a path through a maze with the Greedy search and A* search algorithm.
*/

// Function to move the breadth first search bot. Much like the previous bot, it changes coordinates depending on what value was returned from the greedy search algorithm.
void Grid::greedyBot() {

	int direction = greedySearch(greedyxPos, greedyyPos);

	if (direction == 0) {
		greedyyPos--;
	}
	if (direction == 1) {
		greedyxPos++;
	}
	if (direction == 2) {
		greedyyPos++;
	}
	if (direction == 3) {
		greedyxPos--;
	}

}

int Grid::greedySearch(int xPos, int yPos) {

	//Reset of the visitid and marked booleans to false
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].visited = false;
			grid[x][y].markedG = false;
		}
	}

	/*Include a custom made set called the "priorityQueue". This set has a class connected to it which will calculate the manhattan distance compared to the end node.
	The set will use this function in order to sort all the elements based on the lowest result of the manhattan distance function.*/
	PriorityQueue<GridElement *, DistanceCompare> GreedyQueue;

	// push first element on queue, mark it visited and spawn yellow bot on the initial state
	GreedyQueue.push(&grid[greedyxPos][greedyyPos]);
	GridElement *current_element = &grid[greedyxPos][greedyyPos];
	current_element->visited = true;

	//Execute this while loop until the end condition is reached
	while (!grid[a][b].visited) {

		//First set the current element to top element of the custom queue and afterwards pop the first element of the queue
		current_element = GreedyQueue.top();
		GreedyQueue.pop();

		/* find all possible unvisited directions related to the first element in the custom set,
		after that put all the directions related to the front element in the queue (the same as breadthFirstSearch) */
		int n_unvisited_directions = 0;
		Direction unvisited_directions[N_DIRECTIONS];
		for (int direction = 0; direction < N_DIRECTIONS; direction++) {
			if (!current_element->walls[direction]
				&& !current_element->neighbours[direction]->visited) {
				unvisited_directions[n_unvisited_directions++] = (Direction)direction;
				GridElement *next_element = current_element->neighbours[direction];
				//Push all the elements found in the for loop in the queue
				next_element->parent = current_element;
				GreedyQueue.push(next_element);
				//Mark all the elements in the stack to visited
				next_element->visited = true;
			}
		}
	}

	/*While the initial condition has not been reached yet, keep backtracking and marking every parent of a certain layer,
	in the end this while loop will create a path from the intial to the end condition*/
	while (!grid[greedyxPos][greedyyPos].markedG) {
		current_element->parent->markedG = true;
		current_element->parent->child = current_element;
		current_element = current_element->parent;
	}
	//similar to the previous algorithm, this part returns the direction depending on where the next node is.
	current_element->GPos = false;
	current_element->child->GPos = true;

	current_element = current_element->child;

	if (current_element->x > current_element->parent->x) {
		return 1;
	}
	if (current_element->x < current_element->parent->x) {
		return 3;
	}
	if (current_element->y > current_element->parent->y) {
		return 2;
	}
	if (current_element->y < current_element->parent->y) {
		return 0;
	}

}


// Function to move the breadth aStar search bot. Much like the previous bots, it changes coordinates depending on what value was returned from the greedy search algorithm.
void Grid::aStarBot() {

	int direction = aStarSearch(aStarxPos, aStaryPos);

	if (direction == 0) {
		aStaryPos--;
	}
	if (direction == 1) {
		aStarxPos++;
	}
	if (direction == 2) {
		aStaryPos++;
	}
	if (direction == 3) {
		aStarxPos--;
	}

}

int Grid::aStarSearch(int xPos, int yPos) {

	//Reset of the visitid and marked booleans to false
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			grid[x][y].visited = false;
			grid[x][y].markedA = false;
			grid[x][y].stepCount = -1;
		}
	}

	/*Include a custom made set called the "priorityQueueAstar". This set has a class connected to it which will calculate the manhattan distance compared to the end node,
	but also */
	PriorityQueue<GridElement *, DistanceCompareAstar> aStarQueue;

	// push first element on queue
	//Draw green bot on the initial state, mark it visited and pop the queue afterwards
	aStarQueue.push(&grid[aStarxPos][aStaryPos]);
	GridElement *current_element = &grid[aStarxPos][aStaryPos];
	current_element->visited = true;

	while (!grid[a][b].visited) {

		//Pop the first element of the queue and set the current element to top element of the custom queue
		current_element = aStarQueue.top();
		current_element->visited = true;
		aStarQueue.pop();

		// find all unvisited directions related to the first element of the custom queue and put all the directions related to the front element in the queue
		int n_unvisited_directions = 0;
		Direction unvisited_directions[N_DIRECTIONS];
		for (int direction = 0; direction < N_DIRECTIONS; direction++) {
			if (!current_element->walls[direction]
				&& !current_element->neighbours[direction]->visited) {
				unvisited_directions[n_unvisited_directions++] = (Direction)direction;
				GridElement *next_element = current_element->neighbours[direction];
				/*Included a step counter to count the steps taken to reach the current position. If a parent has been previously visited, check if the route is shorter.
				If it is replace the parent with new value and push on queue*/
				if (next_element->stepCount >= 0) {
					if (next_element->stepCount < current_element->stepCount + 1) {
						aStarQueue.erase(next_element);
						next_element->parent = current_element;
						next_element->stepCount = current_element->stepCount + 1;
						aStarQueue.push(next_element);
					}
				}
				else {
					next_element->parent = current_element;
					next_element->stepCount = current_element->stepCount + 1;
					aStarQueue.push(next_element);
				}
			}
		}
	}


	/*While the initial condition has not been reached yet, keep backtracking and marking every parent of a certain layer,
	in the end this while loop will draw a path from the intial to the end condition*/
	while (!grid[aStarxPos][aStaryPos].markedA) {
		current_element->parent->markedA = true;
		current_element->parent->child = current_element;
		current_element = current_element->parent;
	}

	//similar to the previous algorithm, this part returns the direction depending on where the next node is.
	current_element->APos = false;
	current_element->child->APos = true;

	current_element = current_element->child;

	if (current_element->x > current_element->parent->x) {
		return 1;
	}
	if (current_element->x < current_element->parent->x) {
		return 3;
	}
	if (current_element->y > current_element->parent->y) {
		return 2;
	}
	if (current_element->y < current_element->parent->y) {
		return 0;
	}
}
