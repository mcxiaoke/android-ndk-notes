/*
 * ActivityHandler.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef ACTIVITYHANDLER_HPP_
#define ACTIVITYHANDLER_HPP_

#include "Types.hpp"

namespace demo{
class EventHandler{
public:
	virtual ~EventHandler(){};


	virtual status onActivate() = 0;
	virtual void onDeactivate() = 0;
	virtual status onStep() = 0;

	virtual void onStart() {};
	virtual void onResume() {};
	virtual void onPause() {};
	virtual void onStop() {};
	virtual void onDestroy() {};

	virtual void onSaveState(void** data, size_t* size) {};
	virtual void onConfigurationChanged() {};
	virtual void onLowMemory() {};

	virtual void onCreateWindow(){};
	virtual void onDestroyWindow(){};
	virtual void onGainFocus(){};
	virtual void onLostFocus(){}

};

}


#endif /* ACTIVITYHANDLER_HPP_ */
