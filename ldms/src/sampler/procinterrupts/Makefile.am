pkglib_LTLIBRARIES =
lib_LTLIBRARIES =
dist_man7_MANS =
dist_man1_MANS =


AM_CPPFLAGS = @OVIS_INCLUDE_ABS@
AM_LDFLAGS = @OVIS_LIB_ABS@

if ENABLE_PROCINTERRUPTS
libprocinterrupts_la_SOURCES = procinterrupts.c
libprocinterrupts_la_CFLAGS = $(SAMPLER_CFLAGS)
libprocinterrupts_la_LIBADD = $(SAMPLER_LIBADD)
libprocinterrupts_la_LDFLAGS = $(SAMPLER_LDFLAGS)
pkglib_LTLIBRARIES += libprocinterrupts.la
dist_man7_MANS += Plugin_procinterrupts.man
endif


