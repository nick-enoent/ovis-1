include ../common.am

libsysclassib_la_SOURCES = sysclassib.c
libsysclassib_la_CFLAGS  = $(SAMPLER_CFLAGS)
libsysclassib_la_LIBADD  = $(SAMPLER_LIBADD) -libmad -libumad
libsysclassib_la_LDFLAGS = $(SAMPLER_LDFLAGS)

pkglib_LTLIBRARIES = libsysclassib.la
