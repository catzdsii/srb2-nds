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
/// \file  dsi_collision.h
/// \brief Collision detection header using AABB

#ifndef __DSI_COLLISION__
#define __DSI_COLLISION__

#include "../doomdef.h"
#include "../m_fixed.h"

// Check collision between two AABBs
// x, y, z: center position
// w, h, d: width, height, depth
boolean DSI_Collision_AABB(fixed_t x1, fixed_t y1, fixed_t z1, fixed_t w1, fixed_t h1, fixed_t d1,
                            fixed_t x2, fixed_t y2, fixed_t z2, fixed_t w2, fixed_t h2, fixed_t d2);

// Check if a point is inside an AABB
boolean DSI_Collision_PointInAABB(fixed_t px, fixed_t py, fixed_t pz,
                                   fixed_t bx, fixed_t by, fixed_t bz,
                                   fixed_t bw, fixed_t bh, fixed_t bd);

// Simple collision response (push object out)
void DSI_Collision_Response(fixed_t *x1, fixed_t *y1, fixed_t *z1,
                            fixed_t x2, fixed_t y2, fixed_t z2,
                            fixed_t w1, fixed_t h1, fixed_t d1,
                            fixed_t w2, fixed_t h2, fixed_t d2);

#endif // __DSI_COLLISION__

