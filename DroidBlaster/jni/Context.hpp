#ifndef _PACKT_CONTEXT_HPP_
#define _PACKT_CONTEXT_HPP_

#include "Types.hpp"

namespace packt {
    class GraphicsService;
    class InputService;
    class SoundService;
    class TimeService;

    struct Context {
        GraphicsService* mGraphicsService;
        InputService*    mInputService;
        SoundService*    mSoundService;
        TimeService*     mTimeService;
    };
}

#endif
