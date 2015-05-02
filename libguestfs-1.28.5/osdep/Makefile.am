# libguestfs-mingw-port

include $(top_srcdir)/subdir-rules.mk

noinst_LTLIBRARIES = libosdep.la

libosdep_la_SOURCES = \
    os-error.h \
    os-fcntl.h \
    os-pipe.h \
    os-process.h \
    os-shmem.h \
    os-socket.h \
    os-socketpair.h \
    os-fcntl.c \
    os-pipe.c \
    os-process.c \
    os-shmem.c \
    os-socket.c \
    os-socketpair.c

if WINDOWS
libosdep_la_CPPFLAGS = \
    -DWINVER=0x0501 \
    -I../winport
endif

libosdep_la_CFLAGS = \
    $(WARN_CFLAGS) $(WERROR_CFLAGS) \
    $(GCC_VISIBILITY_HIDDEN) 

if WINDOWS
libosdep_la_LIBADD = \
    ../winport/libwinport.la \
    -lws2_32 -lwsock32
endif

if LINUX
libosdep_la_LIBADD = \
    -lrt
endif

