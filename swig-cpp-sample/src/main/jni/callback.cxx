#include "callback.h"

AsyncUidProvider::AsyncUidProvider() {

}

AsyncUidProvider::~AsyncUidProvider() {

}

void AsyncUidProvider::getUid() {
    onUid(getuid());
}
