%module(directors=1) Native

%{
#include "unistd.h"
#include "democpp.h"
#include "simple.h"

class AsyncUidProvider {
public:
  AsyncUidProvider() {
  }
  virtual ~AsyncUidProvider() {
  }
  void getUid() {
   onUid(getuid());
  }
  virtual void onUid(uid_t uid) {
  }
};
%}

%include "enums.swg" // generate java enum type
%include "std_string.i" // std::string type
%include "std_vector.i" // std::vector type

%include "democpp.h"
%include "simple.h"


%feature("director") AsyncUidProvider;

typedef unsigned int uid_t; // typedef

class AsyncUidProvider {
public:
  AsyncUidProvider();
  virtual ~AsyncUidProvider();
  void getUid();
  virtual void onUid(uid_t uid);
};

/* Exception handling for getuid. */
/**
%exception getuid {
  $action
  if (!result) {
   jclass clazz = jenv->FindClass("java/lang/OutOfMemoryError");
   jenv->ThrowNew(clazz, "Out of Memory");
   return $null;
  }
}
**/

/* Exception handling for getuid. */
%javaexception("java.lang.IllegalAccessException") getuid {
  $action
  if (!result) {
   jclass clazz = jenv->FindClass("java/lang/IllegalAccessException");
   jenv->ThrowNew(clazz, "Illegal Access");
   return $null;
  }
}

/* Ask SWIG to wrap getuid function. */
extern uid_t getuid(void);

