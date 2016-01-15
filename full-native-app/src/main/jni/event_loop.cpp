/*
* @Author: mcxiaoke
* @Date:   2016-01-15 21:25:38
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 22:40:13
*/

#include "event_loop.h"
#include "log.h"

EventLoop::EventLoop(android_app* app, ActivityHandler& handler):
    mApp(app), mEnabled(false), mQuit(false), mActivityHandler(handler)
{
    mApp->userData = this;
    mApp->onAppCmd = callback_appEvent;
}

void EventLoop::run() {
    int32_t result;
    int32_t events;
    android_poll_source* source;
    app_dummy();
    Log::info("Starting event loop");
    while (true) {
        // event processing loop
        while ((result = ALooper_pollAll(mEnabled ? 0 : -1, NULL,
                                         &events, (void**)&source)) >= 0) {
            // an event has to be processed.
            if (source != NULL) {
                // Log::info("Processing an event");
                source->process(mApp, source);
            }
            if (mApp->destroyRequested) {
                Log::info("Exiting event loop");
                return;
            }
        }
        // steps the application
        if ((mEnabled) && (!mQuit)) {
            if (mActivityHandler.onStep() != STATUS_OK) {
                mQuit = true;
                ANativeActivity_finish(mApp->activity);
            }
        }
    }
}


void EventLoop::activate() {
    // enable activity only if a window is available
    if ((!mEnabled) && (mApp->window != NULL)) {
        mQuit = false;
        mEnabled = true;
        if (mActivityHandler.onActivate() != STATUS_OK) {
            // error, quit
            mQuit = true;
            deactivate();
            ANativeActivity_finish(mApp->activity);
        }
    }

    return;
}


void EventLoop::deactivate() {
    if (mEnabled) {
        mActivityHandler.onDeactivate();
        mEnabled = false;
    }
}


void EventLoop::callback_appEvent(android_app* app, int32_t cmd) {
    EventLoop& loop = *(EventLoop*) app->userData;
    loop.processAppEvent(cmd);
}

void  EventLoop::processAppEvent(int32_t cmd) {
    switch (cmd) {
    case APP_CMD_CONFIG_CHANGED:
        mActivityHandler.onConfigurationChanged();
        break;
    case APP_CMD_INIT_WINDOW:
        mActivityHandler.onCreateWindow();
        break;
    case APP_CMD_DESTROY:
        mActivityHandler.onDestroy();
        break;
    case APP_CMD_GAINED_FOCUS:
        activate();
        mActivityHandler.onGainFocus();
        break;
    case APP_CMD_LOST_FOCUS:
        mActivityHandler.onLostFocus();
        deactivate();
        break;
    case APP_CMD_LOW_MEMORY:
        mActivityHandler.onLowMemory();
        break;
    case APP_CMD_PAUSE:
        mActivityHandler.onPause();
        deactivate();
        break;
    case APP_CMD_RESUME:
        mActivityHandler.onResume();
        break;
    case APP_CMD_SAVE_STATE:
        mActivityHandler.onSaveInstanceState(
            &mApp->savedState, &mApp->savedStateSize);
        break;
    case APP_CMD_START:
        mActivityHandler.onStart();
        break;
    case APP_CMD_STOP:
        mActivityHandler.onStop();
        break;
    case APP_CMD_TERM_WINDOW:
        mActivityHandler.onDestroyWindow();
        deactivate();
        break;
    default:
        break;
    }
}
