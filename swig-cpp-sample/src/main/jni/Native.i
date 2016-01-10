%module Native

%{
#include "democpp.h"
#include "simple.h"
%}

%include "enums.swg" // generate java enum type
%include "std_string.i" // std::string type
%include "std_vector.i" // std::vector type

%include "democpp.h"
%include "simple.h"

%javaconst(1);
%javaconst(0) BIG;
%javaconst(0) LARGE;

#define EXPRESSION (0x100+5)
#define BIG 1000LL
#define LARGE 2000ULL
