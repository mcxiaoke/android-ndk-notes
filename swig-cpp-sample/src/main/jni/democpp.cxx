
#include "democpp.h"

#define M_PI 3.14159265358979323846

Shape::Shape() {
    nshapes++;
}

Shape::~Shape() {
    nshapes--;
}

Shape::Shape(double a, double b): x(a), y(b) {
    nshapes++;
}

/* Move the shape to a new location */
Shape& Shape::move(double dx, double dy) {
    x += dx;
    y += dy;
    return *this;
}

std::string Shape::info() {
    std::ostringstream ss;
    ss << "Shape(" << x << "," << y << ")" << std::endl;
    return ss.str();
}

void Shape::methodWithDefaultArgs(int a, float b, std::string c) {
    std::cout << a << b << c << endl;
}

void Shape::methodOverloaded(int i) {
    std::cout << i << endl;
}
void  Shape::methodOverloaded(double d) {
    std::cout << d << endl;
}
void Shape::methodOverloaded(char s[]) {
    std::cout << std::string(s) << endl;
}
void Shape::methodOverloaded(int i, double d, std::vector<int> v) {
    std::stringstream ss;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i != 0)
            ss << "," << v[i];
    }
    std::string s = ss.str();
    std::cout << i << d << s << endl;
}

int Shape::nshapes = 0;

double Shape::area() {
    return x * y;
}

double Shape::perimeter() {
    return 2 * (x + y);
}

double Circle::area() {
    return M_PI * radius * radius;
}

double Circle::perimeter() {
    return 2 * M_PI * radius;
}

std::string Circle::info() {
    std::ostringstream ss;
    ss << "Circle(" << x << "," << y << ")" << std::endl;
    return ss.str();
}

double Square::area() {
    return width * width;
}

double Square::perimeter() {
    return 4 * width;
}

std::string Square::info() {
    std::ostringstream ss;
    ss << "Square(" << x << "," << y << ")" << std::endl;
    return ss.str();
}


