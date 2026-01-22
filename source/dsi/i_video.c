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
/// \file  i_video.c
/// \brief Video interface for DSi using raycasting engine

#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../doomdef.h"
#include "../i_video.h"
#include "../z_zone.h"
#include "../v_video.h"
#include "../r_main.h"
#include "../r_local.h"
#include "dsi_utils.h"
#include "r_raycast.h"

// DSi screen dimensions
#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192

rendermode_t rendermode = render_soft;
boolean highcolor = false;
boolean allow_fullscreen = false;

consvar_t cv_vidwait = {"vid_wait", "On", CV_SAVE, CV_OnOff, NULL, 0, NULL, NULL, 0, 0, NULL};

// Video buffers
static u16 *videoBufferMain = NULL;
static u16 *videoBufferSub = NULL;

void I_StartupGraphics(void)
{
	// Setup video mode for main screen
	videoSetMode(MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE);
	
	// Allocate video buffers
	videoBufferMain = (u16*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16));
	videoBufferSub = (u16*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16));
	
	if (!videoBufferMain || !videoBufferSub)
	{
		I_Error("Could not allocate video buffers!");
	}
	
	// Setup backgrounds
	bgInit(0, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(0, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	
	// Initialize video structure
	vid.width = SCREEN_WIDTH;
	vid.height = SCREEN_HEIGHT;
	vid.bpp = 2; // 16-bit
	vid.rowbytes = vid.width * vid.bpp;
	vid.recalc = false;
	
	// Initialize raycasting engine
	R_InitRaycast();
	
	CONS_Printf("DSi graphics initialized: %dx%d\n", vid.width, vid.height);
}

void I_ShutdownGraphics(void)
{
	if (videoBufferMain)
	{
		free(videoBufferMain);
		videoBufferMain = NULL;
	}
	if (videoBufferSub)
	{
		free(videoBufferSub);
		videoBufferSub = NULL;
	}
	
	R_ShutdownRaycast();
}

void I_SetPalette(RGBA_t *palette)
{
	// Convert palette to DSi format
	// DSi uses 15-bit color (RGB555)
	for (int i = 0; i < 256; i++)
	{
		u16 color = RGB15(
			palette[i].s.red >> 3,
			palette[i].s.green >> 3,
			palette[i].s.blue >> 3
		);
		// Store in palette if needed
		// DSi doesn't use indexed color in MODE_5, but we can use it for sprites
	}
}

INT32 VID_NumModes(void)
{
	return 1; // Only one mode on DSi
}

INT32 VID_GetModeForSize(INT32 w, INT32 h)
{
	(void)w;
	(void)h;
	return 0; // Only one mode
}

void VID_PrepareModeList(void)
{
	// Not needed for DSi
}

INT32 VID_SetMode(INT32 modenum)
{
	(void)modenum;
	return 0; // Always succeeds
}

const char *VID_GetModeName(INT32 modenum)
{
	(void)modenum;
	return "DSi 256x192";
}

void I_UpdateNoBlit(void)
{
	// Not needed
}

void I_FinishUpdate(void)
{
	// Copy raycast buffer to screen
	if (videoBufferMain)
	{
		// Get the current frame buffer from raycasting engine
		u16 *frameBuffer = R_GetFrameBuffer();
		if (frameBuffer)
		{
			// Copy to main screen
			dmaCopy(frameBuffer, bgGetGfxPtr(0), SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16));
		}
	}
	
	// Wait for VBlank
	swiWaitForVBlank();
}

void I_WaitVBL(INT32 count)
{
	for (INT32 i = 0; i < count; i++)
	{
		swiWaitForVBlank();
	}
}

void I_ReadScreen(UINT8 *scr)
{
	// Not implemented
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

