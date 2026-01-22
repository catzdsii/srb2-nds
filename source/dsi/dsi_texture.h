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
/// \file  dsi_texture.h
/// \brief Texture management header for DSi

#ifndef __DSI_TEXTURE__
#define __DSI_TEXTURE__

#include <nds.h>
#include "../doomdef.h"

// Initialize texture system
void DSI_Texture_Init(void);

// Shutdown texture system
void DSI_Texture_Shutdown(void);

// Load texture from WAD (8-bit palettized)
int DSI_Texture_Load(const char *name, UINT8 **data, int *width, int *height);

// Convert texture to DSi format (downsample if needed)
void DSI_Texture_Convert(UINT8 *src, int srcWidth, int srcHeight,
                         UINT8 *dst, int dstWidth, int dstHeight);

#endif // __DSI_TEXTURE__

