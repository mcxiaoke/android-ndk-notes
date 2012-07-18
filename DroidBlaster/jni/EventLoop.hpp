#ifndef _PACKT_EVENTLOOP_HPP_
#define _PACKT_EVENTLOOP_HPP_

#include "EventHandler.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace demo {
    class EventLoop {
    public:
        EventLoop(android_app* pApplication);
        void run(EventHandler* pActivityHandler,
                 InputHandler* pInputHandler);

    protected:
        void activate();
        void deactivate();

        void processAppEvent(int32_t pCommand);
        int32_t processInputEvent(AInputEvent* pEvent);

    private:
        // Private callbacks handling events occuring in the thread loop.
        static void callback_event(android_app* pApplication,
            int32_t pCommand);
        static int32_t callback_input(android_app* pApplication,
            AInputEvent* pEvent);

    private:
        // Saves application state when application is active/paused.
        bool mEnabled;
        // Indicates if the event handler wants to exit.
        bool mQuit;
        // Application details provided by Android.
        android_app* mApplication;
        // Activity event observer.
        EventHandler* mActivityHandler;
        // Input event observer.
        InputHandler* mInputHandler;
    };
}
#endif
