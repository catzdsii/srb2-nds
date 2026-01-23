// SONIC ROBO BLAST 2 - DSi Port
//-----------------------------------------------------------------------------
// Simplified D_SRB2Main for DSi with extreme memory constraints
// This is a minimal initialization - NOT the full game startup!

#include "../doomdef.h"
#include "../d_main.h"
#include "../m_argv.h"
#include "../console.h"
#include "../z_zone.h"
#include "../i_system.h"
#include "../g_game.h"
#include "../p_setup.h"
#include "../w_wad.h"
#include "../i_sound.h"
#include "../m_misc.h"
#include "../i_video.h"
#include "r_raycast.h"

// --- Missing Global Variables (Linker Fixes) ---
boolean dedicated = false;
boolean devparm = false;
boolean debugload = false;
boolean advancedemo = false;
boolean singletics = false;
boolean window_notinfocus = false;
boolean sound_disabled = false;
boolean digital_disabled = false;
boolean midi_disabled = false;
boolean shiftdown = false;
boolean ctrldown = false;

char srb2home[MAX_WADPATH] = "sd:/";
char srb2path[MAX_WADPATH] = "sd:/";
char savegamename[256] = "sd:/srb2save.ssg";
const char *pandf = "%s/%s";

tic_t rendergametic = 0;
tic_t lastdraw = 0;
gamestate_t wipegamestate = GS_LEVEL;
INT32 postimgtype = 0;
INT32 postimgtype2 = 0;

// --- Missing Functions (Linker Fixes) ---
void D_StartTitle(void) { gamestate = GS_LEVEL; }
void D_ProcessEvents(void) {}
void D_AdvanceDemo(void) {}

// Minimal D_SRB2Main for DSi - skips most initialization
void D_SRB2Main_DSi(void)
{
	CONS_Printf("=== DSi Boot Sequence ===\n");
	
	// 1. Memory setup
	CONS_Printf("Z_Init(): Init zone memory allocation daemon.\n");
	Z_Init();

	// 2. Timer
	CONS_Printf("I_StartupTimer()...\n");
	I_StartupTimer();

	// 3. Initialize tables
	CONS_Printf("P_PatchInfoTables()...\n");
	P_PatchInfoTables();

	// 4. Try to load WAD files (essential for SRB2)
	const char *wad_paths[] = {
		"sd:/srb2/srb2.wad",
		"sd:/srb2/zones.wad",
		"sd:/srb2/player.dta",
		"sd:/SRB2.WAD",
		"sd:/SRB2.SRB",
		"sd:/roms/nds/SRB2.WAD",
		"sd:/roms/nds/SRB2.SRB",
		// "fat:/" paths disabled to prevent DSi hardware hangs
		/*
		"fat:/srb2/srb2.wad",
		"fat:/srb2/zones.wad",
		"fat:/srb2/player.dta",
		"fat:/SRB2.WAD",
		"fat:/SRB2.SRB",
		"fat:/roms/nds/SRB2.WAD",
		"fat:/roms/nds/SRB2.SRB",
		*/
		"SRB2.WAD",
		"SRB2.SRB"
	};
	char *wads[2] = { NULL, NULL };

	printf("Searching for SRB2.WAD...\n");
	for (int i = 0; i < 9; i++) { // Adjusted loop count for removed paths
		if (FIL_ReadFileOK(wad_paths[i])) {
			wads[0] = (char*)wad_paths[i];
			printf("Found: %s\n", wad_paths[i]);
			break;
		}
	}

	if (wads[0] != NULL)
	{
		printf("Loading %s...\n", wads[0]);
		if (!W_InitMultipleFiles(wads))
		{
			printf("ERROR: W_InitMultipleFiles failed!\n");
		}
		else
		{
			printf("Successfully loaded game data.\n");
		}
	}
	else
	{
		printf("CRITICAL: SRB2.WAD not found!\n");
		printf("Please place SRB2.WAD on SD card root.\n");
	}

	// 5. Initialize sound (stub only on DSi)
	CONS_Printf("I_StartupSound()...\n");
	I_StartupSound();

	// Skip:
	// - P_BackupTables() (corrupts memory)
	// - M_SetupDefaultConditionSets() (uses too much RAM)
	// 6. Initialize graphics
	CONS_Printf("I_StartupGraphics()...\n");
	I_StartupGraphics();

	CONS_Printf("Boot complete - starting game loop!\n");
	
	// Set initial game state
	gamestate = GS_LEVEL;
	player_t *player = &players[consoleplayer];
	
	// Main game loop
	while(1) {
		// Render frame
		R_RenderRaycastFrame(player);
		
		// Swap buffers / VBlank
		I_FinishUpdate();
		
		// Handle input
		scanKeys();
		if (keysDown() & KEY_START) break;
	}
}

// Stub D_SRB2Main that calls DSi version
void D_SRB2Main(void)
{
	D_SRB2Main_DSi();
}
