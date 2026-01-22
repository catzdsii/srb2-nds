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
/// \file  r_raycast.h
/// \brief Raycasting engine header for DSi (Doom-style rendering)

#ifndef __R_RAYCAST__
#define __R_RAYCAST__

#include "../doomdef.h"
#include "../r_defs.h"
#include "../r_local.h"

// Raycasting constants
#define RAYCAST_WIDTH  256
#define RAYCAST_HEIGHT 192
#define RAYCAST_FOV    90  // Field of view in degrees
#define MAX_DISTANCE   2048.0f
#define WALL_HEIGHT    64.0f

// Initialize raycasting engine
void R_InitRaycast(void);

// Shutdown raycasting engine
void R_ShutdownRaycast(void);

// Render a frame using raycasting
void R_RenderRaycastFrame(player_t *player);

// Get the current frame buffer
u16 *R_GetFrameBuffer(void);

// Set wall texture
void R_SetWallTexture(int textureId, UINT8 *data, int width, int height);

// Set floor/ceiling texture
void R_SetFloorTexture(int textureId, UINT8 *data, int width, int height);
void R_SetCeilingTexture(int textureId, UINT8 *data, int width, int height);

// Set sky texture (static image)
void R_SetSkyTexture(UINT8 *data, int width, int height);

#endif // __R_RAYCAST__

