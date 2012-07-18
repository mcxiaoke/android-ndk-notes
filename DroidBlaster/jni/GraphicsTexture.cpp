#include "Log.hpp"
#include "GraphicsTexture.hpp"

namespace packt {
    GraphicsTexture::GraphicsTexture(android_app* pApplication,
        const char* pPath) :
        mResource(pApplication, pPath),
        mTextureId(0),
        mWidth(0), mHeight(0)
    {}

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
        Log::info("Loading texture %s", mResource.getPath());

        png_byte lHeader[8];
        png_structp lPngPtr = NULL; png_infop lInfoPtr = NULL;
        png_byte* lImageBuffer = NULL; png_bytep* lRowPtrs = NULL;
        png_int_32 lRowSize; bool lTransparency;

        // Opens and checks image signature (first 8 bytes).
        if (mResource.open() != STATUS_OK) goto ERROR;
        packt_Log_debug("Checking signature.");
        if (mResource.read(lHeader, sizeof(lHeader)) != STATUS_OK)
            goto ERROR;
        if (png_sig_cmp(lHeader, 0, 8) != 0) goto ERROR;

        // Creates required structures.
        packt_Log_debug("Creating required structures.");
        lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
        if (!lPngPtr) goto ERROR;
        lInfoPtr = png_create_info_struct(lPngPtr);
        if (!lInfoPtr) goto ERROR;

        // Prepares reading operation by setting-up a read callback.
        png_set_read_fn(lPngPtr, &mResource, callback_read);
        // Set-up error management. If an error occurs while reading,
        // code will come back here and jump
        if (setjmp(png_jmpbuf(lPngPtr))) goto ERROR;

        // Ignores first 8 bytes already read and processes header.
        png_set_sig_bytes(lPngPtr, 8);
        png_read_info(lPngPtr, lInfoPtr);
        // Retrieves PNG info and updates PNG struct accordingly.
        png_int_32 lDepth, lColorType;
        png_uint_32 lWidth, lHeight;
        png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight,
            &lDepth, &lColorType, NULL, NULL, NULL);
        mWidth = lWidth; mHeight = lHeight;

        // Creates a full alpha channel if transparency is encoded as
        // an array of palette entries or a single transparent color.
        lTransparency = false;
        if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(lPngPtr);
            lTransparency = true;
            goto ERROR;
        }
        // Expands PNG with less than 8bits per channel to 8bits.
        if (lDepth < 8) {
            png_set_packing (lPngPtr);
        // Shrinks PNG with 16bits per color channel down to 8bits.
        } else if (lDepth == 16) {
            png_set_strip_16(lPngPtr);
        }
        // Indicates that image needs conversion to RGBA if needed.
        switch (lColorType) {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(lPngPtr);
            mFormat = lTransparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB:
            mFormat = lTransparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            mFormat = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            png_set_expand_gray_1_2_4_to_8(lPngPtr);
            mFormat = lTransparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(lPngPtr);
            mFormat = GL_LUMINANCE_ALPHA;
            break;
        }
        // Validates all tranformations.
        png_read_update_info(lPngPtr, lInfoPtr);

        // Get row size in bytes.
        lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
        if (lRowSize <= 0) goto ERROR;
        // Ceates the image buffer that will be sent to OpenGL.
        lImageBuffer = new png_byte[lRowSize * lHeight];
        if (!lImageBuffer) goto ERROR;
        // Pointers to each row of the image buffer. Row order is
        // inverted because different coordinate systems are used by
        // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
        lRowPtrs = new png_bytep[lHeight];
        if (!lRowPtrs) goto ERROR;
        for (int32_t i = 0; i < lHeight; ++i) {
            lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
        }
        // Reads image content.
        png_read_image(lPngPtr, lRowPtrs);

        // Frees memory and resources.
        mResource.close();
        png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
        delete[] lRowPtrs;
        return lImageBuffer;

    ERROR:
        Log::error("Error while reading PNG file");
        mResource.close();
        delete[] lRowPtrs; delete[] lImageBuffer;
        if (lPngPtr != NULL) {
            png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr: NULL;
            png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
        }
        return NULL;
    }

    void GraphicsTexture::callback_read(png_structp pStruct,
        png_bytep pData, png_size_t pSize) {
        Resource* lResource = ((Resource*) png_get_io_ptr(pStruct));
        if (lResource->read(pData, pSize) != STATUS_OK) {
            lResource->close();
        }
    }

    status GraphicsTexture::load() {
        uint8_t* lImageBuffer = loadImage();
        if (lImageBuffer == NULL) {
            return STATUS_KO;
        }

        // Creates a new OpenGL texture.
        GLenum lErrorResult;
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        // Set-up texture properties.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
            GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);
        // Loads image data into OpenGL.
        glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0,
                     mFormat, GL_UNSIGNED_BYTE, lImageBuffer);
        delete[] lImageBuffer;
        if (glGetError() != GL_NO_ERROR) {
            Log::error("Error loading texture into OpenGL.");
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
        mWidth = 0; mHeight = 0; mFormat = 0;
    }

    void GraphicsTexture::apply() {
        glActiveTexture( GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
    }
}
