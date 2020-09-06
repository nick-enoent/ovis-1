ACLOCAL_AMFLAGS = -I m4
# unlike default automake, we build this dir's local pieces before its children.
SUBDIRS = .
lib_LTLIBRARIES =
pkglib_LTLIBRARIES =

AM_CPPFLAGS = @OVIS_INCLUDE_ABS@
AM_LDFLAGS = @OVIS_LIB_ABS@

CORE_LIBADD = $(top_builddir)/ldms/src/core/libldms.la \
	      @LDFLAGS_GETTIME@ \
	      $(top_builddir)/lib/src/ovis_util/libovis_util.la
COMMON_LIBADD = libsampler_base.la $(CORE_LIBADD) \
		$(top_builddir)/lib/src/coll/libcoll.la

libsampler_base_la_SOURCES = sampler_base.c sampler_base.h
libsampler_base_la_LIBADD = $(CORE_LIBADD)
lib_LTLIBRARIES += libsampler_base.la

PAPI_LDFLAGS = @LIBPAPI_LIB64DIR_FLAG@ @LIBPAPI_LIBDIR_FLAG@ \
             @LIBPFM_LIB64DIR_FLAG@ @LIBPFM_LIBDIR_FLAG@

ldmssamplerincludedir = $(includedir)/ldms/sampler
ldmssamplerinclude_HEADERS = sampler_base.h

if ENABLE_IBNET
SUBDIRS += ibnet
endif

if ENABLE_JOBID
SUBDIRS += ldms_jobid
endif

if ENABLE_CRAY_POWER_SAMPLER
SUBDIRS += cray_power_sampler
endif

if ENABLE_KGNILND
SUBDIRS += kgnilnd
endif

if ENABLE_VMSTAT
SUBDIRS += vmstat
endif
if ENABLE_LOADAVG
SUBDIRS += loadavg
endif

if ENABLE_FPTRANS
SUBDIRS += fptrans
endif

# always include examples
SUBDIRS += examples

if ENABLE_VARSET
SUBDIRS += variable
endif

if ENABLE_LLNL
SUBDIRS += llnl
endif

if ENABLE_JOBINFO
SUBDIRS += job_info
endif

if ENABLE_SLURM_JOBINFO
SUBDIRS += job_info_slurm
endif

if ENABLE_LUSTRE
SUBDIRS += lustre
endif

if ENABLE_PAPI_SAMPLER
SUBDIRS += papi
endif

if ENABLE_SLURM_SAMPLER
SUBDIRS += slurm
endif

if ENABLE_SPANK_PLUGIN
SUBDIRS += spank
endif

if ENABLE_IBM_OCC_SAMPLER
SUBDIRS += ibm_occ
endif

if ENABLE_CRAY_SYSTEM_SAMPLER
SUBDIRS += cray_system_sampler
endif

if ENABLE_ARIES_MMR
SUBDIRS += aries_mmr
endif

if ENABLE_LNET_STATS
SUBDIRS += lnet_stats
endif

if ENABLE_MEMINFO
SUBDIRS += meminfo
endif

if ENABLE_MSR_INTERLAGOS
SUBDIRS += msr_interlagos
endif

if ENABLE_SWITCHX
SUBDIRS += switchx
endif

if ENABLE_PERFEVENT
SUBDIRS += perfevent
endif

if ENABLE_HWEVENTPAPI
libhweventpapi_la_SOURCES = hweventpapi.c
libhweventpapi_la_CFLAGS = $(AM_CFLAGS) $(LIBPAPI_INCDIR_FLAG)
libhweventpapi_la_LDFLAGS = $(AM_LDFLAGS) $(PAPI_LDFLAGS)
libhweventpapi_la_LIBADD = $(COMMON_LIBADD) $(JOBID_LIBFLAGS) -lpapi -lpfm -lm
pkglib_LTLIBRARIES += libhweventpapi.la
endif

if ENABLE_APPINFO
SUBDIRS += appinfo_lib
endif

if ENABLE_MPI_SAMPLER
SUBDIRS += shm
endif

if ENABLE_RAPL
librapl_la_SOURCES = rapl.c
librapl_la_CFLAGS = $(AM_CFLAGS) $(LIBPAPI_INCDIR_FLAG)
librapl_la_LDFLAGS = $(AM_LDFLAGS) $(PAPI_LDFLAGS)
librapl_la_LIBADD = $(COMMON_LIBADD) -lpapi -lpfm -lm
pkglib_LTLIBRARIES += librapl.la
endif

if ENABLE_PROCDISKSTATS
SUBDIRS += procdiskstats
endif

if ENABLE_PROCINTERRUPTS
SUBDIRS += procinterrupts
endif

if ENABLE_PROCNETDEV
SUBDIRS += procnetdev
endif

if ENABLE_PROCNFS
SUBDIRS += procnfs
endif

if ENABLE_FILESINGLE
SUBDIRS += filesingle
endif

if ENABLE_PROCSTAT
SUBDIRS += procstat
endif

if ENABLE_DSTAT
SUBDIRS += dstat
endif

if ENABLE_SYSCLASSIB
SUBDIRS += sysclassib
endif

if ENABLE_OPA2
OPA2_LIBADD = -libmad -libumad -lpthread
libopa2_la_SOURCES = opa2.c
OPAINC=/usr/include/infiniband
libopa2_la_CFLAGS = $(AM_CFLAGS) -I$(OPAINC)
libopa2_la_LIBADD = $(AM_LDFLAGS) $(COMMON_LIBADD) $(OPA2_LIBADD) $(JOBID_LIBFLAGS)
pkglib_LTLIBRARIES += libopa2.la
endif

if ENABLE_ATASMART
SUBDIRS += sampler_atasmart
endif

if ENABLE_GENERIC_SAMPLER
SUBDIRS += generic_sampler
endif

if ENABLE_LDMS_TEST
SUBDIRS += test-samplers
endif

if ENABLE_SYSPAPI_SAMPLER
SUBDIRS += syspapi
endif

if ENABLE_APP_SAMPLER
SUBDIRS += app_sampler
endif

if ENABLE_HELLO_STREAM
SUBDIRS += hello_stream
endif
