/*
 * acinfo: X-Plane aircraft info plugin.
 * See LICENSE file for copyright and license details.
 *
 * acinfo.h: Main X-Plane plugin.
 */
#pragma once

#include <bsd/string.h>
#include <stdio.h>
#include <string.h>

#include "XPLMDataAccess.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
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

/* Array sizes. */
#define DEFAULT_BUFSIZE 512
#define ACNAME_BUFSIZE 1024

/* Constants for DRE. */
#define XPDREF_WRITABLE 1
#define MSG_ADD_DATAREF 0x01000000

/* Define constants to avoid X-Plane SDK magic numbers. */
#define PLUGIN_LOAD_SUCCESS 1
#define PLUGIN_ENABLE_SUCCESS 1

/* Plugin description. */
static const char *name = "Aircraft Information Plugin";
static const char *short_name = "acinfo";
static const char *sig = "edu.tamu.vscl.acinfo";
static const char *desc = "Provides custom datarefs describing aircraft.";

/* Other plugin signatures. */
static const char *dresig = "xplanesdk.examples.DataRefEditor";

/* Custom datarefs. */
static const char *aircraft_type_dr = "vscl/sim/aircraft/type";
static const char *aircraft_name_dr = "vscl/sim/aircraft/name";
/*@null@*/ static XPLMDataRef aircraft_type = NULL;
/*@null@*/ static XPLMDataRef aircraft_name = NULL;

/* Values to hold the data. */
static int aircraft_type_value = 0;
/*@unique@*/ static char aircraft_name_value[ACNAME_BUFSIZE];
static const char *default_name = "unknown";

/* Getters/setters for datarefs. */
static int get_acname(UNUSED void *, void *, int, int);
static int get_actype(UNUSED void *);
static void set_acname(UNUSED void *, void *, int, int);
static void set_actype(UNUSED void *, int);

/* Deferred initialization loop for registering with DataRef Editor. */
static float register_dr(UNUSED float, UNUSED float, UNUSED int, UNUSED void *);
