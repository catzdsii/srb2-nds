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
/// \file  i_sound.c
/// \brief Sound interface for DSi

#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <string.h>

#include "../doomdef.h"
#include "../i_sound.h"
#include "../s_sound.h"
#include "../z_zone.h"

// DSi sound system using maxmod (supports .it and .wav)
mm_sound_effect snd_effect;

void I_StartupSound(void)
{
	// Initialize maxmod
	// Note: soundbank_bin needs to be created separately
	// mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// Set sound volume
	mmSetModuleVolume(512);
	mmSetEffectsVolume(512);
	
	CONS_Printf("DSi sound system initialized (maxmod)\n");
}

void I_ShutdownSound(void)
{
	mmStop();
	mmUnload(MOD_MUSIC);
}

void I_UpdateSound(void)
{
	// Update handled by maxmod
	mmFrame();
}

void I_SubmitSound(void)
{
	// Not needed for maxmod
}

void I_SetMusicVolume(int volume)
{
	mmSetModuleVolume(volume);
}

void I_SetSfxVolume(int volume)
{
	mmSetEffectsVolume(volume);
}

int I_StartSound(int id, int vol, int sep, int pitch, int priority, const mobj_t *origin)
{
	// Start sound effect
	// For now, simplified implementation
	(void)id;
	(void)vol;
	(void)sep;
	(void)pitch;
	(void)priority;
	(void)origin;
	
	return 0;
}

void I_StopSound(int handle, int id)
{
	// Stop sound effect
	(void)handle;
	(void)id;
}

boolean I_SoundIsPlaying(int handle)
{
	// Check if sound is playing
	(void)handle;
	return false;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	// Update sound parameters
	(void)handle;
	(void)vol;
	(void)sep;
	(void)pitch;
}

void I_StartMusic(const char *musicname, boolean loop)
{
	// Start music (using .it or .wav format)
	// For now, simplified
	(void)musicname;
	(void)loop;
}

void I_StopMusic(void)
{
	mmStop();
}

void I_PauseMusic(void)
{
	mmPause();
}

void I_ResumeMusic(void)
{
	mmResume();
}

void I_SetMusicMusLump(void *data, size_t len)
{
	// Set music from memory
	(void)data;
	(void)len;
}

void I_InitMusic(void)
{
	// Music initialization
}

void I_ShutdownMusic(void)
{
	// Music shutdown
	I_StopMusic();
}

