/*
* @Author: mcxiaoke
* @Date:   2016-01-10 20:34:50
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-10 20:35:44
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("uid = %d, gid = %d, pid = %d\n", getuid(), getgid(), getpid());
    return 0;
}
