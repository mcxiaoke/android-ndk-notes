/*
 * Main.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "Context.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"

void android_main(android_app *app) {
    demo::TimeService timeService;
    demo::GraphicsService graphicsService(app, &timeService);
    demo::Context context = { &graphicsService, &timeService };
    demo::EventLoop eventLoop(app);
    db::DroidBlaster droidBlaster(&context);
    eventLoop.run(droidBlaster);
}

