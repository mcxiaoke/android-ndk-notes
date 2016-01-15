/*
* @Author: mcxiaoke
* @Date:   2016-01-15 23:14:07
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 23:17:37
*/

#include "ship.h"
#include "log.h"
#include "types.h"

static const float INITAL_X = 0.5f;
static const float INITAL_Y = 0.25f;

Ship::Ship(android_app* app, GraphicsManager& gm):
    mGraphicsManager(gm), mGraphics(NULL) {

}

void Ship::registerShip(GraphicsElement* graphics) {
    mGraphics = graphics;
}

void Ship::initialize() {
    mGraphics->location.x = INITAL_X * mGraphicsManager.getRenderWidth();
    mGraphics->location.y = INITAL_Y * mGraphicsManager.getRenderHeight();
}
