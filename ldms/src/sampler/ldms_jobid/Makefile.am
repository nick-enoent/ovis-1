include ../common.am

libjobid_la_SOURCES = ldms_jobid.c
libjobid_la_CFLAGS = $(SAMPLER_CFLAGS)
libjobid_la_LIBADD = $(SAMPLER_LIBADD)
libjobid_la_LDFLAGS = $(SAMPLER_LDFLAGS) libjobid.la

libjobidincludedir = $(includedir)/ldms/sampler
libjobidinclude_HEADERS = ldms_jobid.h
pkglib_LTLIBRARIES = libjobid.la
