/*
 * EventLoop.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "EventLoop.hpp"
#include "Log.hpp"

namespace demo {
EventLoop::EventLoop(android_app *app) :
		mApp(app), mEnabled(false), mQuit(false), mEventHandler(NULL) {
	mApp->onAppCmd = activityCallback;
	mApp->userData = this;
}

void EventLoop::run(EventHandler &eventHandler) {
	int32_t result;
	int32_t events;
	android_poll_source *source;
	app_dummy();
	mEventHandler = &eventHandler;

	demo::Log::info("starting event loop");
	while (true) {
		while ((result = ALooper_pollAll(mEnabled ? 0 : -1, NULL, &events,
				(void**) &source)) >= 0) {
			if (source != NULL) {
				demo::Log::info("processing an event.");
				source->process(mApp, source);
			}
			if (mApp->destroyRequested) {
				demo::Log::info("Exiting event loop.");
				return;
			}
		}

		if (mEnabled && (!mQuit)) {
			if (mEventHandler->onStep() != STATUS_OK) {
				mQuit = true;
				ANativeActivity_finish(mApp->activity);
			}
		}
	}
}

void EventLoop::activate() {
	if ((!mEnabled) && (mApp->window != NULL)) {
		mQuit = false;
		mEnabled = true;
		if (mEventHandler->onActivate() != STATUS_OK) {
			mQuit = true;
			ANativeActivity_finish(mApp->activity);
		}
	}
}

void EventLoop::deactivate() {
	if (mEnabled) {
		mEventHandler->onDeactivate();
		mEnabled = false;
	}
}

void EventLoop::processActivityEvent(int32_t cmd) {
	switch (cmd) {
	case APP_CMD_CONFIG_CHANGED:
		mEventHandler->onConfigurationChanged();
		break;
	case APP_CMD_INIT_WINDOW:
		mEventHandler->onCreateWindow();
		break;
	case APP_CMD_DESTROY:
		mEventHandler->onDestroy();
		break;
	case APP_CMD_GAINED_FOCUS:
		activate();
		mEventHandler->onGainFocus();
		break;
	case APP_CMD_LOST_FOCUS:
		deactivate();
		mEventHandler->onLostFocus();
		break;
	case APP_CMD_LOW_MEMORY:
		mEventHandler->onLowMemory();
		break;
	case APP_CMD_PAUSE:
		mEventHandler->onPause();
		deactivate();
		break;
	case APP_CMD_RESUME:
		mEventHandler->onResume();
		break;
	case APP_CMD_SAVE_STATE:
		mEventHandler->onSaveState(&mApp->savedState,&mApp->savedStateSize);
		break;
	case APP_CMD_START:
		mEventHandler->onStart();
		break;
	case APP_CMD_STOP:
		mEventHandler->onStop();
		break;
	case APP_CMD_TERM_WINDOW:
		mEventHandler->onDestroyWindow();
		deactivate();
		break;
	default:
		break;
	}
}

void EventLoop::activityCallback(android_app *app, int32_t cmd) {
	EventLoop &eventLoop = *(EventLoop*) app->userData;
	eventLoop.processActivityEvent(cmd);
}

}

