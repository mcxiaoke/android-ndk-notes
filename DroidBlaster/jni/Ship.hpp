#ifndef _DBS_SHIP_HPP_
#define _DBS_SHIP_HPP_

#include "Context.hpp"
#include "InputService.hpp"
#include "GraphicsService.hpp"
#include "GraphicsSprite.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

namespace db {
    class Ship {
    public:
        Ship(demo::Context* pContext);

        void spawn();
        void update();

    private:
        demo::GraphicsService* mGraphicsService;
        demo::InputService* mInputService;
        demo::TimeService* mTimeService;

        demo::GraphicsSprite* mSprite;
        demo::Location mLocation;
        float mAnimSpeed;
    };
}
#endif
