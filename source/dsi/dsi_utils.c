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
/// \file  dsi_utils.c
/// \brief Utility functions implementation for DSi port

#include <nds.h>
#include <stdlib.h>
#include <string.h>
#include "dsi_utils.h"

void *DSI_Malloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
	{
		DSI_LOG("Memory allocation error: %u bytes", (unsigned int)size);
	}
	return ptr;
}

void DSI_Free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void DSI_ConvertTexture8to16(UINT8 *src8, u16 *dst16, int width, int height, u16 *palette)
{
	if (!src8 || !dst16 || !palette)
		return;
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			UINT8 index = src8[y * width + x];
			dst16[y * width + x] = palette[index];
		}
	}
}

