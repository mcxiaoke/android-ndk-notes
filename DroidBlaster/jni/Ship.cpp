/*
 * Ship.cpp
 *
 *  Created on: 2012-7-14
 *      Author: mcxiaoke
 */

#include "Ship.hpp"
#include "Log.hpp"

namespace db {

Ship::Ship(demo::Context* context) :
        mGraphicsService(context->mGraphicsService), mLocation(), mAnimSpeed(
                8.0f) {

    mSprite = context->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture("ship.png"), 64, 64, &mLocation);

}

void Ship::spawn() {
    demo::Log::info("Ship::spawn()");
    const int32_t FRAME_1 = 0;
    const int32_t FRAME_NB = 8;
    mSprite->setAnimation(FRAME_1, FRAME_NB, mAnimSpeed, true);
    mLocation.setPosition(mGraphicsService->getWidth() * 1 / 2,
            mGraphicsService->getHeight() * 1 / 4);
}

}

