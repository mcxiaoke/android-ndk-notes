/*
* @Author: mcxiaoke
* @Date:   2016-01-09 19:15:23
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-09 19:34:58
*/

int readOnly = 1000;
int readWrite = 2000;

long fact( int n) {
    if (n <= 1) return 1;
    else return n * fact(n - 1);
}
