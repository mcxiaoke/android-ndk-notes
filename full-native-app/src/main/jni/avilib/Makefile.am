# # Process this file with automake to produce Makefile.in.

AM_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(XIO_CFLAGS) \
	-I$(top_srcdir)

noinst_LTLIBRARIES = libavi.la libwav.la

# this can be improved

if ENABLE_EXPERIMENTAL
PLATFORM = platform_tc.c
else
PLATFORM = platform_posix.c
endif

libavi_la_SOURCES = \
	$(PLATFORM) \
	platform.h \
	avidump.c \
	avilib.c \
	avilib.h \
	static_avilib.h

libwav_la_SOURCES = \
	$(PLATFORM) \
	wavlib.c \
	wavlib.h \
	static_wavlib.h

EXTRA_DIST = README.avilib
