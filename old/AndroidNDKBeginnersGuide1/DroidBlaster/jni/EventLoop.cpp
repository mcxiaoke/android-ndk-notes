#include "EventLoop.hpp"
#include "Log.hpp"

namespace packt {
    EventLoop::EventLoop(android_app* pApplication) :
      mEnabled(false), mQuit(false),
      mApplication(pApplication),
      mActivityHandler(NULL), mInputHandler(NULL),
      mSensorPollSource(), mSensorManager(NULL),
      mSensorEventQueue(NULL) {
        mApplication->userData = this;
        mApplication->onAppCmd = callback_event;
        mApplication->onInputEvent = callback_input;
    }

    void EventLoop::run(ActivityHandler* pActivityHandler,
                        InputHandler* pInputHandler) {
        int32_t lResult;
        int32_t lEvents;
        android_poll_source* lSource;

        // Makes sure native glue is not stripped by the linker.
        app_dummy();
        mActivityHandler = pActivityHandler;
        mInputHandler    = pInputHandler;

        // Global step loop.
        packt::Log::info("Starting event loop");
        while (true) {
            // Event processing loop.
            while ((lResult = ALooper_pollAll(mEnabled ? 0 : -1,
                         NULL, &lEvents, (void**) &lSource)) >= 0) {
                // An event has to be processed.
                if (lSource != NULL) {
                    lSource->process(mApplication, lSource);
                }
                // Application is getting destroyed.
                if (mApplication->destroyRequested) {
                    packt::Log::info("Exiting event loop");
                    return;
                }
            }

            // Steps the application.
            if ((mEnabled) && (!mQuit)) {
                if (mActivityHandler->onStep() != STATUS_OK) {
                    mQuit = true;
                    ANativeActivity_finish(mApplication->activity);
                }
            }
        }
    }

    void EventLoop::activate() {
        // Enables activity only if a window is available.
        if ((!mEnabled) && (mApplication->window != NULL)) {
            // Registers sensor queue.
            mSensorPollSource.id = LOOPER_ID_USER;
            mSensorPollSource.app = mApplication;
            mSensorPollSource.process = callback_sensor;
            mSensorManager = ASensorManager_getInstance();
            if (mSensorManager != NULL) {
                mSensorEventQueue = ASensorManager_createEventQueue(
                    mSensorManager, mApplication->looper,
                    LOOPER_ID_USER, NULL, &mSensorPollSource);
                if (mSensorEventQueue == NULL) goto ERROR;
            }

            mQuit = false; mEnabled = true;
            if (mActivityHandler->onActivate() != STATUS_OK) {
                goto ERROR;
            }
        }
        return;

    ERROR:
        mQuit = true;
        deactivate();
        ANativeActivity_finish(mApplication->activity);
    }

    void EventLoop::deactivate() {
        if (mEnabled) {
            mActivityHandler->onDeactivate();
            mEnabled = false;

            if (mSensorEventQueue != NULL) {
                ASensorManager_destroyEventQueue(mSensorManager,
                    mSensorEventQueue);
                mSensorEventQueue = NULL;
            }
            mSensorManager = NULL;
        }
    }

    void EventLoop::processAppEvent(int32_t pCommand) {
        switch (pCommand) {
        case APP_CMD_CONFIG_CHANGED:
            mActivityHandler->onConfigurationChanged();
            break;
        case APP_CMD_INIT_WINDOW:
            mActivityHandler->onCreateWindow();
            break;
        case APP_CMD_DESTROY:
            mActivityHandler->onDestroy();
            break;
        case APP_CMD_GAINED_FOCUS:
            activate();
            mActivityHandler->onGainFocus();
            break;
        case APP_CMD_LOST_FOCUS:
            mActivityHandler->onLostFocus();
            deactivate();
            break;
        case APP_CMD_LOW_MEMORY:
            mActivityHandler->onLowMemory();
            break;
        case APP_CMD_PAUSE:
            mActivityHandler->onPause();
            deactivate();
            break;
        case APP_CMD_RESUME:
            mActivityHandler->onResume();
            break;
        case APP_CMD_SAVE_STATE:
            mActivityHandler->onSaveState(&mApplication->savedState,
                &mApplication->savedStateSize);
            break;
        case APP_CMD_START:
            mActivityHandler->onStart();
            break;
        case APP_CMD_STOP:
            mActivityHandler->onStop();
            break;
        case APP_CMD_TERM_WINDOW:
            mActivityHandler->onDestroyWindow();
            deactivate();
            break;
        default:
            break;
        }
    }

    void EventLoop::callback_event(android_app* pApplication,
        int32_t pCommand) {
        EventLoop& lEventLoop = *(EventLoop*) pApplication->userData;
        lEventLoop.processAppEvent(pCommand);
    }

    int32_t EventLoop::processInputEvent(AInputEvent* pEvent) {
        int32_t lEventType = AInputEvent_getType(pEvent);
        switch (lEventType) {
        case AINPUT_EVENT_TYPE_MOTION:
            switch (AInputEvent_getSource(pEvent)) {
            case AINPUT_SOURCE_TOUCHSCREEN:
                return mInputHandler->onTouchEvent(pEvent);
                break;

            case AINPUT_SOURCE_TRACKBALL:
                return mInputHandler->onTrackballEvent(pEvent);
                break;
            }
            break;

        case AINPUT_EVENT_TYPE_KEY:
            return mInputHandler->onKeyboardEvent(pEvent);
            break;
        }
        return 0;
    }

    int32_t EventLoop::callback_input(android_app* pApplication,
        AInputEvent* pEvent) {
        EventLoop& lEventLoop = *(EventLoop*) pApplication->userData;
        return lEventLoop.processInputEvent(pEvent);
    }

    void EventLoop::processSensorEvent() {
        ASensorEvent lEvent;
        while (ASensorEventQueue_getEvents(mSensorEventQueue,
                                           &lEvent, 1) > 0) {
            switch (lEvent.type) {
            case ASENSOR_TYPE_ACCELEROMETER:
                mInputHandler->onAccelerometerEvent(&lEvent);
                break;
            }
        }
    }

    void EventLoop::callback_sensor(android_app* pApplication,
        android_poll_source* pSource) {
        EventLoop& lEventLoop = *(EventLoop*) pApplication->userData;
        lEventLoop.processSensorEvent();
    }
}
