/*
 * Resource.hpp
 *
 *  Created on: 2012-7-13
 *      Author: mcxiaoke
 */

#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

#include "Types.hpp"
#include <android_native_app_glue.h>

namespace demo {
class Resource {
public:
    Resource(android_app* app, const char* path);
    const char* getPath();
    status open();
    void close();
    status read(void* buffer, size_t size);
private:
    const char* mPath;
    AAssetManager* mAssetManager;
    AAsset* mAsset;
};
}

#endif /* RESOURCE_HPP_ */
