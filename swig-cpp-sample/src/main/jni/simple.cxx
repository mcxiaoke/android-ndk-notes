#include "simple.h"
#include <string>

int add(int a, int b) {
    return a + b;
}

MyClass::MyClass(): i(100) {

}

MyClass::MyClass(int x): i(x) {

}

MyClass::~MyClass() {
    i = 0;
}

std::string MyClass::show() {
    return std::string("value=" + i);
}
