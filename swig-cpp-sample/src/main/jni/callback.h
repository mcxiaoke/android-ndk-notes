#ifndef CALLBACK_H
#define CALLBACK_H

#include <unistd.h>

class AsyncUidProvider {
public:
    AsyncUidProvider();
    virtual ~AsyncUidProvider();
    void getUid();
    virtual void onUid(uid_t uid){}
};

#endif
