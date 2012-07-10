/*
 * Main.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "DroidBlaster.hpp"
#include "EventLoop.hpp"

void android_main(android_app *app){
	demo::EventLoop eventLoop(app);
	db::DroidBlaster droidBlaster;
	eventLoop.run(droidBlaster);
}

