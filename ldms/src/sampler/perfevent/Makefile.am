include ../common.am

libperfevent_la_SOURCES = perfevent.c
libperfevent_la_CFLAGS  = $(SAMPLER_CFLAGS)
libperfevent_la_LIBADD  = $(SAMPLER_LIBADD) -lm
libperfevent_la_LDFLAGS = $(SAMPLER_LDFLAGS)

pkglib_LTLIBRARIES = libperfevent.la
