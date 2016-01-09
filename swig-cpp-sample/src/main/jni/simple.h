#ifndef UNIX_H
#define UNIX_H

#include <string>

using namespace std;

int add(int a, int b);

class MyClass {
public:
    MyClass();
    MyClass(int x);
    ~MyClass();
    std::string show();
private:
    int i;
};

#endif
