#include <iostream>
#include <string>
#include <vector>
#include <sstream>


// struct Vector {
//   Vector(double x, double y, double z);
//   ~Vector();
//   double x, y, z;
// };

/**
class List {
public:
  List();
  ~List();
  int  search(char *item);
  void insert(char *item);
  void remove(char *item);
  char *get(int n);
  int  length;
};
**/

using namespace std;

class Shape {
// 如果有虚函数，SWIG不会生成Java构造函数
public:
  Shape();

  Shape(double a, double b);

  ~Shape();
  double  x, y;
  Shape&    move(double dx, double dy);
  std::string info();
  double area();
  double perimeter();
  void methodWithDefaultArgs(int a = 1, float b = 2.8, std::string c = "default arg");
  void methodOverloaded(int i);
  void methodOverloaded(double d);
  void methodOverloaded(char s[]);
  void methodOverloaded(int i, double d, std::vector<int> v);
  static int nshapes;
};

class Circle : public Shape {
private:
  double radius;
public:
  Circle(double r) : radius(r) { }
  virtual std::string info() = 0;
  virtual double area();
  virtual double perimeter();
};

class Square : public Shape {
private:
  double width;
public:
  Square(double w) : width(w) { }
  virtual std::string info() = 0;
  virtual double area();
  virtual double perimeter();
};
