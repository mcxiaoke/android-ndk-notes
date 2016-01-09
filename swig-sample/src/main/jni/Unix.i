%module Unix

%{
#include <unistd.h>
#include "Unix.h"
%}
%include "enums.swg" // generate java enum type

%javaconst(1); // default compile time constant
%javaconst(0) MAX_HEIGHT; // runtime constant

#define MAX_WIDTH 640 // compile constant
%constant int MAX_HEIGHT = 320; // runtime constant

enum { ONE = 1, TWO = 2, THREE, FOUR }; // enum
enum Direction { EAST = 10, SOUTH = 20, WEST = 30, NORTH = 40 }; // named enum

%immutable;
extern int readOnly; // read only
%mutable;
extern int readWrite; // read and write

/* Point structure. */
struct Point {
int x;
int y;
};


typedef unsigned int uid_t; // typedef

extern uid_t getuid(void); // method

extern long fact(int n); // method
