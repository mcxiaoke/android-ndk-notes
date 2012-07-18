/*
 * DroidBlaster.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "DroidBlaster.hpp"
#include "Log.hpp"

#include <unistd.h>
#include <math.h>

namespace db {

DroidBlaster::DroidBlaster(demo::Context *context) :
        mGraphicsService(context->mGraphicsService), mSoundService(
                context->mSoundService), mTimeService(context->mTimeService), mShip(
                context), mBackground(context) {
    demo::Log::debug("Creating DroidBlaster.");
}

DroidBlaster::~DroidBlaster() {
    demo::Log::debug("Destructing DroidBlaster.");
}

demo::status DroidBlaster::onActivate() {
    demo::Log::debug("Activating DroidBlaster.");
    if (mGraphicsService->start() != demo::STATUS_OK) {
        return demo::STATUS_KO;
    }

    if (mSoundService->start() != demo::STATUS_OK) {
        return demo::STATUS_KO;
    }
    mSoundService->playBGM("bgm.mp3");

    mBackground.spawn();
    mShip.spawn();
    mTimeService->reset();

    return demo::STATUS_OK;
}

void DroidBlaster::onDeactivate() {
    demo::Log::debug("Deactivating DroidBlaster.");
    mGraphicsService->stop();
    mSoundService->stop();
}

demo::status DroidBlaster::onStep() {
//    demo::Log::debug("Starting step.");
    mTimeService->update();
    mBackground.update();
    if (mGraphicsService->update() != demo::STATUS_OK) {
        demo::Log::debug("update graphics service failed.");
        return demo::STATUS_KO;
    }
//    demo::Log::debug("update graphics service.");
//    demo::Log::debug("Steping done..");
    return demo::STATUS_OK;
}

void DroidBlaster::onStart() {
    demo::Log::debug("onStart.");
}

void DroidBlaster::onResume() {
    demo::Log::debug("onResume.");
}
void DroidBlaster::onPause() {
    demo::Log::debug("onPause.");
}
void DroidBlaster::onStop() {
    demo::Log::debug("onStop.");
}
void DroidBlaster::onDestroy() {
    demo::Log::debug("onDestroy.");
}

void DroidBlaster::onSaveState(void** data, size_t* size) {
    demo::Log::debug("onSaveState.");
}
void DroidBlaster::onConfigurationChanged() {
    demo::Log::debug("onConfigurationChanged.");
}
void DroidBlaster::onLowMemory() {
    demo::Log::debug("onLowMemory.");
}

void DroidBlaster::onCreateWindow() {
    demo::Log::debug("onCreateWindow.");
}
void DroidBlaster::onDestroyWindow() {
    demo::Log::debug("onDestroyWindow.");
}
void DroidBlaster::onGainFocus() {
    demo::Log::debug("onGainFocus.");
}
void DroidBlaster::onLostFocus() {
    demo::Log::debug("onLostFocus.");
}

//void DroidBlaster::clear() {
//    memset(mWindowBuffer.bits, 0,
//            mWindowBuffer.stride * mWindowBuffer.height * sizeof(uint32_t*));
//}
//
//void DroidBlaster::drawCursor(int size, int px, int py) {
//    const int halfSize = size / 2;
//    const int upLeftX = px - halfSize;
//    const int upLeftY = py - halfSize;
//    const int downRightX = px + halfSize;
//    const int downRightY = py + halfSize;
//
//    uint32_t* line = reinterpret_cast<uint32_t*>(mWindowBuffer.bits)
//            + (mWindowBuffer.stride * upLeftY);
//    for (int iy = upLeftY; iy < downRightY; ++iy) {
//        for (int ix = upLeftX; ix < downRightX; ++ix) {
//            line[ix] = 255;
//        }
//        line = line + mWindowBuffer.stride;
//    }
//}

}

