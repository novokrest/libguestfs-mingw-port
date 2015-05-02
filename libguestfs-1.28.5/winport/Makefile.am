# libguestfs-mingw-port

include $(top_srcdir)/subdir-rules.mk

noinst_LTLIBRARIES = libwinport.la

libwinport_la_SOURCES = \
    win-port.h \
    win-error.c \
    win-getuid.c \
    win-mem.c \
    win-realpath.c \
    win-strndup.c \
    win-symlink.c \
    win-vaprintf.c

libwinport_la_CPPFLAGS = \
   -DWINVER=0x0501

libwinport_la_CFLAGS = \
    $(WARN_CFLAGS) $(WERROR_CFLAGS) \
    $(GCC_VISIBILITY_HIDDEN)

libwinport_la_LIBADD = \
    -lws2_32 -lwsock32
