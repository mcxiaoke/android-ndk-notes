#ifndef _PACKT_GRAPHICSSPRITE_HPP_
#define _PACKT_GRAPHICSSPRITE_HPP_

#include "GraphicsTexture.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

namespace packt {
    class GraphicsSprite {
    public:
        GraphicsSprite(GraphicsTexture* pImage,
            int32_t pHeight, int32_t pWidth, Location* pLocation);

        void load();
        void draw(float pTimeStep);

        void setAnimation(int32_t pStartFrame, int32_t pFrameCount,
            float pSpeed, bool pLoop);
        bool animationEnded();

    private:
        GraphicsTexture* mTexture;
        Location* mLocation;
        // Frame.
        int32_t mHeight, mWidth;
        int32_t mFrameXCount, mFrameYCount, mFrameCount;
        // Animation.
        int32_t mAnimStartFrame, mAnimFrameCount;
        float mAnimSpeed, mAnimFrame;
        bool mAnimLoop;
    };
}
#endif
