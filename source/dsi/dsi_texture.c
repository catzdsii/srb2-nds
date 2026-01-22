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
/// \file  dsi_texture.c
/// \brief Texture management for DSi (palettized 8-bit textures)

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../doomdef.h"
#include "../w_wad.h"
#include "dsi_texture.h"
#include "dsi_utils.h"

// Texture cache for DSi
// Uses 8-bit palettized textures (max 128x128)

#define MAX_CACHED_TEXTURES 32
#define MAX_TEXTURE_SIZE 128

typedef struct {
	char name[9];           // Texture name (8 chars + null)
	UINT8 *data;            // 8-bit texture data
	int width, height;      // Texture dimensions
	boolean loaded;         // Is texture loaded?
} cached_texture_t;

static cached_texture_t textureCache[MAX_CACHED_TEXTURES];
static int numCachedTextures = 0;

void DSI_Texture_Init(void)
{
	memset(textureCache, 0, sizeof(textureCache));
	numCachedTextures = 0;
	
	CONS_Printf("DSi texture system initialized\n");
}

void DSI_Texture_Shutdown(void)
{
	for (int i = 0; i < MAX_CACHED_TEXTURES; i++)
	{
		if (textureCache[i].data)
		{
			DSI_Free(textureCache[i].data);
			textureCache[i].data = NULL;
		}
	}
}

// Load texture from WAD (8-bit palettized)
int DSI_Texture_Load(const char *name, UINT8 **data, int *width, int *height)
{
	// Check cache first
	for (int i = 0; i < numCachedTextures; i++)
	{
		if (strncmp(textureCache[i].name, name, 8) == 0 && textureCache[i].loaded)
		{
			*data = textureCache[i].data;
			*width = textureCache[i].width;
			*height = textureCache[i].height;
			return i;
		}
	}
	
	// Load from WAD
	// This is simplified - in real implementation, we'd load from WAD file
	// For now, return NULL
	
	*data = NULL;
	*width = 0;
	*height = 0;
	return -1;
}

// Convert texture to DSi format (downsample if needed)
void DSI_Texture_Convert(UINT8 *src, int srcWidth, int srcHeight,
                         UINT8 *dst, int dstWidth, int dstHeight)
{
	if (!src || !dst)
		return;
	
	// Simple nearest-neighbor downsampling
	float scaleX = (float)srcWidth / dstWidth;
	float scaleY = (float)srcHeight / dstHeight;
	
	for (int y = 0; y < dstHeight; y++)
	{
		for (int x = 0; x < dstWidth; x++)
		{
			int srcX = (int)(x * scaleX);
			int srcY = (int)(y * scaleY);
			
			if (srcX < srcWidth && srcY < srcHeight)
			{
				dst[y * dstWidth + x] = src[srcY * srcWidth + srcX];
			}
		}
	}
}

