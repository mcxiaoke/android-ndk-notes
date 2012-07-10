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
		mApp(app) {
}

void EventLoop::run() {
	int32_t result;
	int32_t events;
	android_poll_source *source;
	app_dummy();

	demo::Log::info("starting event loop");
	while (true) {
		while ((result = ALooper_pollAll(-1, NULL, &events, (void**) &source))
				>= 0) {
			if (source != NULL) {
				demo::Log::info("processing an event.");
				source->process(mApp, source);
			}
			if (mApp->destroyRequested) {
				demo::Log::info("Exiting event loop.");
				return;
			}
		}
	}
}

}

