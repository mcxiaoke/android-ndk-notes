#ifndef _DBS_BACKGROUND_HPP_
#define _DBS_BACKGROUND_HPP_

#include "Context.hpp"
#include "GraphicsService.hpp"
#include "GraphicsTileMap.hpp"
#include "Types.hpp"

namespace dbs {
    class Background {
    public:
        Background(packt::Context* pContext);

        void spawn();
        void update();

    private:
        packt::TimeService* mTimeService;
        packt::GraphicsService* mGraphicsService;

        packt::GraphicsTileMap* mTileMap;
        packt::Location mLocation; float mAnimSpeed;
    };
}
#endif
