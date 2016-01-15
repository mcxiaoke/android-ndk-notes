#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include "activity_handler.h"
#include <android_native_app_glue.h>


class EventLoop {
public:
    EventLoop(android_app* app, ActivityHandler& handler);
    void  run();
private:
    void activate();
    void deactivate();
    void processAppEvent(int32_t cmd);
    static void callback_appEvent(android_app* app, int32_t cmd);
private:
    android_app* mApp;
    bool mEnabled;
    bool mQuit;
    ActivityHandler& mActivityHandler;
};

#endif
