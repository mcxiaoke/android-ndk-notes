#APP_ABI := all
#APP_ABI := armeabi armeabi-v7a arm64_v8a x86 x86_64 mips mips64
APP_ABI := armeabi x86 
APP_PLATFORM := android-15

# APP_MODULES: By default the Android NDK build system builds all modules that
# are declared by the Android.mk file. This variable can override this behavior and
# provide a space-separated list of modules that need to be built

# APP_OPTIM: This variable can be set to either release or debug to alter the
# optimization level of the generated binaries. By default the release mode is
# used and the generated binaries are highly optimized. This variable can be set to
# debug mode to generate un-optimized binaries that are easier to debug.

# APP_CLAGS: This variable lists the compiler flags that will be passed to the
# compiler while compiling C and C++ source files for any of the modules.

# APP_CPPFLAGS: This variable lists the compilers flags that will be passed to the
# compiler while compiling the C++ source files for any of the modules.

# APP_BUILD_SCRIPT: By default the Android NDK build system looks for the
# Android.mk build file under the jni sub-directory of the project. This behavior
# can be altered by using this variable, and a different build file can be used.

# APP_ABI: By default Android NDK build system generates binaries for armeabi
# ABI. This variable can be used to alter this behavior and generate binaries for a
# different ABI

# APP_STL: By default the Android NDK build system uses the minimal STL runtime
# library, also known as the system library. This variable can be used to select a
# different STL implementation.
# APP_STL := stlport_shared
