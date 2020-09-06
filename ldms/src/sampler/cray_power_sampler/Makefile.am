include ../common.am

libtsampler_la_SOURCES = tsampler.c tsampler.h
libtsampler_la_CFLAGS = $(SAMPLER_CFLAGS)
libtsampler_la_LIBADD = $(SAMPLER_LIBADD) \
			$(top_builddir)/lib/src/ovis_event/libovis_event.la -lpthread
libtsampler_la_LDFLAGS = $(SAMPLER_LDFLAGS)

libtimer_base_la_CFLAGS = $(SAMPLER_CFLAGS) -I$(top_srcdir)/ldms/src/sampler/ldms_jobid
libtimer_base_la_SOURCES = timer_base.c timer_base.h
libtimer_base_la_LIBADD = $(SAMPLER_LIBADD) libtsampler.la
libtimer_base_la_LDFLAGS = $(SAMPLER_LDFLAGS) -DENABLE_JOBID

libhfclock_la_SOURCES = hfclock.c
libhfclock_la_CFLAGS  = $(SAMPLER_CFLAGS)
libhfclock_la_LIBADD  = $(SAMPLER_LIBADD) libtimer_base.la
libhfclock_la_LDFLAGS = $(SAMPLER_LDFLAGS)

libcray_power_sampler_la_SOURCES = cray_power_sampler.c
libcray_power_sampler_la_CFLAGS = $(SAMPLER_CFLAGS) $(AM_CFLAGS)
libcray_power_sampler_la_LIBADD = $(SAMPLER_LIBADD) libtimer_base.la
libcray_power_sampler_la_LDFLAGS = $(SAMPLER_LDFLAGS)

pkglib_LTLIBRARIES = libtsampler.la libtimer_base.la libhfclock.la libcray_power_sampler.la
