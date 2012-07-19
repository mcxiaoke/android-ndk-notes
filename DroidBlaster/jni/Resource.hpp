#ifndef _PACKT_RESOURCE_HPP_
#define _PACKT_RESOURCE_HPP_

#include "Types.hpp"

#include <android_native_app_glue.h>
#include <fstream>

namespace packt {
    class Resource {
    public:
        Resource(android_app* pApplication, const char* pPath);

        const char* getPath();

        status open();
        void close();
        status read(void* pBuffer, size_t pCount);

        off_t getLength();
        const void* bufferize();

    private:
        const char* mPath;
        std::ifstream mInputStream;
        char* mBuffer;
    };
}
#endif
