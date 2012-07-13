/*
 * GraphicsTexture.hpp
 *
 *  Created on: 2012-7-13
 *      Author: mcxiaoke
 */

#ifndef GRAPHICSTEXTURE_HPP_
#define GRAPHICSTEXTURE_HPP_

#include "Context.hpp"
#include "Resource.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <GLES/gl.h>
#include <png.h>

namespace demo {
class GraphicsTexture {
public:
    GraphicsTexture(android_app* app, const char* path);

    const char* getPath();
    int32_t getHeight();
    int32_t getWidth();
    status load();
    void unload();
    void apply();

protected:
    uint8_t* loadImage();
private:
    static void callback_read(png_structp pst, png_bytep data, png_size_t size);
private:
    Resource mResource;
    GLuint mTextureId;
    int32_t mWidth;
    int32_t mHeight;
    GLint mFormat;
};

}

#endif /* GRAPHICSTEXTURE_HPP_ */
