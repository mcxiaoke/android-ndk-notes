/*
 * GraphicsSprite.hpp
 *
 *  Created on: 2012-7-14
 *      Author: mcxiaoke
 */

#ifndef GRAPHICSSPRITE_HPP_
#define GRAPHICSSPRITE_HPP_

#include "GraphicsTexture.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

namespace demo {
class GraphicsSprite {
public:
    GraphicsSprite(GraphicsTexture* texture, int32_t height, int32_t width,
            Location* location);

    void load();
    void draw(float timeStep);

    void setAnimation(int32_t startFrame, int32_t frameCount, float speed,
            bool loop);
    bool animationEnded();

private:
    GraphicsTexture* mTexture;
    Location* mLocation;
    // for frame
    int32_t mHeight;
    int32_t mWidth;
    int32_t mFrameXCount;
    int32_t mFrameYCount;
    int32_t mFrameCount;
    // animation
    int32_t mAnimStartFrame;
    int32_t mAnimFrameCount;
    float mAnimSpeed;
    float mAnimFrame;
    bool mAnimLoop;
};

}

#endif /* GRAPHICSSPRITE_HPP_ */
