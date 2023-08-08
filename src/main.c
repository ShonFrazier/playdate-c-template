#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pd_api.h"

#ifdef _WINDLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static int update(void* userdata);

EXPORT int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't
		// run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}

	return 0;
}

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;

	// return a non-zero number to tell the system to update the display, or zero if update isn’t needed.
	return 1;
}

