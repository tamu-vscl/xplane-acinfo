/*
 * acinfo: X-Plane aircraft info plugin.
 * See LICENSE file for copyright and license details.
 *
 * acinfo.h: Main X-Plane plugin.
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

/*
 * Define the unused attribute to suppress warnings about unused parameters in
 * functions the X-Plane SDK requires but this plugin does not use.
 */
#if (defined __GNUC__ || defined __GNUG__ || defined __clang__)
#	define UNUSED __attribute__((__unused__))
#else
#	define UNUSED
#endif

/* Define constants to avoid X-Plane SDK magic numbers. */
const int PLUGIN_LOAD_SUCCESS = 1;
const int PLUGIN_LOAD_FAILED = 0;
const int PLUGIN_ENABLE_SUCCESS = 1;
const int PLUGIN_ENABLE_FAILED = 0;

/* Plugin description. */
const char *name = "Aircraft Information Plugin";
const char *short_name = "acinfo";
const char *sig = "edu.tamu.vscl.acinfo";
const char *desc = "Provides custom datarefs describing player aircraft.";

/* Other plugin signatures. */
const char *dresig = "xplanesdk.examples.DataRefEditor";

/* Custom datarefs. */
const char *aircraft_type_dr = "vscl/sim/aircraft/type";
const char *aircraft_name_dr = "vscl/sim/aircraft/name";
XPLMDataRef aircraft_type = NULL;
XPLMDataRef aircraft_name = NULL;

/* Values to hold the data. */
int16_t aircraft_type_value = 0;
size_t acname_bufsize = 255;
char aircraft_name_value[acname_bufsize];

/* Getters/setters for datarefs. */
static int get_acname(UNUSED void *, int *, int, int);
static int get_actype(UNUSED void *);
static void set_acname(UNUSED void *, int *, int, int);
static void set_acname(UNUSED void *, int);

/* Deferred initialization loop for registering with DataRef Editor. */
static float register_dr(UNUSED float, UNUSED float, UNUSED int, UNUSED void *);