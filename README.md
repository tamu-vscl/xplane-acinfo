# X-Plane Aircraft Information Plugin

## About

The Aircraft Information (ACInfo) plugin aims to make it easy to 
identify the name and type of an X-Plane aircraft for other applications
and plugins by providing custom datarefs that encode this information.

### DataRefs

ACInfo provides the following DataRefs:

*   `vscl/sim/aircraft/name`: the aircraft name as a string (`char *`).
*   `vscl/sim/aircraft/type`: the aircraft type as an `int64_t` bitmask.

These can be requested using the X-Plane SDK or the new DREF UDP 
interface.

### Types

Types are a generic representation of what an aircraft is.
They include powerplant type and aircraft role.
Since these can be combined, types are represented as a bitmask, so e.g.
a jet fighter would be represented as `JET ^ FIGHTER`.
Refer to the [`lua/acinfo.lua`](lua/acinfo.lua) script for the most
up-to-date type definitions.

### Algorithms

(Well, they're sort of algorithms...)
ACInfo uses the aircraft's ICAO name (`sim/aircraft/view/acf_ICAO`) if
available as the aircraft name.
If this is known to be empty or bad, the aircraft's filename is used.
In some cases, this is then mapped to a custom name.

To determine engine type, the plugin looks at engine DataRefs specific 
to piston-engined aircraft and sees if they are nonzero.
If prop RPM and manifold pressure are both nonzero, the aircraft is 
determined to be piston-engined, and if prop RPM only is nonzero, the
aircraft is a turboprop.
Otherwise, it is considered a jet (inclusive of turbojets and 
turbofans).
For the aircraft role, the aircraft's path is iterated over to find the
common X-Plane subdirectories, which are then mapped to a type integer.
In certain cases this is overriden with known types.

## Requirements

ACInfo implements most of its core logic in the Lua programming 
language.
Starting with X-Plane 10.20, Lua is embedded into X-Plane and can be 
used to script the simulator.
ACInfo uses the [FlyWithLua](https://github.com/X-Friese/FlyWithLua)
plugin to provide a convenient API to interact with X-Plane.
Accordingly, FlyWithLua must be installed for X-Plane.

For the C plugin, the 
[X-Plane SDK](http://www.xsquawkbox.net/xpsdk/mediawiki/Main_Page)
is required.
The SDK is included as a submodule with the path `ext/sdk`, and can
be instantiated with
```
git submodule update --init
```
which will initialize and update all of the submodules.

Additionally, `libbsd` is required as the plugin makes use of 
`strlcpy(3)`.

## Building the C Plugin

ACInfo uses a traditional Makefile. 
Compile flags are set in [`config.mk`](config.mk), and rules are placed
in [`src/acinfo.mk`](src/acinfo.mk).
The actual Makefile contains no user-editable code.

To build the plugin, `cd` into the repository's directory and issue
```
make -j2
```
which will output the plugin to `lib/libacinfo.so`.
This file will be symlinked to [`64/lin.xpl`](64/lin.xpl) for use as an
X-Plane "fat plugin".
To remove leftover object files, if present, run
```
make clean
```
or 
```
make distclean
```
to remove all object files, including the plugin.

## Installing the Plugin 
### C Plugin 
Place the plugin into the `Resources/plugins` directory of your X-Plane
installation with the following structure:
```
acinfo/
├── 64
│   └── lin.xpl
└── lib
    └── libacinfo.so
```

### Lua Scripts
To install the Lua script(s), place them into the `Scripts` directory
of the main FlyWithLua plugin directory.

## License
See the [`LICENSE.md`](LICENSE.md) file for license and copyright 
information.
