// SONIC ROBO BLAST 2 - DSi Port
//-----------------------------------------------------------------------------
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
// Copyright (C) 1999-2016 by Sonic Team Junior.
// DSi Port by catzdsii - Based on the 3DS port
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  i_system.c
/// \brief System interface for DSi

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../doomdef.h"
#include "../i_system.h"
#include "../z_zone.h"
#include "../m_argv.h"
#include "../d_main.h"

extern volatile int vblankCount;

tic_t I_GetTime(void)
{
	// Get time in ticks (TICRATE ticks per second)
	static u32 startTime = 0;
	u32 now = (vblankCount * 1000) / 60;
	if (startTime == 0)
		startTime = now;
	return (tic_t)((now - startTime) * TICRATE / 1000);
}

void I_GetTime_Scaled(tic_t *tic, tic_t *frac)
{
	tic_t time = I_GetTime();
	*tic = time;
	*frac = 0;
}

void I_Sleep(void)
{
	swiWaitForVBlank();
}

INT32 I_StartupSystem(void)
{
	// System already initialized in main()
	CONS_Printf("DSi system initialized.\n");
	return 0;
}

void I_ShutdownSystem(void)
{
	// Cleanup if needed
}

void I_Error(const char *error, ...)
{
	char errormsg[256];
	va_list argptr;
	
	va_start(argptr, error);
	vsnprintf(errormsg, sizeof(errormsg), error, argptr);
	va_end(argptr);
	
	consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 1, 0, false, true);
	iprintf("FATAL ERROR:\n");
	iprintf("%s\n", errormsg);
	iprintf("\nPress START to quit.\n");
	
	while(1)
	{
		scanKeys();
		if (keysDown() & KEY_START)
			break;
		swiWaitForVBlank();
	}
	
	exit(1);
}

void I_Quit(void)
{
	I_ShutdownSystem();
	exit(0);
}

void I_OutputMsg(const char *error, ...)
{
	char msg[256];
	va_list argptr;
	
	va_start(argptr, error);
	vsnprintf(msg, sizeof(msg), error, argptr);
	va_end(argptr);
	
	// Output to console
	iprintf("%s", msg);
}

void I_StartupTimer(void)
{
	// Timer already initialized
}

void I_ShutdownTimer(void)
{
	// Cleanup if needed
}

void I_StartupNetwork(void)
{
	// Network not supported on DSi
}

void I_ShutdownNetwork(void)
{
	// Network not supported on DSi
}

void I_StartupSound(void)
{
	// Sound initialization in i_sound.c
}

void I_ShutdownSound(void)
{
	// Sound cleanup in i_sound.c
}

void I_StartupGraphics(void)
{
	// Graphics initialization in i_video.c
}

void I_ShutdownGraphics(void)
{
	// Graphics cleanup in i_video.c
}

void I_GetEvent(void)
{
	// Event handling in g_input.c
}

void I_OsPolling(void)
{
	// Polling handled elsewhere
}

void I_UpdateNoBlit(void)
{
	// Not needed for DSi
}

void I_FinishUpdate(void)
{
	// Update handled in i_video.c
}

void I_ReadScreen(UINT8 *scr)
{
	// Screen reading not implemented
	(void)scr;
}

void I_BeginRead(void)
{
	// Not needed
}

void I_EndRead(void)
{
	// Not needed
}

void I_StartupMouse(void)
{
	// Mouse not supported on DSi
}

void I_ShutdownMouse(void)
{
	// Mouse not supported on DSi
}

void I_StartupJoystick(void)
{
	// Joystick initialization in g_input.c
}

void I_ShutdownJoystick(void)
{
	// Joystick cleanup in g_input.c
}

void I_StartupKeyboard(void)
{
	// Keyboard not supported on DSi
}

void I_ShutdownKeyboard(void)
{
	// Keyboard not supported on DSi
}

void I_GetRandomBytes(void *bytes, size_t size)
{
	// Use system random
	for (size_t i = 0; i < size; i++)
	{
		((UINT8 *)bytes)[i] = (UINT8)(rand() & 0xFF);
	}
}

void I_GetMemFree(size_t *freebytes)
{
	// Return approximate free memory
	// DSi has 16MB total, but we can't easily query free memory
	// This is a rough estimate
	*freebytes = 4 * 1024 * 1024; // Assume 4MB free
}

void I_DisplayType(void)
{
	// DSi XL specific
	CONS_Printf("DSi XL - 133MHz ARM9, 16MB RAM\n");
}

