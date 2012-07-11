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

DroidBlaster::DroidBlaster(demo::Context &context, android_app *app) :
		mApp(app), mTimeService(context.mTimeService), mInitialized(false), mPosX(
				0), mPosY(0), mSize(24), mSpeed(100.0f) {
	demo::Log::info("Creating DroidBlaster.");
}

DroidBlaster::~DroidBlaster() {
	demo::Log::info("Destructing DroidBlaster.");
}

demo::status DroidBlaster::onActivate() {
	demo::Log::info("Activating DroidBlaster.");
	mTimeService->reset();

	ANativeWindow *window = mApp->window;
	if (ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBX_8888)
			< 0) {
		return demo::STATUS_KO;
	}
	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0) {
		ANativeWindow_unlockAndPost(window);
	} else {
		return demo::STATUS_KO;
	}
	if (!mInitialized) {
		mPosX = mWindowBuffer.width / 2;
		mPosY = mWindowBuffer.height / 2;
		mInitialized = true;
	}
	return demo::STATUS_OK;
}

void DroidBlaster::onDeactivate() {
	demo::Log::info("Deactivating DroidBlaster.");
}

demo::status DroidBlaster::onStep() {
	demo::Log::info("Starting step.");
	mTimeService->update();

	mPosX = fmod(mPosX + mSpeed * mTimeService->elapsed(), mWindowBuffer.width);
	ANativeWindow *window = mApp->window;
	if (ANativeWindow_lock(window, &mWindowBuffer, NULL) >= 0) {
		clear();
		drawCursor(mSize, mPosX, mPosY);
		ANativeWindow_unlockAndPost(window);
		return demo::STATUS_OK;
	} else {
		return demo::STATUS_KO;
	}

	usleep(300000);
	demo::Log::info("Steping done..");
	return demo::STATUS_OK;
}

void DroidBlaster::onStart() {
	demo::Log::info("onStart.");
}

void DroidBlaster::onResume() {
	demo::Log::info("onResume.");
}
void DroidBlaster::onPause() {
	demo::Log::info("onPause.");
}
void DroidBlaster::onStop() {
	demo::Log::info("onStop.");
}
void DroidBlaster::onDestroy() {
	demo::Log::info("onDestroy.");
}

void DroidBlaster::onSaveState(void** data, size_t* size) {
	demo::Log::info("onSaveState.");
}
void DroidBlaster::onConfigurationChanged() {
	demo::Log::info("onConfigurationChanged.");
}
void DroidBlaster::onLowMemory() {
	demo::Log::info("onLowMemory.");
}

void DroidBlaster::onCreateWindow() {
	demo::Log::info("onCreateWindow.");
}
void DroidBlaster::onDestroyWindow() {
	demo::Log::info("onDestroyWindow.");
}
void DroidBlaster::onGainFocus() {
	demo::Log::info("onGainFocus.");
}
void DroidBlaster::onLostFocus() {
	demo::Log::info("onLostFocus.");
}

void DroidBlaster::clear() {
	memset(mWindowBuffer.bits, 0,
			mWindowBuffer.stride * mWindowBuffer.height * sizeof(uint32_t*));
}

void DroidBlaster::drawCursor(int size, int px, int py) {
	const int halfSize = size / 2;
	const int upLeftX = px - halfSize;
	const int upLeftY = py - halfSize;
	const int downRightX = px + halfSize;
	const int downRightY = py + halfSize;

	uint32_t* line = reinterpret_cast<uint32_t*>(mWindowBuffer.bits)
			+ (mWindowBuffer.stride * upLeftY);
	for (int iy = upLeftY; iy < downRightY; ++iy) {
		for (int ix = upLeftX; ix < downRightX; ++ix) {
			line[ix] = 255;
		}
		line = line + mWindowBuffer.stride;
	}
}

}

