/*
 * TimeService.hpp
 *
 *  Created on: 2012-7-11
 *      Author: mcxiaoke
 */

#ifndef TIMESERVICE_HPP_
#define TIMESERVICE_HPP_

#include "Types.hpp"
#include <time.h>

namespace demo {
class TimeService {
public:
	TimeService();

	void reset();
	void update();

	double now();
	float elapsed();
private:
	float mElapsed;
	double mLastTime;
};
}

#endif /* TIMESERVICE_HPP_ */
