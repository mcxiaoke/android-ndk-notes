#include "Asteroid.hpp"
#include "Log.hpp"

namespace dbs {
    Asteroid::Asteroid(packt::Context* pContext) :
        mTimeService(pContext->mTimeService),
        mGraphicsService(pContext->mGraphicsService),
        mLocation(), mSpeed(0.0f) {
        mSprite = pContext->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture(
                    "/sdcard/droidblaster/asteroid.png"),
            64, 64, &mLocation);
    }

    void Asteroid::spawn() {
        const float MIN_SPEED = 4.0f;
        const float MIN_ANIM_SPEED = 8.0f, ANIM_SPEED_RANGE = 16.0f;

        mSpeed = -RAND(mGraphicsService->getHeight()) - MIN_SPEED;
        float lPosX = RAND(mGraphicsService->getWidth());
        float lPosY = RAND(mGraphicsService->getHeight())
                      + mGraphicsService->getHeight();
        mLocation.setPosition(lPosX, lPosY);

        float lAnimSpeed = MIN_ANIM_SPEED + RAND(ANIM_SPEED_RANGE);
        mSprite->setAnimation(8, -1, lAnimSpeed, true);
    }

    void Asteroid::update() {
        mLocation.translate(0.0f, mTimeService->elapsed() * mSpeed);
        if (mLocation.mPosY <= 0) {
            spawn();
        }
    }
}
