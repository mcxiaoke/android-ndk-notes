#ifndef _PACKT_GRAPHICSSERVICE_HPP_
#define _PACKT_GRAPHICSSERVICE_HPP_

#include "GraphicsSprite.hpp"
#include "GraphicsTexture.hpp"
#include "GraphicsTileMap.hpp"
#include "TimeService.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <EGL/egl.h>

namespace packt {
    class GraphicsService {
    public:
        GraphicsService(android_app* pApplication,
                        TimeService* pTimeService);
        ~GraphicsService();

        const int32_t& getHeight();
        const int32_t& getWidth();

        status start();
        void stop();
        status update();

        GraphicsTexture* registerTexture(const char* pPath);
        GraphicsSprite* registerSprite(GraphicsTexture* pTexture,
            int32_t pHeight, int32_t pWidth, Location* pLocation);
        GraphicsTileMap* registerTileMap(const char* pPath,
            GraphicsTexture* pTexture, Location* pLocation);

    protected:
        status loadResources();
        status unloadResources();
        void setup();

    private:
        android_app* mApplication;
        TimeService* mTimeService;

        // Display properties.
        int32_t mWidth, mHeight;
        EGLDisplay mDisplay;
        EGLSurface mSurface;
        EGLContext mContext;

        // Graphics resources.
        GraphicsTexture* mTextures[32]; int32_t mTextureCount;
        GraphicsSprite*  mSprites[256]; int32_t mSpriteCount;
        GraphicsTileMap* mTileMaps[8]; int32_t mTileMapCount;
    };
}
#endif
