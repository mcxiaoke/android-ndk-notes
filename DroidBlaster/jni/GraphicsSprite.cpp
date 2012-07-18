#include "GraphicsSprite.hpp"
#include "Log.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace packt {
    GraphicsSprite::GraphicsSprite(GraphicsTexture* pTexture,
        int32_t pHeight, int32_t pWidth, Location* pLocation) :
        mTexture(pTexture), mLocation(pLocation),
        mHeight(pHeight), mWidth(pWidth),
        mFrameCount(0), mFrameXCount(0), mFrameYCount(0),
        mAnimStartFrame(0), mAnimFrameCount(0),
        mAnimSpeed(0), mAnimFrame(0), mAnimLoop(false) {
    }

    void GraphicsSprite::load() {
        mFrameXCount = mTexture->getWidth() / mWidth;
        mFrameYCount = mTexture->getHeight() / mHeight;
        mFrameCount = (mTexture->getHeight() / mHeight)
                    * (mTexture->getWidth() / mWidth);
    }

    void GraphicsSprite::setAnimation(int32_t pStartFrame,
        int32_t pFrameCount, float pSpeed, bool pLoop) {
        mAnimStartFrame = pStartFrame;
        mAnimFrame = 0.0f, mAnimSpeed = pSpeed, mAnimLoop = pLoop;

        int32_t lMaxFrameCount = mFrameCount - pStartFrame;
        if ((pFrameCount > -1) && (pFrameCount <= lMaxFrameCount)) {
            mAnimFrameCount = pFrameCount;
        } else {
            mAnimFrameCount = lMaxFrameCount;
        }
    }

    bool GraphicsSprite::animationEnded() {
        return mAnimFrame > (mAnimFrameCount - 1);
    }

    void GraphicsSprite::draw(float pTimeStep) {
        int32_t lCurrentFrame, lCurrentFrameX, lCurrentFrameY;

        // Updates animation in loop mode.
        mAnimFrame += pTimeStep * mAnimSpeed;
        if (mAnimLoop) {
            lCurrentFrame = (mAnimStartFrame +
                             int32_t(mAnimFrame) % mAnimFrameCount);
        }
        // Updates animation in one-shot mode.
        else {
            // If animation ended.
            if (animationEnded()) {
                lCurrentFrame = mAnimStartFrame + (mAnimFrameCount-1);
            } else {
                lCurrentFrame = mAnimStartFrame + int32_t(mAnimFrame);
            }
        }
        // Computes frame X and Y indexes from its id.
        lCurrentFrameX = lCurrentFrame % mFrameXCount;
        // lCurrentFrameY is converted from OpenGL coordinates
        // to top-left coordinates.
        lCurrentFrameY = mFrameYCount - 1
                         - (lCurrentFrame / mFrameXCount);

        // Draws selected frame.
        mTexture->apply();
        int32_t lCrop[] = { lCurrentFrameX * mWidth,
                            lCurrentFrameY * mHeight,
                            mWidth, mHeight };
        glTexParameteriv(GL_TEXTURE_2D,
                         GL_TEXTURE_CROP_RECT_OES,
                         lCrop);
        glDrawTexfOES(mLocation->mPosX - (mWidth / 2),
                      mLocation->mPosY - (mHeight / 2),
                      0.0f, mWidth, mHeight);
    }
}
