/*
 * Ship.hpp
 *
 *  Created on: 2012-7-14
 *      Author: mcxiaoke
 */

#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsSprite.hpp"
#include "Types.hpp"

namespace db {

class Ship {
public:
    Ship(demo::Context* context);

    void spawn();
private:
    demo::GraphicsService* mGraphicsService;
    demo::GraphicsSprite* mSprite;
    demo::Location mLocation;
    float mAnimSpeed;
};

}

#endif /* SHIP_HPP_ */
