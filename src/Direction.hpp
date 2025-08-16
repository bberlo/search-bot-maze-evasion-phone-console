//
//  Direction.hpp
//
//  Created by David Stritzl on 08/11/15.
//
//

#ifndef Direction_hpp
#define Direction_hpp

#define N_DIRECTIONS 4

enum Direction {
    DirectionNorth = 0,
    DirectionEast  = 1,
    DirectionSouth = 2,
    DirectionWest  = 3
};

Direction oppositeDirection(Direction);

#endif /* Direction_hpp */
