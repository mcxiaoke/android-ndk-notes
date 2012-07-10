/*
 * EventLoop.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef EVENTLOOP_HPP_
#define EVENTLOOP_HPP_

#include "Types.hpp"
#include <android_native_app_glue.h>

namespace demo{
	class EventLoop{
	public:
		EventLoop(android_app *app);
		void run();
	private:
		android_app *mApp;
	};
}




#endif /* EVENTLOOP_HPP_ */
