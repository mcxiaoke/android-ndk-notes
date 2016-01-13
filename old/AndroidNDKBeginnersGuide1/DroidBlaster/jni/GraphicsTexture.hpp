#ifndef _PACKT_GRAPHICSTEXTURE_HPP_
#define _PACKT_GRAPHICSTEXTURE_HPP_

#include "Context.hpp"
#include "Resource.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <GLES/gl.h>
#include <png.h>

namespace packt {
    class GraphicsTexture {
    public:
        GraphicsTexture(android_app* pApplication, const char* pPath);

        const char* getPath();
        int32_t getHeight();
        int32_t getWidth();

        status load();
        void unload();
        void apply();

    protected:
        uint8_t* loadImage();

    private:
        static void callback_read(png_structp pStruct,
            png_bytep pData, png_size_t pSize);

    private:
        Resource mResource;
        GLuint mTextureId;
        int32_t mWidth, mHeight;
        GLint mFormat;
    };
}
#endif
