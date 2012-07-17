#ifndef _DBS_BACKGROUND_HPP_
#define _DBS_BACKGROUND_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsTileMap.hpp"
#include "Types.hpp"

namespace db {
    class Background {
    public:
        Background(demo::Context* pContext);

        void spawn();
        void update();

    private:
        demo::TimeService* mTimeService;
        demo::GraphicsService* mGraphicsService;

        demo::GraphicsTileMap* mTileMap;
        demo::Location mLocation; float mAnimSpeed;
    };
}
#endif
