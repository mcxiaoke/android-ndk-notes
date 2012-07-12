/*
 * Context.hpp
 *
 *  Created on: 2012-7-11
 *      Author: mcxiaoke
 */

#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "Types.hpp"

namespace demo {

class GraphicsService;
class TimeService;

struct Context {
    GraphicsService *mGraphicsService;
    TimeService *mTimeService;
};
}

#endif /* CONTEXT_HPP_ */
