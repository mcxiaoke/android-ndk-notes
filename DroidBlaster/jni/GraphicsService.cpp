#include "GraphicsService.hpp"
#include "Log.hpp"
#include "Resource.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>


namespace packt {
    GraphicsService::GraphicsService(android_app* pApplication,
                                     TimeService* pTimeService) :
        mApplication(pApplication),
        mTimeService(pTimeService),
        mWidth(0), mHeight(0),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_CONTEXT),
        mContext(EGL_NO_SURFACE),
        mTextures(), mTextureCount(0),
        mSprites(), mSpriteCount(0),
        mTileMaps(), mTileMapCount(0) {
        Log::info("Creating GraphicsService.");
    }

    GraphicsService::~GraphicsService() {
        Log::info("Destructing GraphicsService.");

        // Deletes tile maps.
        for (int32_t i = 0; i < mTileMapCount; ++i) {
            delete mTileMaps[i];
            mTileMaps[i] = NULL;
        }
        mTileMapCount = 0;

        // Deletes all sprites.
        for (int32_t i = 0; i < mSpriteCount; ++i) {
            delete mSprites[i];
            mSprites[i] = NULL;
        }
        mSpriteCount = 0;

        // Deletes all textures.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            delete mTextures[i];
            mTextures[i] = NULL;
        }
        mTextureCount = 0;
    }

    const int32_t& GraphicsService::getHeight() {
        return mHeight;
    }

    const int32_t& GraphicsService::getWidth() {
        return mWidth;
    }

    status GraphicsService::start() {
        Log::info("Starting GraphicsService.");

        EGLint lFormat, lNumConfigs, lErrorResult;
        EGLConfig lConfig;
        // Defines display requirements. 16bits mode here.
        const EGLint lAttributes[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
        };

        // Retrieves a display connection and initializes it.
        packt_Log_debug("Connecting to the display.");
        mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay == EGL_NO_DISPLAY) goto ERROR;
        if (!eglInitialize(mDisplay, NULL, NULL)) goto ERROR;

        // Selects the first OpenGL configuration found.
        packt_Log_debug("Selecting a display config.");
        if(!eglChooseConfig(mDisplay, lAttributes, &lConfig, 1,
            &lNumConfigs) || (lNumConfigs <= 0)) goto ERROR;

        // Reconfigures the Android window with the EGL format.
        packt_Log_debug("Configuring window format.");
        if (!eglGetConfigAttrib(mDisplay, lConfig,
            EGL_NATIVE_VISUAL_ID, &lFormat)) goto ERROR;
        ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0,
            lFormat);
        // Creates the display surface.
        packt_Log_debug("Initializing the display.");
        mSurface = eglCreateWindowSurface(mDisplay, lConfig,
            mApplication->window, NULL);
        if (mSurface == EGL_NO_SURFACE) goto ERROR;
        mContext = eglCreateContext(mDisplay, lConfig, EGL_NO_CONTEXT,
            NULL);
        if (mContext == EGL_NO_CONTEXT) goto ERROR;

        // Activates the display surface.
        packt_Log_debug("Activating the display.");
        if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
         || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
         || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
         || (mWidth <= 0) || (mHeight <= 0)) goto ERROR;
        glViewport(0, 0, mWidth, mHeight);

        // Displays information about OpenGL.
        Log::info("Starting GraphicsService");
        Log::info("Version  : %s", glGetString(GL_VERSION));
        Log::info("Vendor   : %s", glGetString(GL_VENDOR));
        Log::info("Renderer : %s", glGetString(GL_RENDERER));
        Log::info("Viewport : %d x %d", mWidth, mHeight);

        if (loadResources() != STATUS_OK) goto ERROR;
        setup();
        return STATUS_OK;

    ERROR:
        Log::error("Error while starting GraphicsService");
        stop();
        return STATUS_KO;
    }

    void GraphicsService::stop() {
        Log::info("Stopping GraphicsService.");
        unloadResources();

        // Destroys OpenGL context.
        if (mDisplay != EGL_NO_DISPLAY) {
            eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                           EGL_NO_CONTEXT);
            if (mContext != EGL_NO_CONTEXT) {
                eglDestroyContext(mDisplay, mContext);
                mContext = EGL_NO_CONTEXT;
            }
            if (mSurface != EGL_NO_SURFACE) {
                eglDestroySurface(mDisplay, mSurface);
                mSurface = EGL_NO_SURFACE;
            }
            eglTerminate(mDisplay);
            mDisplay = EGL_NO_DISPLAY;
        }
    }

    status GraphicsService::loadResources() {
        // Loads all requested textures.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            if (mTextures[i]->load() != STATUS_OK) {
                return STATUS_KO;
            }
        }
        // Loads all requested sprites.
        for (int32_t i = 0; i < mSpriteCount; ++i) {
            mSprites[i]->load();
        }
        // Loads all requested tile maps.
        for (int32_t i = 0; i < mTileMapCount; ++i) {
            if (mTileMaps[i]->load() != STATUS_OK) {
                return STATUS_KO;
            }
        }
        return STATUS_OK;
    }

    status GraphicsService::unloadResources() {
        for (int32_t i = 0; i < mTileMapCount; ++i) {
            mTileMaps[i]->unload();
        }
        for (int32_t i = 0; i < mTextureCount; ++i) {
            mTextures[i]->unload();
        }
        return STATUS_OK;
    }

    void GraphicsService::setup() {
        // Initializes base GL state.
        glEnable(GL_TEXTURE_2D);
        // In a simple 2D game, we have control over the third
        // dimension. So we do not really need a Z-buffer.
        glDisable(GL_DEPTH_TEST);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        // Set-up view aand projection matrixes.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrthof(0.0f, mWidth, 0.0f, mHeight, 0.0f, 1.0f);

        glMatrixMode( GL_MODELVIEW);
        glLoadIdentity();
    }

    status GraphicsService::update() {
        float lTimeStep = mTimeService->elapsed();

        // Draw tiles and sprites (with transparency).
        for (int32_t i = 0; i < mTileMapCount; ++i) {
            mTileMaps[i]->draw();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (int32_t i = 0; i < mSpriteCount; ++i) {
            mSprites[i]->draw(lTimeStep);
        }
        glDisable(GL_BLEND);

        // Shows rendering surface.
        if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers.", eglGetError());
            return STATUS_KO;
        }
        return STATUS_OK;
    }

    GraphicsTexture* GraphicsService::registerTexture(
        const char* pPath) {
        // Finds out if texture already loaded.
        for (int32_t i = 0; i < mTextureCount; ++i) {
            if (strcmp(pPath, mTextures[i]->getPath()) == 0) {
                return mTextures[i];
            }
        }

        // Appends a new texture to the texture array.
        GraphicsTexture* lTexture = new GraphicsTexture(
            mApplication, pPath);
        mTextures[mTextureCount++] = lTexture;
        return lTexture;
    }

    GraphicsSprite* GraphicsService::registerSprite(
        GraphicsTexture* pTexture, int32_t pHeight,
        int32_t pWidth, Location* pLocation) {
        // Appends a new sprite to the sprite array.
        GraphicsSprite* lSprite = new GraphicsSprite(pTexture,
            pHeight, pWidth, pLocation);
        mSprites[mSpriteCount++] = lSprite;
        return lSprite;
    }

    GraphicsTileMap* GraphicsService::registerTileMap(
        const char* pPath, GraphicsTexture* pTexture,
        Location* pLocation) {
        // Appends a new tile map to the tile map array.
        GraphicsTileMap* lTileMap = new GraphicsTileMap(mApplication,
            pPath, pTexture, pLocation);
        mTileMaps[mTileMapCount++] = lTileMap;
        return lTileMap;
    }
}
