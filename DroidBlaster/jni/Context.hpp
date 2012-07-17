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
class SoundService;
class TimeService;

struct Context {
    GraphicsService *mGraphicsService;
    SoundService *mSoundService;
    TimeService *mTimeService;
};
}

#endif /* CONTEXT_HPP_ */
