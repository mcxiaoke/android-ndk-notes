/*
 * Main.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */


#include "EventLoop.hpp"

void android_main(android_app *app){
	demo::EventLoop eventLoop(app);
	eventLoop.run();
}

