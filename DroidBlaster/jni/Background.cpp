#include "Background.hpp"
#include "Log.hpp"

namespace dbs {
    Background::Background(packt::Context* pContext) :
      mTimeService(pContext->mTimeService),
      mGraphicsService(pContext->mGraphicsService),
      mLocation(), mAnimSpeed(8.0f) {
        mTileMap = mGraphicsService->registerTileMap("tilemap.tmx",
            mGraphicsService->registerTexture("tilemap.png"),
            &mLocation);
    }

    void Background::spawn() {
        mLocation.setPosition(0.0f, 0.0f);
    }

    void Background::update() {
        const float SCROLL_PER_SEC = -64.0f;
        float lScrolling = mTimeService->elapsed() * SCROLL_PER_SEC;
        mLocation.translate(0.0f, lScrolling);
    }
}
