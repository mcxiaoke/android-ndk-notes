/*
* @Author: mcxiaoke
* @Date:   2016-01-15 21:29:59
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 22:21:14
*/

#include "app.h"
#include "event_loop.h"
#include "log.h"

void android_main(android_app* app) {
    MainApp(app).run();
}
