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
/// \file  dsi_sprite.c
/// \brief Sprite management for DSi (billboards for Sonic and enemies)

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../doomdef.h"
#include "../p_mobj.h"
#include "../r_defs.h"
#include "dsi_sprite.h"
#include "dsi_utils.h"

// Sprite system for DSi
// Sonic and enemies are rendered as 2D billboards in the 3D world

#define MAX_SPRITES 32
#define SPRITE_SIZE 32  // 32x32 sprites

typedef struct {
	int x, y;           // Screen position
	int width, height;  // Sprite dimensions
	u16 *data;          // Sprite data (RGB15)
	boolean active;     // Is sprite active?
	float distance;     // Distance from camera (for sorting)
} sprite_t;

static sprite_t sprites[MAX_SPRITES];
static int numSprites = 0;

void DSI_Sprite_Init(void)
{
	memset(sprites, 0, sizeof(sprites));
	numSprites = 0;
	
	CONS_Printf("DSi sprite system initialized\n");
}

void DSI_Sprite_Shutdown(void)
{
	for (int i = 0; i < MAX_SPRITES; i++)
	{
		if (sprites[i].data)
		{
			DSI_Free(sprites[i].data);
			sprites[i].data = NULL;
		}
	}
}

// Load sprite from 8-bit palettized data
void DSI_Sprite_Load(int spriteId, UINT8 *data, int width, int height, u16 *palette)
{
	if (spriteId < 0 || spriteId >= MAX_SPRITES)
		return;
	
	sprite_t *spr = &sprites[spriteId];
	
	// Free old data
	if (spr->data)
		DSI_Free(spr->data);
	
	// Allocate and convert
	spr->data = (u16*)DSI_Malloc(width * height * sizeof(u16));
	if (spr->data)
	{
		DSI_ConvertTexture8to16(data, spr->data, width, height, palette);
		spr->width = width;
		spr->height = height;
		spr->active = true;
	}
}

// Render sprite as billboard
void DSI_Sprite_RenderBillboard(int spriteId, float x, float y, float z, 
                                 float camX, float camY, float camZ, float camAngle)
{
	if (spriteId < 0 || spriteId >= MAX_SPRITES || !sprites[spriteId].active)
		return;
	
	sprite_t *spr = &sprites[spriteId];
	
	// Calculate sprite position relative to camera
	float dx = x - camX;
	float dy = y - camY;
	float dz = z - camZ;
	
	// Calculate distance for sorting
	float distance = sqrtf(dx*dx + dy*dy + dz*dz);
	spr->distance = distance;
	
	// Project to screen (simplified 2D projection)
	// This is a very basic implementation
	float screenX = 128.0f + dx * 10.0f; // Rough projection
	float screenY = 96.0f - dz * 10.0f;
	
	spr->x = (int)screenX;
	spr->y = (int)screenY;
}

// Draw all sprites to frame buffer
void DSI_Sprite_DrawAll(u16 *frameBuffer, int screenWidth, int screenHeight)
{
	// Sort sprites by distance (back to front)
	for (int i = 0; i < numSprites - 1; i++)
	{
		for (int j = i + 1; j < numSprites; j++)
		{
			if (sprites[i].distance < sprites[j].distance)
			{
				sprite_t temp = sprites[i];
				sprites[i] = sprites[j];
				sprites[j] = temp;
			}
		}
	}
	
	// Draw sprites
	for (int i = 0; i < numSprites; i++)
	{
		if (!sprites[i].active || !sprites[i].data)
			continue;
		
		int x = sprites[i].x - sprites[i].width / 2;
		int y = sprites[i].y - sprites[i].height / 2;
		
		// Clip to screen
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x + sprites[i].width > screenWidth)
			x = screenWidth - sprites[i].width;
		if (y + sprites[i].height > screenHeight)
			y = screenHeight - sprites[i].height;
		
		// Draw sprite
		for (int sy = 0; sy < sprites[i].height; sy++)
		{
			for (int sx = 0; sx < sprites[i].width; sx++)
			{
				u16 color = sprites[i].data[sy * sprites[i].width + sx];
				// Skip transparent pixels (assuming 0 is transparent)
				if (color != 0)
				{
					int px = x + sx;
					int py = y + sy;
					if (px >= 0 && px < screenWidth && py >= 0 && py < screenHeight)
					{
						frameBuffer[py * screenWidth + px] = color;
					}
				}
			}
		}
	}
}

