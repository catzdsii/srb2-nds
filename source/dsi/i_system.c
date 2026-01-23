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

volatile int vblankCount = 0;

void VblankHandler(void) {
	vblankCount++;
}

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
	irqSet(IRQ_VBLANK, VblankHandler);
	irqEnable(IRQ_VBLANK);
	
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
	
	// Ensure Sub Screen is in text mode
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG);
	consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
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

/*
void I_StartupNetwork(void)
{
	// Network not supported on DSi
}

void I_ShutdownNetwork(void)
{
	// Network not supported on DSi
}
*/

/*
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
*/

void I_GetEvent(void)
{
	// Event handling in g_input.c
}

void I_OsPolling(void)
{
	// Polling handled elsewhere
}

/*
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
*/

void I_StartupMouse(void)
{
	// Mouse not supported on DSi
}

void I_ShutdownMouse(void)
{
}

// Stubs for missing functions
void I_Tactile(FFType Type, const JoyFF_t *Effect) { (void)Type; (void)Effect; }
void I_UpdateNoVsync(void) {}
const CPUInfoFlags *I_CPUInfo(void) { return NULL; }

INT32 I_NumJoys(void)
{
	return 0;
}

const char *I_GetJoyName(INT32 joyindex)
{
	(void)joyindex;
	return NULL;
}

INT32 I_mkdir(const char *dirname, INT32 unixmode)
{
	(void)dirname;
	(void)unixmode;
	return -1;
}

// Clipboard stubs
INT32 I_ClipboardCopy(const char *data, size_t size) { (void)data; (void)size; return 0; }
const char *I_ClipboardPaste(void) { return NULL; }

// Input stubs
INT32 I_GetKey(void) { return 0; }
ticcmd_t *I_BaseTiccmd(void) { return NULL; }
ticcmd_t *I_BaseTiccmd2(void) { return NULL; }

void I_StartupMouse2(void) {}
void I_InitJoystick(void) {}
void I_InitJoystick2(void) {}

const char *I_LocateWad(void)
{
	FILE *f;

	// Check 1: Absolute path on fat device (standard)
	f = fopen("fat:/srb2/srb2.srb", "rb");
	if (f) { fclose(f); return "fat:/srb2"; }

	f = fopen("fat:/srb2/srb2.wad", "rb");
	if (f) { fclose(f); return "fat:/srb2"; }

	// Check 2: Absolute path on sd device (MelonDS/TwilightMenu sometimes prefer this)
	f = fopen("sd:/srb2/srb2.srb", "rb");
	if (f) { fclose(f); return "sd:/srb2"; }

	f = fopen("sd:/srb2/srb2.wad", "rb");
	if (f) { fclose(f); return "sd:/srb2"; }

	// Check 3: Current directory 'srb2' folder (relative)
	f = fopen("srb2/srb2.srb", "rb");
	if (f) { fclose(f); return "srb2"; }

	f = fopen("srb2/srb2.wad", "rb");
	if (f) { fclose(f); return "srb2"; }

	// Check 4: Root directory (if user put files in root)
	f = fopen("srb2.srb", "rb");
	if (f) { fclose(f); return "."; } // Return current directory

	f = fopen("srb2.wad", "rb");
	if (f) { fclose(f); return "."; }

	// Check 5: Root absolute
	f = fopen("fat:/srb2.srb", "rb");
	if (f) { fclose(f); return "fat:/"; }

	f = fopen("sd:/srb2.srb", "rb");
	if (f) { fclose(f); return "sd:/"; }

	return NULL;
}
void I_RegisterSysCommands(void) {}
void I_GetDiskFreeSpace(INT64 *total)
{
	if (total)
		*total = 1024 * 1024 * 1024; // 1GB
}

char *I_GetEnv(const char *name)
{
	(void)name;
	return NULL;
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

UINT32 I_GetFreeMem(UINT32 *total)
{
	// Return approximate free memory
	// DSi has 16MB total, but we can't easily query free memory
	// This is a rough estimate
	if (total)
		*total = 16 * 1024 * 1024;
	return 10 * 1024 * 1024; // Assume 10MB free on DSi
}

void I_DisplayType(void)
{
	// DSi XL specific
	CONS_Printf("DSi XL - 133MHz ARM9, 16MB RAM\n");
}

