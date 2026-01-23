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
/// \file  i_cdmus.c
/// \brief CD music interface for DSi (not supported)

#include "../doomdef.h"
#include "../i_sound.h"
#include "../s_sound.h"
#include "../command.h"

// CD music not supported on DSi - stub implementation
//consvar_t cd_volume = {"cd_volume","31",CV_SAVE,soundvolume_cons_t, NULL, 0, NULL, NULL, 0, 0, NULL};
//consvar_t cdUpdate  = {"cd_update","1",CV_SAVE, NULL, NULL, 0, NULL, NULL, 0, 0, NULL};

// CD music not supported on DSi
void I_CDMusInit(void) {}
void I_CDMusShutdown(void) {}
void I_CDMusPlay(INT32 track) { (void)track; }
void I_CDMusStop(void) {}
void I_CDMusResume(void) {}
void I_CDMusSetVolume(INT32 volume) { (void)volume; }
INT32 I_CDMusGetVolume(void) { return 0; }

