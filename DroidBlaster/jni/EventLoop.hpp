/*
 * EventLoop.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef EVENTLOOP_HPP_
#define EVENTLOOP_HPP_

#include "Types.hpp"
#include "EventHandler.hpp"
#include <android_native_app_glue.h>

namespace demo {
class EventLoop {
public:
    EventLoop(android_app *app);
    void run(EventHandler &eventHandler);
protected:
    void activate();
    void deactivate();
    void processActivityEvent(int32_t cmd);
private:
    static void activityCallback(android_app *app, int32_t cmd);
private:
    bool mEnabled;
    bool mQuit;
    EventHandler *mEventHandler;
    android_app *mApp;
};
}

#endif /* EVENTLOOP_HPP_ */
