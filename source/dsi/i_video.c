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
/// \brief Video interface for DSi using 8-bit software rendering

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

// DSi screen dimensions
#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT
#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192

rendermode_t rendermode = render_soft;
boolean highcolor = false;
boolean allow_fullscreen = false;
UINT8 graphics_started = 0;

consvar_t cv_vidwait = {"vid_wait", "On", CV_SAVE, CV_OnOff, NULL, 0, NULL, NULL, 0, 0, NULL};

void I_StartupGraphics(void)
{
	graphics_started = 1;
	// Setup video mode for main screen (Top)
	// MODE_5_2D allows BgType_Bmp8 (Extended Palette) on BG3
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	
	// Setup video mode for sub screen (Bottom - Console)
	// Enable BG0 for text output
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankC(VRAM_C_SUB_BG); 
	
	// Re-init console on Sub screen
	// MapBase 2 (offset 4KB), TileBase 0 (offset 0KB)
	consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
	
	// Initialize Main BG3 as 8-bit Bitmap
	bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	
	// Initialize Sub BG0 as Text (Console)
	// Console init is handled in i_main.c usually, but ensuring it here doesn't hurt
	// consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	// Initialize video structure for SRB2 Software Renderer
	vid.width = SCREEN_WIDTH;
	vid.height = SCREEN_HEIGHT;
	vid.bpp = 1; // 8-bit
	vid.rowbytes = vid.width; // 1 byte per pixel
	vid.recalc = false;
	
	// Allocate 8-bit buffer in Main RAM (or Fast RAM if available)
	// We need 256*192 = 49152 bytes.
	// Z_Malloc is preferred for SRB2 memory management, but static allocation works too.
	vid.buffer = Z_Malloc(vid.width * vid.height * vid.bpp, PU_STATIC, NULL);
	
	if (!vid.buffer)
	{
		I_Error("Could not allocate video buffer!");
	}
	
	// Clear buffer
	memset(vid.buffer, 0, vid.width * vid.height);

	CONS_Printf("DSi graphics initialized: %dx%d 8bpp\n", vid.width, vid.height);
}

void I_ShutdownGraphics(void)
{
	graphics_started = 0;
	// memory freed by Z_FreeTags(PU_STATIC) at exit usually
	// or we can explicitly free if we kept the pointer, but vid.buffer is global
}

void I_SetPalette(RGBA_t *palette)
{
	// Convert palette to DSi format (RGB555) and write to BG_PALETTE
	// DSi BG_PALETTE is 256 entries of 16-bit colors
	
	u16 dsi_palette[256];
	int i;
	
	for (i = 0; i < 256; i++)
	{
		dsi_palette[i] = RGB15(
			palette[i].s.red >> 3,
			palette[i].s.green >> 3,
			palette[i].s.blue >> 3
		);
	}
	
	// Copy to VRAM Palette (Main Engine)
	// DMA is safer/faster
	dmaCopy(dsi_palette, BG_PALETTE, 256 * 2);
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
	return "DSi 256x192 8bpp";
}

void I_UpdateNoBlit(void)
{
	// Not needed
}

void I_FinishUpdate(void)
{
	// Copy 8-bit buffer to VRAM
	// bgGetGfxPtr(3) returns address of BG3 bitmap data
	// 256*192 bytes
	
	// Wait for VBlank to avoid tearing? 
	// Or DMA immediately? DMA is fast.
	// SRB2 logic calls this at end of frame.
	
	swiWaitForVBlank(); // Wait first
	
	// DMA Copy: Source, Dest, Size (in words? dmaCopy uses bytes usually in libnds helpers)
	// dmaCopy uses bytes.
	dmaCopy(vid.buffer, bgGetGfxPtr(3), SCREEN_WIDTH * SCREEN_HEIGHT);
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
	// Read back from buffer
	memcpy(scr, vid.buffer, vid.width * vid.height);
}

void I_BeginRead(void)
{
	// Not needed
}

void I_EndRead(void)
{
	// Not needed
}
