# Makefile
#
# Copyright (C) 2003 Christophe Varoqui, <christophe.varoqui@opensvc.com>
#
include ../Makefile.inc

SONAME=0
DEVLIB = libmultipath.so
LIBS = $(DEVLIB).$(SONAME)
CFLAGS += -I$(mpathcmddir)
LIBDEPS = -lpthread -ldl -ldevmapper -ludev -L$(mpathcmddir) -lmpathcmd
ifdef SYSTEMD
	ifeq ($(shell test $(SYSTEMD) -gt 209 && echo 1), 1)
		LIBDEPS += -lsystemd
	else
		LIBDEPS += -lsystemd-daemon
	endif
endif

OBJS = memory.o parser.o vector.o devmapper.o callout.o \
       hwtable.o blacklist.o util.o dmparser.o config.o \
       structs.o discovery.o propsel.o dict.o \
       pgpolicies.o debug.o defaults.o uevent.o \
       switchgroup.o uxsock.o print.o alias.o log_pthread.o \
       log.o configure.o structs_vec.o sysfs.o prio.o checkers.o \
       lock.o waiter.o file.o wwids.o prioritizers/alua_rtpg.o

LIBDM_API_FLUSH = $(shell grep -Ecs '^[a-z]*[[:space:]]+dm_task_no_flush' /usr/include/libdevmapper.h)

ifneq ($(strip $(LIBDM_API_FLUSH)),0)
	CFLAGS += -DLIBDM_API_FLUSH -D_GNU_SOURCE
endif

LIBDM_API_COOKIE = $(shell grep -Ecs '^[a-z]*[[:space:]]+dm_task_set_cookie' /usr/include/libdevmapper.h)

ifneq ($(strip $(LIBDM_API_COOKIE)),0)
	CFLAGS += -DLIBDM_API_COOKIE
endif

LIBUDEV_API_RECVBUF = $(shell grep -Ecs '^[a-z]*[[:space:]]+udev_monitor_set_receive_buffer_size' /usr/include/libudev.h)

ifneq ($(strip $(LIBUDEV_API_RECVBUF)),0)
	CFLAGS += -DLIBUDEV_API_RECVBUF
endif

ifdef SYSTEMD
	CFLAGS += -DUSE_SYSTEMD=$(SYSTEMD)
endif

LIBDM_API_DEFERRED = $(shell grep -Ecs '^[a-z]*[[:space:]]+dm_task_deferred_remove' /usr/include/libdevmapper.h)

ifneq ($(strip $(LIBDM_API_DEFERRED)),0)
	CFLAGS += -DLIBDM_API_DEFERRED
endif

all: $(LIBS)

$(LIBS): $(OBJS)
	$(CC) $(LDFLAGS) $(SHARED_FLAGS) -Wl,-soname=$@ $(CFLAGS) -o $@ $(OBJS) $(LIBDEPS)
	$(LN) $@ $(DEVLIB)

install:
	$(INSTALL_PROGRAM) -d $(DESTDIR)$(syslibdir)
	$(INSTALL_PROGRAM) -m 755 $(LIBS) $(DESTDIR)$(syslibdir)/$(LIBS)
	$(INSTALL_PROGRAM) -m 755 -d $(DESTDIR)$(libdir)
	$(LN) $(LIBS) $(DESTDIR)$(syslibdir)/$(DEVLIB)

uninstall:
	$(RM) $(DESTDIR)$(syslibdir)/$(LIBS)
	$(RM) $(DESTDIR)$(syslibdir)/$(DEVLIB)

clean:
	$(RM) core *.a *.o *.gz *.so *.so.*
