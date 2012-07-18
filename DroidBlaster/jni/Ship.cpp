#include "Ship.hpp"
#include "Log.hpp"

namespace dbs {
    Ship::Ship(packt::Context* pContext) :
      mInputService(pContext->mInputService),
      mGraphicsService(pContext->mGraphicsService),
      mTimeService(pContext->mTimeService),
      mLocation(), mAnimSpeed(8.0f) {
        mSprite = pContext->mGraphicsService->registerSprite(
            mGraphicsService->registerTexture("ship.png"), 64, 64,
            &mLocation);
        mInputService->setRefPoint(&mLocation);
    }

    void Ship::spawn() {
        const int32_t FRAME_1 = 0; const int32_t FRAME_NB = 8;
        mSprite->setAnimation(FRAME_1, FRAME_NB, mAnimSpeed, true);
        mLocation.setPosition(mGraphicsService->getWidth() * 1 / 2,
                              mGraphicsService->getHeight() * 1 / 4);
    }

    void Ship::update() {
        const float SPEED_PERSEC = 400.0f;
        float lSpeed = SPEED_PERSEC * mTimeService->elapsed();

        // Moves the ship.
        mLocation.translate(mInputService->getHorizontal() * lSpeed,
                            mInputService->getVertical()   * lSpeed);
    }
}
