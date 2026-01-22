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
/// \file  r_raycast.c
/// \brief Raycasting engine implementation for DSi (Doom-style rendering)
///
/// This is a simplified raycasting engine similar to Doom's rendering.
/// It uses raycasting for walls and simple floor/ceiling rendering.

#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "../doomdef.h"
#include "../r_defs.h"
#include "../r_local.h"
#include "../r_main.h"
#include "../p_local.h"
#include "../d_player.h"
#include "../m_fixed.h"
#include "r_raycast.h"
#include "dsi_utils.h"

// Frame buffer for raycasting
static u16 *frameBuffer = NULL;

// Texture storage (simplified - using 8-bit palettized textures)
#define RC_MAX_TEXTURES 64
#define TEXTURE_SIZE 128  // Max 128x128

static UINT8 *wallTextures[RC_MAX_TEXTURES];
static int wallTextureWidth[RC_MAX_TEXTURES];
static int wallTextureHeight[RC_MAX_TEXTURES];

static UINT8 *floorTexture = NULL;
static UINT8 *ceilingTexture = NULL;
static UINT8 *skyTexture = NULL;
static int floorTexWidth, floorTexHeight;
static int ceilingTexWidth, ceilingTexHeight;
static int skyTexWidth, skyTexHeight;

// Color palette (converted from SRB2 palette)
static u16 colorPalette[256];

void R_InitRaycast(void)
{
	// Allocate frame buffer
	frameBuffer = (u16*)malloc(RAYCAST_WIDTH * RAYCAST_HEIGHT * sizeof(u16));
	if (!frameBuffer)
	{
		I_Error("Could not allocate frame buffer for raycasting!");
	}
	
	// Initialize texture arrays
	memset(wallTextures, 0, sizeof(wallTextures));
	memset(wallTextureWidth, 0, sizeof(wallTextureWidth));
	memset(wallTextureHeight, 0, sizeof(wallTextureHeight));
	
	CONS_Printf("Raycasting engine initialized\n");
}

void R_ShutdownRaycast(void)
{
	// Free frame buffer
	if (frameBuffer)
	{
		free(frameBuffer);
		frameBuffer = NULL;
	}
	
	// Free textures
	for (int i = 0; i < RC_MAX_TEXTURES; i++)
	{
		if (wallTextures[i])
		{
			free(wallTextures[i]);
			wallTextures[i] = NULL;
		}
	}
	
	if (floorTexture)
	{
		free(floorTexture);
		floorTexture = NULL;
	}
	
	if (ceilingTexture)
	{
		free(ceilingTexture);
		ceilingTexture = NULL;
	}
	
	if (skyTexture)
	{
		free(skyTexture);
		skyTexture = NULL;
	}
}

u16 *R_GetFrameBuffer(void)
{
	return frameBuffer;
}

void R_SetWallTexture(int textureId, UINT8 *data, int width, int height)
{
	if (textureId < 0 || textureId >= RC_MAX_TEXTURES)
		return;
	
	if (width > TEXTURE_SIZE || height > TEXTURE_SIZE)
		return;
	
	// Free old texture if exists
	if (wallTextures[textureId])
		free(wallTextures[textureId]);
	
	// Allocate and copy
	wallTextures[textureId] = (UINT8*)malloc(width * height);
	if (wallTextures[textureId])
	{
		memcpy(wallTextures[textureId], data, width * height);
		wallTextureWidth[textureId] = width;
		wallTextureHeight[textureId] = height;
	}
}

void R_SetFloorTexture(int textureId, UINT8 *data, int width, int height)
{
	(void)textureId; // For now, only one floor texture
	
	if (width > TEXTURE_SIZE || height > TEXTURE_SIZE)
		return;
	
	if (floorTexture)
		free(floorTexture);
	
	floorTexture = (UINT8*)malloc(width * height);
	if (floorTexture)
	{
		memcpy(floorTexture, data, width * height);
		floorTexWidth = width;
		floorTexHeight = height;
	}
}

void R_SetCeilingTexture(int textureId, UINT8 *data, int width, int height)
{
	(void)textureId; // For now, only one ceiling texture
	
	if (width > TEXTURE_SIZE || height > TEXTURE_SIZE)
		return;
	
	if (ceilingTexture)
		free(ceilingTexture);
	
	ceilingTexture = (UINT8*)malloc(width * height);
	if (ceilingTexture)
	{
		memcpy(ceilingTexture, data, width * height);
		ceilingTexWidth = width;
		ceilingTexHeight = height;
	}
}

void R_SetSkyTexture(UINT8 *data, int width, int height)
{
	if (skyTexture)
		free(skyTexture);
	
	skyTexture = (UINT8*)malloc(width * height);
	if (skyTexture)
	{
		memcpy(skyTexture, data, width * height);
		skyTexWidth = width;
		skyTexHeight = height;
	}
}

// Convert palette index to RGB15 color
static inline u16 PaletteToRGB15(UINT8 index)
{
	// This should use the actual SRB2 palette
	// For now, use a simple conversion
	if (index < 256)
		return colorPalette[index];
	return RGB15(0, 0, 0);
}

// Simple DDA raycasting (Digital Differential Analyzer)
static float CastRay(float rayX, float rayY, float dirX, float dirY, 
                     int *hitSide, int *mapX, int *mapY)
{
	float deltaDistX = fabsf(1.0f / dirX);
	float deltaDistY = fabsf(1.0f / dirY);
	
	int stepX, stepY;
	float sideDistX, sideDistY;
	
	// Determine which direction to step
	if (dirX < 0)
	{
		stepX = -1;
		sideDistX = (rayX - (int)rayX) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = ((int)rayX + 1.0f - rayX) * deltaDistX;
	}
	
	if (dirY < 0)
	{
		stepY = -1;
		sideDistY = (rayY - (int)rayY) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = ((int)rayY + 1.0f - rayY) * deltaDistY;
	}
	
	// Perform DDA
	int hit = 0;
	while (!hit)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			*mapX += stepX;
			*hitSide = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			*mapY += stepY;
			*hitSide = 1;
		}
		
		// Check if we hit a wall
		// This is simplified - in real SRB2, we'd check the BSP tree
		// For now, assume we hit something if we go out of bounds
		if (*mapX < 0 || *mapX >= 64 || *mapY < 0 || *mapY >= 64)
		{
			hit = 1;
		}
		// TODO: Check actual map data
	}
	
	// Calculate distance
	float perpWallDist;
	if (*hitSide == 0)
		perpWallDist = (sideDistX - deltaDistX);
	else
		perpWallDist = (sideDistY - deltaDistY);
	
	return perpWallDist;
}

void R_RenderRaycastFrame(player_t *player)
{
	if (!frameBuffer || !player || !player->mo)
		return;
	
	// Get player position and angle
	viewx = player->mo->x;
	viewy = player->mo->y;
	viewz = player->mo->z;
	viewangle = player->mo->angle;
	
	// Convert to float for calculations
	float px = FIXED_TO_FLOAT(viewx);
	float py = FIXED_TO_FLOAT(viewy);
	float angle = FIXED_TO_FLOAT(viewangle) * (M_PI / 180.0f);
	
	// Calculate view direction
	float dirX = cosf(angle);
	float dirY = sinf(angle);
	float planeX = -sinf(angle) * 0.66f; // FOV multiplier
	float planeY = cosf(angle) * 0.66f;
	
	// Clear frame buffer (draw sky first)
	u16 skyColor = RGB15(5, 10, 20); // Dark blue sky
	if (skyTexture)
	{
		// Draw sky texture (simplified - just a gradient for now)
		for (int y = 0; y < RAYCAST_HEIGHT / 2; y++)
		{
			for (int x = 0; x < RAYCAST_WIDTH; x++)
			{
				frameBuffer[y * RAYCAST_WIDTH + x] = skyColor;
			}
		}
	}
	else
	{
		// Fill with sky color
		for (int y = 0; y < RAYCAST_HEIGHT / 2; y++)
		{
			for (int x = 0; x < RAYCAST_WIDTH; x++)
			{
				frameBuffer[y * RAYCAST_WIDTH + x] = skyColor;
			}
		}
	}
	
	// Cast rays for each column
	for (int x = 0; x < RAYCAST_WIDTH; x++)
	{
		// Calculate ray position and direction
		float cameraX = 2.0f * x / (float)RAYCAST_WIDTH - 1.0f;
		float rayDirX = dirX + planeX * cameraX;
		float rayDirY = dirY + planeY * cameraX;
		
	// Cast ray
	int mapX = (int)px;
	int mapY = (int)py;
	int side = 0;
	float perpWallDist = CastRay(px, py, rayDirX, rayDirY, &side, &mapX, &mapY);
	
	// Clamp distance to prevent division by zero
	if (perpWallDist < 0.1f)
		perpWallDist = 0.1f;
		
	// Calculate line height
	int lineHeight = (int)(RAYCAST_HEIGHT / perpWallDist);
		if (lineHeight > RAYCAST_HEIGHT)
			lineHeight = RAYCAST_HEIGHT;
		
		// Calculate draw start and end
		int drawStart = -lineHeight / 2 + RAYCAST_HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + RAYCAST_HEIGHT / 2;
		if (drawEnd >= RAYCAST_HEIGHT)
			drawEnd = RAYCAST_HEIGHT - 1;
		
		// Draw wall column
		u16 wallColor = RGB15(15, 15, 15); // White wall (simplified)
		if (side == 1)
			wallColor = RGB15(10, 10, 10); // Darker for north/south walls
		
		for (int y = drawStart; y < drawEnd; y++)
		{
			frameBuffer[y * RAYCAST_WIDTH + x] = wallColor;
		}
		
		// Draw floor (simplified)
		u16 floorColor = RGB15(5, 5, 5); // Dark gray floor
		for (int y = drawEnd; y < RAYCAST_HEIGHT; y++)
		{
			frameBuffer[y * RAYCAST_WIDTH + x] = floorColor;
		}
	}
}

