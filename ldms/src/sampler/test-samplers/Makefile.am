include ../common.am

pkglib_LTLIBRARIES =

libclock_la_SOURCES = clock.c
libclock_la_CFLAGS  = $(SAMPLER_CFLAGS) 
libclock_la_LIBADD  = $(SAMPLER_LIBADD)
libclock_la_LDFLAGS = $(SAMPLER_LDFLAGS)

libgrptest_la_SOURCES = grptest.c
libgrptest_la_CFLAGS  = $(SAMPLER_CFLAGS)
libgrptest_la_LIBADD  = $(SAMPLER_LIBADD)
libgrptest_la_LDFLAGS = $(SAMPLER_LDFLAGS)

pkglib_LTLIBRARIES += libclock.la libgrptest.la
