#ifndef _PACKT_GRAPHICSTILEMAP_HPP_
#define _PACKT_GRAPHICSTILEMAP_HPP_

#include "GraphicsTexture.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

namespace packt {
    class GraphicsTileMap {
    public:
        GraphicsTileMap(android_app* pApplication, const char* pPath,
            GraphicsTexture* pTexture, Location* pLocation);

        status load();
        void unload();
        void draw();

    private:
        int32_t* loadFile();
        void loadVertices(int32_t* pTiles, uint8_t** pVertexBuffer,
                          uint32_t* pVertexBufferSize);
        void loadIndexes(uint8_t** pIndexBuffer,
                         uint32_t* pIndexBufferSize);

    private:
        Resource mResource;
        Location* mLocation;
        // OpenGL resources.
        GraphicsTexture* mTexture;
        GLuint mVertexBuffer, mIndexBuffer;
        int32_t mVertexCount, mIndexCount;
        const int32_t mVertexComponents;
        // Tile map description.
        int32_t mHeight, mWidth;
        int32_t mTileHeight, mTileWidth;
        int32_t mTileCount, mTileXCount;
    };
}
#endif
