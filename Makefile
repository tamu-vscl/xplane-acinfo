ALL_TARGETS =
CLEAN_TARGETS =
DISTCLEAN_TARGETS =

all: real-all

include config.mk
include src/acinfo.mk

real-all: $(ALL_TARGETS)

clean: $(CLEAN_TARGETS)

distclean: $(DISTCLEAN_TARGETS)
