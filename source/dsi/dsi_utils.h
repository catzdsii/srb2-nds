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
/// \file  dsi_utils.h
/// \brief Utility functions for DSi port

#ifndef __DSI_UTILS__
#define __DSI_UTILS__

#include <stdio.h>
#include <nds.h>
#include "../doomtype.h"

// Utility macros
#define DSI_LOG(fmt, ...) iprintf("[DSi] " fmt "\n", ##__VA_ARGS__)

// Memory management helpers
void *DSI_Malloc(size_t size);
void DSI_Free(void *ptr);

// Texture conversion helpers
void DSI_ConvertTexture8to16(UINT8 *src8, u16 *dst16, int width, int height, u16 *palette);

#endif // __DSI_UTILS__

