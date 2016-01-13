#ifndef _PACKT_EVENTLOOP_HPP_
#define _PACKT_EVENTLOOP_HPP_

#include "ActivityHandler.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace packt {
    class EventLoop {
    public:
        EventLoop(android_app* pApplication);
        void run(ActivityHandler* pActivityHandler,
                 InputHandler* pInputHandler);

    protected:
        void activate();
        void deactivate();

        void processAppEvent(int32_t pCommand);
        int32_t processInputEvent(AInputEvent* pEvent);
        void processSensorEvent();

    private:
        friend class Sensor;

        // Private callbacks handling events occuring in the thread loop.
        static void callback_event(android_app* pApplication,
            int32_t pCommand);
        static int32_t callback_input(android_app* pApplication,
            AInputEvent* pEvent);
        static void callback_sensor(android_app* pApplication,
            android_poll_source* pSource);

    private:
        // Saves application state when application is active/paused.
        bool mEnabled;
        // Indicates if the event handler wants to exit.
        bool mQuit;
        // Application details provided by Android.
        android_app* mApplication;
        // Activity event observer.
        ActivityHandler* mActivityHandler;
        // Input event observer.
        InputHandler* mInputHandler;

        // Event queue.
        ASensorManager* mSensorManager;
        ASensorEventQueue* mSensorEventQueue;
        android_poll_source mSensorPollSource;
    };
}
#endif
