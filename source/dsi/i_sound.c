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
//mm_sound_effect snd_effect;

static int current_mod_id = -1;

void I_StartupSound(void)
{
	// Initialize maxmod
	// We try to load a soundbank from the SD card (DSi Internal SD first)
	FILE *f_check = fopen("sd:/srb2/soundbank.bin", "rb");
	if (f_check)
	{
		fclose(f_check);
		mmInitDefault("sd:/srb2/soundbank.bin");
		CONS_Printf("Initialized soundbank from sd:/srb2/soundbank.bin\n");
	}
	else
	{
		// Fallback to fat:/ REMOVED
		// Warning: This may hang on DSi if Slot-1 is inaccessible
		// mmInitDefault("fat:/srb2/soundbank.bin");
		CONS_Printf("Soundbank not found on SD. Sound disabled.\n");
	}
	
	// Set sound volume
	mmSetModuleVolume(512);
	mmSetEffectsVolume(512);
	
	CONS_Printf("DSi sound system initialized (MaxMod)\n");
}

void I_ShutdownSound(void)
{
	//mmStop();
	//mmUnload(MOD_MUSIC);
}

void I_UpdateSound(void)
{
	// Update handled by maxmod
	//mmFrame();
}

void I_SubmitSound(void)
{
	// Not needed for maxmod
}

void I_SetMusicVolume(UINT8 volume)
{
	//mmSetModuleVolume(volume);
	(void)volume;
}

void I_SetSfxVolume(UINT8 volume)
{
	//mmSetEffectsVolume(volume);
	(void)volume;
}

INT32 I_StartSound(sfxenum_t id, UINT8 vol, UINT8 sep, UINT8 pitch, UINT8 priority, INT32 channel)
{
	// Start sound effect
	// For now, simplified implementation
	(void)id;
	(void)vol;
	(void)sep;
	(void)pitch;
	(void)priority;
	(void)channel;
	
	return 0;
}

void I_StopSound(INT32 handle)
{
	// Stop sound effect
	(void)handle;
}

boolean I_SoundIsPlaying(INT32 handle)
{
	// Check if sound is playing
	(void)handle;
	return false;
}

void I_UpdateSoundParams(INT32 handle, UINT8 vol, UINT8 sep, UINT8 pitch)
{
	// Update sound parameters
	(void)handle;
	(void)vol;
	(void)sep;
	(void)pitch;
}

// Music functions
boolean I_PlaySong(boolean looping)
{
	if (current_mod_id == -1)
		return false;

	mmStart(current_mod_id, looping ? MM_PLAY_LOOP : MM_PLAY_ONCE);
	return true;
}

void I_UnloadSong(void)
{
	if (current_mod_id != -1)
	{
		mmUnload(current_mod_id);
		current_mod_id = -1;
	}
}

boolean I_SetSongSpeed(float speed)
{
	(void)speed;
	return true;
}

boolean I_SongPlaying(void)
{
	return mmActive();
}

boolean I_SongPaused(void)
{
	return false;
}

void I_PauseSong(void)
{
	mmPause();
}

void I_ResumeSong(void)
{
	mmResume();
}

// These are needed for the linker
void I_InitMusic(void) {}
void I_ShutdownMusic(void) {}
boolean I_LoadSong(char *data, size_t len)
{
	(void)len;
	if (current_mod_id != -1)
	{
		//mmUnload(current_mod_id);
	}
		
	// current_mod_id = mmLoad((mm_addr)data);
	// Note: mmLoad expects a module ID from the soundbank, not a pointer.
	// Dynamic loading from WAD is not directly supported by standard mmLoad.
	(void)data;
	return true; 
}

// Dummy function for I_PlayCD
void I_PlayCD(UINT8 track, UINT8 looping) { (void)track; (void)looping; }
void I_StopCD(void) {}
void I_UpdateCD(void) {}
void I_InitCD(void) {}
void I_ResumeCD(void) {}
boolean I_SetSongTrack(INT32 track) { (void)track; return true; }

musictype_t I_SongType(void) { return MU_NONE; }
void I_UpdateMumble(const void *mobj, const void *listener) { (void)mobj; (void)listener; }
void I_FreeSfx(sfxinfo_t *sfx) { (void)sfx; }
void *I_GetSfx(sfxinfo_t *sfx) { (void)sfx; return NULL; }
void I_StopSong(void) {}

UINT8 sound_started = 0;

consvar_t cd_volume = {"cd_volume", "31", CV_SAVE, CV_Unsigned, NULL, 0, NULL, NULL, 0, 0, NULL};
consvar_t cdUpdate = {"cd_update", "1", CV_SAVE, CV_OnOff, NULL, 0, NULL, NULL, 0, 0, NULL};
