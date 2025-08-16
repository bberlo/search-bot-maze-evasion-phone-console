//
//  Direction.cpp
//
//  Created by David Stritzl on 08/11/15.
//
//

#include "Direction.hpp"

Direction oppositeDirection(Direction direction) {
    switch (direction) {
        case DirectionNorth:
            return DirectionSouth;
        case DirectionEast:
            return DirectionWest;
        case DirectionSouth:
            return DirectionNorth;
        case DirectionWest:
            return DirectionEast;
    }
}
