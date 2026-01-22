// SONIC ROBO BLAST 2 - DSi Port
//-----------------------------------------------------------------------------
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
// Copyright (C) 1999-2016 by Sonic Team Junior.
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  dsi_sprite.h
/// \brief Sprite management header for DSi

#ifndef __DSI_SPRITE__
#define __DSI_SPRITE__

#include <nds.h>
#include "../doomdef.h"

// Initialize sprite system
void DSI_Sprite_Init(void);

// Shutdown sprite system
void DSI_Sprite_Shutdown(void);

// Load sprite from 8-bit palettized data
void DSI_Sprite_Load(int spriteId, UINT8 *data, int width, int height, u16 *palette);

// Render sprite as billboard
void DSI_Sprite_RenderBillboard(int spriteId, float x, float y, float z,
                                 float camX, float camY, float camZ, float camAngle);

// Draw all sprites to frame buffer
void DSI_Sprite_DrawAll(u16 *frameBuffer, int screenWidth, int screenHeight);

#endif // __DSI_SPRITE__

