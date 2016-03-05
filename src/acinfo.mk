ALL_TARGETS += ./lib/libacinfo.so
CLEAN_TARGETS += clean-plugin
DISTCLEAN_TARGETS += distclean-plugin

SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)
OBJECTS := $(SOURCES:.c=.o)

src/%.o: src/%.c $(HEADERS) $(XPSDK_HEADERS)
    echo "[acinfo] CC $<"
    $(CC)  -Iinclude $(CPPFLAGS) $(XPSDK_CPPFLAGS) $(CFLAGS) \
$(INCLUDE) $(XPSDK_INCLUDE) -o $@ -c $<

./lib/libacinfo.so: $(OBJECTS)
    echo "[acinfo] Link acinfo"
    $(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS) $(LIBDIRS)

clean-plugin:
    echo "[acinfo] Clean"
    rm -f $(OBJECTS)

distclean-plugin: clean-plugin
    echo "[acinfo] Distclean"
    rm -f simlink-ng.xpl
