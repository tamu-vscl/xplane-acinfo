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
	char buf[DEFAULT_BUFSIZE];

	/* Add plugin information. */
	strlcpy(out_name, name, strlen(name) + 1);
	strlcpy(out_sig, sig, strlen(sig) + 1);
	strlcpy(out_desc, desc, strlen(desc) + 1);

	/*
	 * Print loading information.
	 * Return value is casted to void since we don't really care about
	 * truncation; indeed the default bufsize should always be larger than
	 * the input string.
	 */
	(void) snprintf(buf, DEFAULT_BUFSIZE, "%s: version %s loaded.\n",
	                short_name, _VERSION);
	XPLMDebugString(buf);

	/* Register datarefs. */
	/*@-nullpass@*/
	aircraft_name = XPLMRegisterDataAccessor(aircraft_name_dr,
	                                         xplmType_Data,
	                                         XPDREF_WRITABLE,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         get_acname, set_acname,
	                                         NULL, NULL);
	aircraft_type = XPLMRegisterDataAccessor(aircraft_type_dr,
	                                         xplmType_Int,
	                                         XPDREF_WRITABLE,
	                                         get_actype, set_actype,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL,
	                                         NULL, NULL);
	/*@=nullpass@*/

	/* Initialize datarefs. */
	memset(aircraft_name_value, 0, sizeof(char) * ACNAME_BUFSIZE);
	strlcpy(aircraft_name_value, default_name, strlen(default_name) + 1);
	aircraft_type = XPLMFindDataRef(aircraft_type_dr);
	aircraft_name = XPLMFindDataRef(aircraft_name_dr);

	/* Register the datarefs with the DataRef Editor. */
	/*@-nullpass@*/
	XPLMRegisterFlightLoopCallback(register_dr, 1, NULL);
	/*@=nullpass@*/

	return PLUGIN_LOAD_SUCCESS;
}

void
XPluginStop(void)
{
	/* Unregister everything. */
	/*@-nullpass@*/
	XPLMUnregisterFlightLoopCallback(register_dr, NULL);
	XPLMUnregisterDataAccessor(aircraft_name);
	XPLMUnregisterDataAccessor(aircraft_type);
	/*@=nullpass@*/
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
XPluginReceiveMessage(/*@unused@*/ UNUSED XPLMPluginID src,
                      /*@unused@*/ UNUSED int msg,
                      /*@unused@*/ UNUSED void* params)
{
	return;
}

static int
get_acname(/*@unused@*/ UNUSED void *refcon, void *out, int offset, int max)
{
	char tmp[ACNAME_BUFSIZE];
	size_t maxsz, n, name_len;

	/* If no output pointer is given, return the array buffer size. */
	if (out == NULL) {
		return ACNAME_BUFSIZE;
	}

	/* Make the max size a size_t. */
	maxsz = (size_t) max;

	/* Calculate number of bytes to return. */
	name_len = strlen(aircraft_name_value);
	n = name_len + 1 - offset;
	if (n > maxsz) {
		n = maxsz;
	}

	/* Copy bytes and null-terminate. */
	strlcpy(tmp, aircraft_name_value, n);
	memcpy(out, (void *) &tmp, n);
	return (int) n;
}

static int
get_actype(/*@unused@*/ UNUSED void *refcon)
{
	return aircraft_type_value;
}

static float
register_dr(/*@unused@*/ UNUSED float call_time,
            /*@unused@*/ UNUSED float elapsed_time,
            /*@unused@*/ UNUSED int ctr,
            /*@unused@*/ UNUSED void *refcon)
{
	XPLMPluginID pid = XPLMFindPluginBySignature(dresig);
	if (pid != XPLM_NO_PLUGIN_ID) {
		char buf[DEFAULT_BUFSIZE];
		XPLMSendMessageToPlugin(pid, MSG_ADD_DATAREF,
		                        (void *) aircraft_type_dr);
		(void) snprintf(buf, DEFAULT_BUFSIZE,
			        "%s: dataref \"%s\" registered.\n",
		                short_name, aircraft_type_dr);
		XPLMDebugString(buf);
		XPLMSendMessageToPlugin(pid, MSG_ADD_DATAREF,
		                        (void *) aircraft_name_dr);
		(void) snprintf(buf, DEFAULT_BUFSIZE,
		                "%s: dataref \"%s\" registered.\n",
		                short_name, aircraft_name_dr);
		XPLMDebugString(buf);
	}
	return 0;
}

static void
set_acname(/*@unused@*/ UNUSED void *refcon, void *in, int offset, int max)
{
	size_t maxsz, n;

	/* Make the max size a size_t. */
	maxsz = (size_t) max;

	/* Calculate number of bytes to copy. */
	n = ACNAME_BUFSIZE - (size_t) offset;
	if (n > maxsz) {
		n = maxsz;
	}

	/* Copy bytes and ensure null-termination. */
	memcpy(aircraft_name_value, (char *) in, n - 1);
	aircraft_name_value[n] = '\0';
}

static void
set_actype(/*@unused@*/ UNUSED void *refcon, int in)
{
	aircraft_type_value = in;
}
