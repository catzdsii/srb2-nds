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
/// \file  dsi_collision.c
/// \brief Collision detection using AABB (Axis-Aligned Bounding Boxes)

#include <nds.h>
#include <math.h>
#include "../doomdef.h"
#include "../p_local.h"
#include "../m_fixed.h"
#include "dsi_collision.h"

// AABB collision detection (simplified for performance)
// Uses simple bounding boxes instead of complex polygon collision

boolean DSI_Collision_AABB(fixed_t x1, fixed_t y1, fixed_t z1, fixed_t w1, fixed_t h1, fixed_t d1,
                            fixed_t x2, fixed_t y2, fixed_t z2, fixed_t w2, fixed_t h2, fixed_t d2)
{
	// Check if two AABBs overlap
	fixed_t min1x = x1 - w1/2;
	fixed_t max1x = x1 + w1/2;
	fixed_t min1y = y1 - h1/2;
	fixed_t max1y = y1 + h1/2;
	fixed_t min1z = z1 - d1/2;
	fixed_t max1z = z1 + d1/2;
	
	fixed_t min2x = x2 - w2/2;
	fixed_t max2x = x2 + w2/2;
	fixed_t min2y = y2 - h2/2;
	fixed_t max2y = y2 + h2/2;
	fixed_t min2z = z2 - d2/2;
	fixed_t max2z = z2 + d2/2;
	
	// Check overlap on all axes
	if (max1x < min2x || min1x > max2x)
		return false;
	if (max1y < min2y || min1y > max2y)
		return false;
	if (max1z < min2z || min1z > max2z)
		return false;
	
	return true; // Collision detected
}

// Check collision between mobj and a point
boolean DSI_Collision_PointInAABB(fixed_t px, fixed_t py, fixed_t pz,
                                   fixed_t bx, fixed_t by, fixed_t bz,
                                   fixed_t bw, fixed_t bh, fixed_t bd)
{
	fixed_t minx = bx - bw/2;
	fixed_t maxx = bx + bw/2;
	fixed_t miny = by - bh/2;
	fixed_t maxy = by + bh/2;
	fixed_t minz = bz - bd/2;
	fixed_t maxz = bz + bd/2;
	
	return (px >= minx && px <= maxx &&
	        py >= miny && py <= maxy &&
	        pz >= minz && pz <= maxz);
}

// Simple collision response (push object out of collision)
void DSI_Collision_Response(fixed_t *x1, fixed_t *y1, fixed_t *z1,
                            fixed_t x2, fixed_t y2, fixed_t z2,
                            fixed_t w1, fixed_t h1, fixed_t d1,
                            fixed_t w2, fixed_t h2, fixed_t d2)
{
	// Calculate overlap on each axis
	fixed_t dx = (*x1 - x2);
	fixed_t dy = (*y1 - y2);
	fixed_t dz = (*z1 - z2);
	
	fixed_t overlapX = (w1/2 + w2/2) - (dx < 0 ? -dx : dx);
	fixed_t overlapY = (h1/2 + h2/2) - (dy < 0 ? -dy : dy);
	fixed_t overlapZ = (d1/2 + d2/2) - (dz < 0 ? -dz : dz);
	
	// Push out on the axis with minimum overlap
	if (overlapX < overlapY && overlapX < overlapZ)
	{
		if (dx > 0)
			*x1 += overlapX;
		else
			*x1 -= overlapX;
	}
	else if (overlapY < overlapZ)
	{
		if (dy > 0)
			*y1 += overlapY;
		else
			*y1 -= overlapY;
	}
	else
	{
		if (dz > 0)
			*z1 += overlapZ;
		else
			*z1 -= overlapZ;
	}
}

