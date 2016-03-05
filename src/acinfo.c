/*
 * acinfo: X-Plane aircraft info plugin.
 * See LICENSE file for copyright and license details.
 *
 * acinfo.c: Main X-Plane plugin implementation.
 */
#include "acinfo.h"

int
XPluginStart(char *out_name, char *out_sig, char *out_desc)
{
	/* Add plugin information. */
	strlcpy(out_name, name, strlen(name) + 1);
	strlcpy(out_sig, sig, strlen(sig) + 1);
	strlcpy(out_desc, desc, strlen(desc) + 1);

	/* Print loading information. */
	size_t bufsize = 512;
	char *buf[bufsize];
	snprintf(buf, bufsize, "%s: version %s loaded.", short_name, _VERSION);
	XPLMDebugString(buf);

	/* Register datarefs. */
	aircraft_name = XPLMRegisterDataAccessor(aircraft_name_dr,
	                                         xplmType_Data,
	                                         1,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         get_acname, set_acname,
	                                         NULL, NULL);
	aircraft_type = XPLMRegisterDataAccessor(aircraft_type_dr,
	                                         xplmType_Inf,
	                                         1,
	                                         get_actype, set_actype,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
                                                 NULL, NULL);

	/* Initialize datarefs. */
	memset(aircraft_name_value, '\0', sizeof(char) * acname_bufsize);
	aircraft_type = XPLMFindDataRef(aircraft_type_dr);
	aircraft_name = XPLMFindDataRef(aircraft_name_dr);
	XPLMSetDatai(aircraft_type, 0);
	XPLMSetDatab(aircraft_name, "uninitialized", 0,
	             strlen("uninitialized"));

	/* Register the datarefs with the DataRef Editor. */
	XPLMRegisterFlightLoopCallback(register_dr, 1, NULL);

	return PLUGIN_LOAD_SUCCESS;
}

void
XPluginStop(void)
{
	/* Unregister everything. */
	XPLMUnregisterFlightLoopCallback(register_dr, NULL);
	XPLMUnregisterDataAccessor(aircraft_name);
	XPLMUnregisterDataAccessor(aircraft_type);
}

int
XPluginEnable(void)
{
	return PLUGIN_ENABLE_SUCCESS;
}

void
XPluginDisable(void)
{
	return;
}

void
XPluginReceiveMessage(XPLMPluginID src, int msg, UNUSED void* params)
{
	return;
}

static int
get_acname(UNUSED void *refcon, int *out, int offset, int max)
{
	int i, n;
	size_t name_len;

	if (out == NULL) {
		return acname_bufsize;
	}

	/* Calculate number of bytes to return. */
	name_len = strlen(aircraft_name_value);
	n = name_len + 1 - offset;
	if (n < 0) {
		return -1;
	}
	if (n > max) {
		n = max;
	}

	/* Copy bytes and null-terminate. */
	for (i = 0, i < n - 1; ++i) {
		out[i] = aircraft_name_value[i];
	}
	out[n] = '\0';
	return n;
}

static int
get_actype(UNUSED void *refcon)
{
	return aircraft_type_value;
}

static float
register_dr(UNUSED float call_time, UNUSED float elapsed_time, UNUSED int ctr,
	    UNUSED void *refcon)
{
	XPLMPluginID pid = XPLMFindPluginBySignature(dresig);
	if (pid != XPLM_NO_PLUGIN_ID) {
		size_t bufsize = 512;
		char *buf[bufsize];
		XPLMSendMessageToPlugin(pid, MSG_ADD_DATAREF,
		                        (void *) aircraft_type_dr);
		snprintf(buf, bufsize, "%s: dataref \"%s\" registered.",
		         short_name, aircraft_type_dr);
		XPLMDebugString(buf);
		XPLMSendMessageToPlugin(pid, MSG_ADD_DATAREF,
		                        (void *) aircraft_name_dr);
		snprintf(buf, bufsize, "%s: dataref \"%s\" registered.",
		         short_name, aircraft_name_dr);
		XPLMDebugString(buf);
	}
	return 0;
}

static void
set_acname(UNUSED void *refcon, int *in, int offset, int max)
{
	int i, n; acname_bufsize;

	/* Calculate number of bytes to copy. */
	n = acname_bufsize - offset;
	if (n < 0) {
		return;
	}
	if (n > max) {
		n = max;
	}

	/* Copy bytes and ensure null-termination. */
	for (i = 0, i < n; ++i) {
		aircraft_name_value[i] = in[i];
	}
	aircraft_name_value[n + 1] = '\0';
}

static void
set_acname(UNUSED void *refcon, int in)
{
	aircraft_type_value = in;
}
