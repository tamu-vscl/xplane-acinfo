# Figure out command.
GOALS := $(MAKECMDGOALS)
ifeq ($(MAKECMDGOALS), )
GOALS += "ALL"
endif

# Get first 7 characters in current git commit.
GITCOMMIT=$(shell git log -n 1 --pretty=format:"%H" | cut -c1-7)
ifneq ($(filter-out clean,${GOALS}),)
$(info [acinfo] using version ${GITCOMMIT})
endif

# Compiler
CC = gcc

# Preprocessor flags.
CPPFLAGS  = -D_DEFAULT_SOURCE
CPPFLAGS += -D_POSIX_C_SOURCE=2
CPPFLAGS += -D_FORTIFY_SOURCE=2
# Version.
CPPFLAGS += -D_VERSION="\"git-$(GITCOMMIT)-build-$(shell whoami)-$(shell date -u +%Y%m%dT%H%MZ)\""

# Compiler flags.
CFLAGS += -pedantic
# Enable most compiler warnings.
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wfloat-equal
CFLAGS += -Werror
CFLAGS += -Wundef
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wstrict-overflow=5
CFLAGS += -Wwrite-strings
CFLAGS += -Waggregate-return
# CFLAGS += -Wcast-qual  # j-harris: disabled due to X-Plane SDK behavior.
CFLAGS += -Wswitch-default
CFLAGS += -Wswitch-enum
CFLAGS += -Wunreachable-code
CFLAGS += -Wno-deprecated-declarations
# Other options
CFLAGS += -fPIC
CFLAGS += -fstack-protector
CFLAGS += -funwind-tables
CFLAGS += -fasynchronous-unwind-tables
CFLAGS += -fmessage-length=0
CFLAGS += -O2
CFLAGS += -std=c99
CFLAGS += -m64

# Linker flags for plugin.
LDFLAGS = -shared -rdynamic -nodefaultlibs -undefined_warning -m64

# Include directories.
INCLUDE  = -I/usr/include -I/usr/local/include

# Libraries to link.
LIBS = -lm -lGL -lGLU -lbsd
LIBDIRS = -L/usr/lib64 -L/usr/local/lib64 -L/lib64

# X-Plane SDK stuff.
XPSDK_CPPFLAGS  = -DXPLM200
XPSDK_CPPFLAGS += -DXPLM210
XPSDK_CPPFLAGS += -DXPLM=1
XPSDK_CPPFLAGS += -DIBM=0
XPSDK_CPPFLAGS += -DAPL=0
XPSDK_CPPFLAGS += -DLIN=1
XPSDK_INCLUDE = -I./ext/sdk/CHeaders/XPLM

# Don’t print command lines which are run
.SILENT:

.PHONY: clean distclean
