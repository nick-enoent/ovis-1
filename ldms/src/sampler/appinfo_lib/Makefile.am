include ../common.am

pkglib_LTLIBRARIES =

AM_CPPFLAGS = @OVIS_INCLUDE_ABS@
AM_LDFLAGS = @OVIS_LIB_ABS@

libappinfocl_la_SOURCES = appinfo.c ldms_appinfo.c ldms_appinfo.h ldms_appinfo_shm.h
libappinfocl_la_CFLAGS = $(SAMPLER_CFLAGS) \
			 -I$(top_srcdir)/ldms/src/sampler/ldms_jobid
libappinfocl_la_LIBADD = $(SAMPLER_LIBADD)
libappinfocl_la_LDFLAGS = $(SAMPLER_LDFLAGS)
pkglib_LTLIBRARIES += libappinfocl.la
