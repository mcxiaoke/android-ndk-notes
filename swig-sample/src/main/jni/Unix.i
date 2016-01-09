%module Unix

%{
#include <unistd.h>
%}

typedef unsigned int uid_t;

extern uid_t getuid(void);
