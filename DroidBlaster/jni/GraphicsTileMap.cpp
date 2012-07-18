#include "GraphicsTileMap.hpp"
#include "Log.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>

#include "rapidxml.hpp"

namespace rapidxml {
    static jmp_buf sJmpBuffer;

    void parse_error_handler(const char* pWhat, void* pWhere) {
        packt::Log::error("Error while parsing TMX file.");
        packt::Log::error(pWhat);
        // If this function returns, result is undefined (i.e. crash).
        // Needs to jump instead...
        longjmp(sJmpBuffer, 0);
    }
}

namespace packt {
    GraphicsTileMap::GraphicsTileMap(android_app* pApplication,
        const char* pPath, GraphicsTexture* pTexture,
        Location* pLocation) :
        mResource(pApplication, pPath), mLocation(pLocation),
        mTexture(pTexture), mVertexBuffer(0), mIndexBuffer(0),
        mVertexCount(0), mIndexCount(0), mVertexComponents(5),
        mHeight(0), mWidth(0),
        mTileHeight(0), mTileWidth(0), mTileCount(0), mTileXCount(0)
    {}

    int32_t* GraphicsTileMap::loadFile() {
        using namespace rapidxml;
        xml_document<> lXmlDocument;
        xml_node<>* lXmlMap, *lXmlTileset, *lXmlLayer;
        xml_node<>* lXmlTile, *lXmlData;
        xml_attribute<>* lXmlTileWidth, *lXmlTileHeight;
        xml_attribute<>* lXmlWidth, *lXmlHeight, *lXmlGID;
        char* lFileBuffer = NULL; int32_t* lTiles = NULL;

        packt_Log_debug("Opening TMX file");
        if (mResource.open() != STATUS_OK) goto ERROR;
        {
            int32_t lLength = mResource.getLength();
            if (lLength <= 0) goto ERROR;
            const void* lFileBufferTmp = mResource.bufferize();
            if (lFileBufferTmp == NULL) goto ERROR;
            // Copy the constant buffer into a modifiable buffer.
            lFileBuffer = new char[lLength + 1];
            memcpy(lFileBuffer, lFileBufferTmp, lLength);
            lFileBuffer[lLength] = '\0';
            mResource.close();
        }
        // Parses the document. Jumps back here if an error occurs
        if (setjmp(sJmpBuffer)) goto ERROR;
        lXmlDocument.parse<parse_default>(lFileBuffer);

        // Reads XML tags.
        packt_Log_debug("Parsing TMX file");
        lXmlMap = lXmlDocument.first_node("map");
        if (lXmlMap == NULL) goto ERROR;
        lXmlTileset = lXmlMap->first_node("tileset");
        if (lXmlTileset == NULL) goto ERROR;
        lXmlTileWidth = lXmlTileset->first_attribute("tilewidth");
        if (lXmlTileWidth == NULL) goto ERROR;
        lXmlTileHeight = lXmlTileset->first_attribute("tileheight");
        if (lXmlTileHeight == NULL) goto ERROR;

        lXmlLayer = lXmlMap->first_node("layer");
        if (lXmlLayer == NULL) goto ERROR;
        lXmlWidth = lXmlLayer->first_attribute("width");
        if (lXmlWidth == NULL) goto ERROR;
        lXmlHeight = lXmlLayer->first_attribute("height");
        if (lXmlHeight == NULL) goto ERROR;

        lXmlData = lXmlLayer->first_node("data");
        if (lXmlData == NULL) goto ERROR;

        // Initializes member data.
        packt_Log_debug("Loading tiles from TMX file");
        mWidth      = atoi(lXmlWidth->value());
        mHeight     = atoi(lXmlHeight->value());
        mTileWidth  = atoi(lXmlTileWidth->value());
        mTileHeight = atoi(lXmlTileHeight->value());
        if ((mWidth <= 0) || (mHeight <= 0)
         || (mTileWidth <= 0) || (mTileHeight <= 0)) goto ERROR;
        // Number of tiles in the tile sheet.
        mTileXCount = mTexture->getWidth()/mTileWidth;
        mTileCount = mTexture->getHeight()/mTileHeight * mTileXCount;

        // Loads each tile index into a memory buffer.
        lTiles = new int32_t[mWidth * mHeight];
        lXmlTile = lXmlData->first_node("tile");
        for (int32_t lY = mHeight - 1; lY >= 0; --lY) {
            for (int32_t lX = 0; lX < mWidth; ++lX) {
                if (lXmlTile == NULL) goto ERROR;
                lXmlGID = lXmlTile->first_attribute("gid");
                // TMX files first tile index is 1 instead of 0 (which
                // means no tile), hence -1. Vertical coordinates are
                // reversed between TMX files and OpenGL.
                lTiles[lX + (lY * mWidth)] = atoi(lXmlGID->value())-1;
                if (lTiles[lX + (lY * mWidth)] < 0) goto ERROR;
                // Goes to next tile.
                lXmlTile = lXmlTile->next_sibling("tile");
            }
        }
        delete[] lFileBuffer;
        return lTiles;

    ERROR:
        mResource.close();
        delete[] lFileBuffer; delete[] lTiles;
        mHeight = 0;     mWidth = 0;
        mTileHeight = 0; mTileWidth = 0;
        return NULL;
    }

    void GraphicsTileMap::loadVertices(int32_t* pTiles,
        uint8_t** pVertexBuffer, uint32_t* pVertexBufferSize) {
        Log::info("Loading vertices.");

        // Allocates a temporary vertex buffer.
        // 4 vertices composed of 5 floats (X/Y/Z and U/V) per tile.
        mVertexCount = mHeight * mWidth * 4;
        *pVertexBufferSize = mVertexCount * mVertexComponents;
        GLfloat* lVBuffer = new GLfloat[*pVertexBufferSize];
        *pVertexBuffer = reinterpret_cast<uint8_t*>(lVBuffer);
        int32_t lRowStride = mWidth * 2;
        // Height and width of 1 pixel in UV coordinates (a texel).
        GLfloat lTexelWidth = 1.0f / mTexture->getWidth();
        GLfloat lTexelHeight = 1.0f / mTexture->getHeight();

        // Populates the vertex buffer.
        int32_t i;
        for (int32_t tileY = 0; tileY < mHeight; ++tileY) {
            for (int32_t tileX = 0; tileX < mWidth; ++tileX) {
                // Finds current tile index (0 for 1st tile, 1...).
                int32_t lTileSprite = pTiles[tileY * mWidth + tileX] % mTileCount;
                int32_t lTileSpriteX = (lTileSprite % mTileXCount) * mTileWidth;
                int32_t lTileSpriteY = (lTileSprite / mTileXCount) * mTileHeight;

                // Values to compute vertex offsets in the buffer.
                int32_t lOffsetX1 = tileX * 2;
                int32_t lOffsetX2 = tileX * 2 + 1;
                int32_t lOffsetY1 = (tileY * 2) * (mWidth * 2);
                int32_t lOffsetY2 = (tileY * 2 + 1) * (mWidth * 2);
                // Vertex positions in the scene.
                GLfloat lPosX1 = tileX * mTileWidth;
                GLfloat lPosX2 = (tileX + 1) * mTileWidth;
                GLfloat lPosY1 = tileY * mTileHeight;
                GLfloat lPosY2 = (tileY + 1) * mTileHeight;
                // Tile UV coordinates (coordinates origin needs to be
                // translated from top-left to bottom-left origin).
                GLfloat lU1 = (lTileSpriteX) * lTexelWidth;
                GLfloat lU2 = lU1 + (mTileWidth * lTexelWidth);
                GLfloat lV2 = 1.0f - (lTileSpriteY) * lTexelHeight;
                GLfloat lV1 = lV2 - (mTileHeight * lTexelHeight);
                // Small shift to limit edge artifacts (1/4 of texel).
                lU1 += lTexelWidth/4.0f;  lU2 -= lTexelWidth/4.0f;
                lV1 += lTexelHeight/4.0f; lV2 -= lTexelHeight/4.0f;

                // 4 vertices per tile in the vertex buffer.
                // X1Y2---X2Y2
                //  |       |
                //  |       |
                //  |       |
                // X1Y1---X2Y1
                i = mVertexComponents * (lOffsetY1 + lOffsetX1);
                lVBuffer[i++] = lPosX1; lVBuffer[i++] = lPosY1;
                lVBuffer[i++] = 0.0f;
                lVBuffer[i++] = lU1;    lVBuffer[i++] = lV1;
                i = mVertexComponents * (lOffsetY1 + lOffsetX2);
                lVBuffer[i++] = lPosX2; lVBuffer[i++] = lPosY1;
                lVBuffer[i++] = 0.0f;
                lVBuffer[i++] = lU2;    lVBuffer[i++] = lV1;
                i = mVertexComponents * (lOffsetY2 + lOffsetX1);
                lVBuffer[i++] = lPosX1; lVBuffer[i++] = lPosY2;
                lVBuffer[i++] = 0.0f;
                lVBuffer[i++] = lU1;    lVBuffer[i++] = lV2;
                i = mVertexComponents * (lOffsetY2 + lOffsetX2);
                lVBuffer[i++] = lPosX2; lVBuffer[i++] = lPosY2;
                lVBuffer[i++] = 0.0f;
                lVBuffer[i++] = lU2;    lVBuffer[i++] = lV2;
            }
        }
    }

    void GraphicsTileMap::loadIndexes(uint8_t** pIndexBuffer,
                                      uint32_t* pIndexBufferSize) {
        Log::info("Loading indexes.");

        // Allocates a temporary index buffer. 2 triangles per tile.
        mIndexCount =  mHeight * mWidth * 6;
        *pIndexBufferSize = mIndexCount;
        GLushort* lIBuffer = new GLushort[*pIndexBufferSize];
        *pIndexBuffer  = reinterpret_cast<uint8_t*>(lIBuffer);
        int32_t lRowStride = mWidth * 2;

        int32_t i = 0;
        for (int32_t tileY = 0; tileY < mHeight; tileY++) {
            int32_t lIndexY = tileY * 2;
            for (int32_t tileX = 0; tileX < mWidth; tileX++) {
                int32_t lIndexX = tileX * 2;

                // Values to compute vertex offsets in the buffer.
                GLshort lVertIndexY1 = lIndexY * lRowStride;
                GLshort lVertIndexY2 = (lIndexY + 1) * lRowStride;
                GLshort lVertIndexX1 = lIndexX;
                GLshort lVertIndexX2 = lIndexX + 1;

                // 2 triangles per tile in the index buffer.
                // X1Y2---X2Y2
                //  |  \    |
                //  |   \   |
                //  |    \  |
                // X1Y1---X2Y1
                lIBuffer[i++] = lVertIndexY1 + lVertIndexX1;
                lIBuffer[i++] = lVertIndexY1 + lVertIndexX2;
                lIBuffer[i++] = lVertIndexY2 + lVertIndexX1;

                lIBuffer[i++] = lVertIndexY2 + lVertIndexX1;
                lIBuffer[i++] = lVertIndexY1 + lVertIndexX2;
                lIBuffer[i++] = lVertIndexY2 + lVertIndexX2;
            }
        }
    }

    status GraphicsTileMap::load() {
        GLenum lErrorResult;
        uint8_t* lVertexBuffer = NULL, *lIndexBuffer = NULL;
        uint32_t lVertexBufferSize, lIndexBufferSize;

        // Loads tiles and creates temporary vertex/index buffers.
        int32_t* lTiles = loadFile();
        if (lTiles == NULL) goto ERROR;
        loadVertices(lTiles, &lVertexBuffer, &lVertexBufferSize);
        if (lVertexBuffer == NULL) goto ERROR;
        loadIndexes(&lIndexBuffer,  &lIndexBufferSize);
        if (lIndexBuffer == NULL) goto ERROR;

        // Generates new buffer names.
        packt_Log_debug("Loads tiles into OpenGL %d.", lIndexBuffer);
        glGenBuffers(1, &mVertexBuffer);
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

        // Loads buffers into OpenGL.
        glBufferData(GL_ARRAY_BUFFER, lVertexBufferSize *
                     sizeof(GLfloat), lVertexBuffer, GL_STATIC_DRAW);
        lErrorResult = glGetError();
        if (lErrorResult != GL_NO_ERROR) goto ERROR;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lIndexBufferSize *
                     sizeof(GLushort), lIndexBuffer, GL_STATIC_DRAW);
        lErrorResult = glGetError();
        if (lErrorResult != GL_NO_ERROR) goto ERROR;

        // Unbinds buffers.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        delete[] lTiles;
        delete[] lVertexBuffer; delete[] lIndexBuffer;
        return STATUS_OK;

    ERROR:
        Log::error("Error loading tilemap");
        unload();
        delete[] lTiles;
        delete[] lVertexBuffer; delete[] lIndexBuffer;
        return STATUS_KO;
    }

    void GraphicsTileMap::unload() {
        mHeight     = 0, mWidth      = 0;
        mTileHeight = 0, mTileWidth  = 0;
        mTileCount  = 0, mTileXCount = 0;

        if (mVertexBuffer != 0) {
            glDeleteBuffers(1, &mVertexBuffer);
            mVertexBuffer = 0; mVertexCount = 0;
        }
        if (mIndexBuffer != 0) {
            glDeleteBuffers(1, &mIndexBuffer);
            mIndexBuffer = 0; mIndexCount = 0;
        }
    }

    void GraphicsTileMap::draw() {
        int32_t lVertexSize = mVertexComponents * sizeof(GLfloat);
        GLvoid* lPosOffset  = (GLvoid*) 0;
        GLvoid* lUVOffset   = (GLvoid*) (sizeof(GLfloat) * 3);
        mTexture->apply();

        // Translates the tile map. Rounds value to the nearest
        // integer to avoid edge seams.
        glPushMatrix();
        glTranslatef(int32_t(mLocation->mPosX + 0.5f),
                     int32_t(mLocation->mPosY + 0.5f), 0.0f);

        // Draws using hardware buffers
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        glVertexPointer(3, GL_FLOAT, lVertexSize, lPosOffset);
        glTexCoordPointer(2, GL_FLOAT, lVertexSize, lUVOffset);

        glDrawElements(GL_TRIANGLES, mIndexCount,
                GL_UNSIGNED_SHORT, 0 * sizeof(GLushort));

        // Restores device state.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glPopMatrix();
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}
