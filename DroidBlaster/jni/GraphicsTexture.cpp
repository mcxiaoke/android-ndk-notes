/*
 * GraphicsTexture.cpp
 *
 *  Created on: 2012-7-13
 *      Author: mcxiaoke
 */

#include "Log.hpp"
#include "GraphicsTexture.hpp"

namespace demo {
GraphicsTexture::GraphicsTexture(android_app* app, const char* path) :
        mResource(app, path), mTextureId(0), mWidth(0), mHeight(0), mFormat(0) {
}

const char* GraphicsTexture::getPath() {
    return mResource.getPath();
}

int32_t GraphicsTexture::getHeight() {
    return mHeight;
}

int32_t GraphicsTexture::getWidth() {
    return mWidth;
}

uint8_t* GraphicsTexture::loadImage() {
    png_byte header[8];
    png_structp pstruct = NULL;
    png_infop pinfo = NULL;
    png_byte* buffer = NULL;
    png_bytep* prow = NULL;
    png_int_32 rowSize;
    bool transparency;

    if (mResource.open() != STATUS_OK) {
        goto ERROR;
    }
    if (mResource.read(header, sizeof(header)) != STATUS_OK) {
        goto ERROR;
    }
    if (png_sig_cmp(header, 0, 0) != 0) {
        goto ERROR;
    }

    pstruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pstruct) {
        goto ERROR;
    }
    pinfo = png_create_info_struct(pstruct);
    if (!pinfo) {
        goto ERROR;
    }
    png_set_read_fn(pstruct, &mResource, callback_read);
    if (setjmp(png_jmpbuf(pstruct))) {
        goto ERROR;
    }

    png_set_sig_bytes(pstruct, 8);
    png_read_info(pstruct, pinfo);

    png_int_32 depth;
    png_int_32 colorType;
    png_uint_32 width;
    png_uint_32 height;
    png_get_IHDR(pstruct, pinfo, &width, &height, &depth, &colorType, NULL,
            NULL, NULL);
    mWidth = width;
    mHeight = height;

    transparency = false;

    if (png_get_valid(pstruct, pinfo, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(pstruct);
        transparency = true;
        goto ERROR;
    }
    if (depth < 0) {
        png_set_packing(pstruct);
    } else {
        png_set_strip_16(pstruct);
    }

    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(pstruct);
        mFormat = transparency ? GL_RGBA : GL_RGB;
    } else if (colorType == PNG_COLOR_TYPE_RGB) {
        mFormat = transparency ? GL_RGBA : GL_RGB;
    } else if (colorType == PNG_COLOR_TYPE_RGBA) {
        mFormat = GL_RGBA;
    } else if (colorType == PNG_COLOR_TYPE_GRAY) {
        png_set_expand_gray_1_2_4_to_8(pstruct);
        mFormat = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
    } else if (colorType == PNG_COLOR_TYPE_GA) {
        png_set_expand_gray_1_2_4_to_8(pstruct);
        mFormat = GL_LUMINANCE_ALPHA;
    }

    png_read_update_info(pstruct, pinfo);

    rowSize = png_get_rowbytes(pstruct, pinfo);
    if (rowSize <= 0) {
        goto ERROR;
    }
    buffer = new png_byte[rowSize * height];
    if (!buffer) {
        goto ERROR;
    }
    prow = new png_bytep[height];
    if (!prow) {
        goto ERROR;
    }

    for (int32_t i = 0; i < height; ++i) {
        prow[height - (i + 1)] = buffer + i * rowSize;
    }

    png_read_image(pstruct, prow);

    mResource.close();
    png_destroy_read_struct(&pstruct, &pinfo, NULL);
    delete[] prow;
    return buffer;

    ERROR: Log::error("error while read PNG file");
    mResource.close();
    delete[] prow;
    delete[] buffer;
    if (pstruct != NULL) {
        png_infop* ppinfo = (pinfo != NULL ? &pinfo : NULL);
        png_destroy_read_struct(&pstruct, ppinfo, NULL);
    }
    return NULL;

}

// copy from book code, pdf is incorrect;
void GraphicsTexture::callback_read(png_structp ppng, png_bytep data,
        png_size_t size) {
    Resource* res = ((Resource*) png_get_io_ptr(ppng));
    if (res->read(data, size) != STATUS_OK) {
        res->close();
    }
}

status GraphicsTexture::load() {
    uint8_t* buffer = loadImage();
    if (buffer == NULL) {
        return STATUS_KO;
    }
    GLenum error;
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // here ,pdf is incorrect too, missing a 0
    glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat,
            GL_UNSIGNED_BYTE, buffer);
    delete[] buffer;
    if (glGetError() != GL_NO_ERROR) {
        Log::error("error loading texture into opengl.");
        unload();
        return STATUS_KO;
    }
    return STATUS_OK;

}

void GraphicsTexture::unload() {
    if (mTextureId != 0) {
        glDeleteTextures(1, &mTextureId);
        mTextureId = 0;
    }
    mWidth = 0;
    mHeight = 0;
    mFormat = 0;

}

void GraphicsTexture::apply() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

}

