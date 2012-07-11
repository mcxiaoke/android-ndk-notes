/*
 * Context.hpp
 *
 *  Created on: 2012-7-11
 *      Author: mcxiaoke
 */

#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "Types.hpp"

namespace demo{

	class TimeService;

	struct Context{
		TimeService *mTimeService;
	};
}


#endif /* CONTEXT_HPP_ */
