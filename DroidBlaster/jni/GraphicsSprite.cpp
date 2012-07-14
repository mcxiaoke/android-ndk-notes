/*
 * GraphicsSprite.cpp
 *
 *  Created on: 2012-7-14
 *      Author: mcxiaoke
 */

#include "GraphicsSprite.hpp"
#include "Log.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace demo {

GraphicsSprite::GraphicsSprite(GraphicsTexture* texture, int32_t height,
        int32_t width, Location* location) :
        mTexture(texture), mLocation(location), mHeight(height), mWidth(width), mFrameCount(
                0), mFrameXCount(0), mFrameYCount(0), mAnimStartFrame(0), mAnimFrameCount(
                0), mAnimSpeed(0), mAnimFrame(0), mAnimLoop(false) {
}

void GraphicsSprite::load() {
    Log::info("GraphicsSprite::load()");
    mFrameXCount = mTexture->getWidth() / mWidth;
    mFrameYCount = mTexture->getHeight() / mHeight;
    mFrameCount = (mTexture->getHeight() / mHeight)
            * (mTexture->getWidth() / mWidth);
}

void GraphicsSprite::setAnimation(int32_t startFrame, int32_t frameCount,
        float speed, bool loop) {
    Log::info("GraphicsSprite::setAnimation()");
    mAnimStartFrame = startFrame;
    mAnimFrame = 0.0f;
    mAnimSpeed = speed;
    mAnimLoop = loop;
    int32_t maxFrameCount = mFrameCount - startFrame;
    if (frameCount > -1 && (frameCount <= maxFrameCount)) {
        mAnimFrameCount = frameCount;
    } else {
        mAnimFrameCount = maxFrameCount;
    }
}

bool GraphicsSprite::animationEnded() {
    Log::info("GraphicsSprite::animationEnded()");
    return mAnimFrame > (mAnimFrameCount - 1);
}

void GraphicsSprite::draw(float timeStep) {
    Log::info("GraphicsSprite::draw() timeStep:%d",timeStep);
    int32_t curFrame;
    int32_t curFrameX;
    int32_t curFrameY;

    mAnimFrame += timeStep * mAnimSpeed;
    if (mAnimLoop) {
        curFrame = (mAnimStartFrame + int32_t(mAnimFrame)) % mAnimFrameCount;
    } else {
        if (animationEnded()) {
            curFrame = mAnimStartFrame + (mAnimFrameCount - 1);
        } else {
            curFrame = mAnimStartFrame + int32_t(mAnimFrame);
        }
    }
    curFrameX = curFrame % mFrameXCount;
    curFrameY = mFrameYCount - 1 - (curFrame / mFrameXCount);

    mTexture->apply();
    int32_t crop[] =
            { curFrameX * mWidth, curFrameY * mHeight, mWidth, mHeight };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
    glDrawTexfOES(mLocation->mPosX - (mWidth / 2),
            mLocation->mPosY - (mHeight / 2), 0.0f, mWidth, mHeight);

}

}

