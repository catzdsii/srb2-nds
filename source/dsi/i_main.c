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
/// \file  i_main.c
/// \brief SRB2 main program for DSi

#include <nds.h>
#include <stdio.h>
#include <fat.h>

#include "../doomdef.h"
#include "../d_main.h"
#include "../m_argv.h"
#include "../i_system.h"

int main(int argc, char **argv)
{
	// Initialize the DS hardware
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);
	
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_MAIN_BG);
	vramSetBankE(VRAM_E_MAIN_SPRITE);
	vramSetBankF(VRAM_F_LCD);
	vramSetBankG(VRAM_G_MAIN_BG);
	vramSetBankH(VRAM_H_SUB_BG);
	vramSetBankI(VRAM_I_SUB_SPRITE);
	
	// Initialize FAT filesystem for SD card access
	if (!fatInitDefault())
	{
		videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
		vramSetBankC(VRAM_C_SUB_BG);
		consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
		iprintf("Error: Could not initialize FAT!\n");
		iprintf("Make sure your SD card\n");
		iprintf("is inserted correctly.\n");
		while(1) swiWaitForVBlank();
	}
	
	// Initialize Top Screen (Main)
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	// vramSetBankA(VRAM_A_MAIN_BG); // Already set above
	PrintConsole topScreen;
	consoleInit(&topScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, true, true);
	
	// Initialize Bottom Screen (Sub)
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	// vramSetBankC(VRAM_C_SUB_BG); // Already set above
	PrintConsole bottomScreen;
	consoleInit(&bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false, true);
	
	// Select Top Screen for ASCII Art
	consoleSelect(&topScreen);
	iprintf("\x1b[2J"); // Clear screen
	
	// Center text vertically
	iprintf("\n\n\n\n\n\n\n\n\n");
	
	// Display "catzdsii" in ASCII art (Big Text)
	iprintf("  ___ __ _ _ _ ___ ___  ___ _ _ \n");
	iprintf(" / __/ _` | '_|_  / _ \\/ __| | |\n");
	iprintf("| (_| (_| | |  / / (_) \\__ \\ | |\n");
	iprintf(" \\___\\__,_|_| /___\\___/|___/_|_|\n");
	
	// Wait for 3 seconds so the user can see it
	int wait_i;
	for(wait_i = 0; wait_i < 180; wait_i++) swiWaitForVBlank();
	
	// Clear Top Screen
	iprintf("\x1b[2J");
	
	// Select Bottom Screen for Logs
	consoleSelect(&bottomScreen);
	
	iprintf("SRB2-Lite DSi\n");
	iprintf("Port by catzdsii\n");
	iprintf("Based on 3DS port\n");
	iprintf("Initializing...\n");
	
	myargc = argc;
	myargv = argv;

	CONS_Printf("I_StartupSystem...");
	I_StartupSystem();

	// Check if arguments were passed via DSi Homebrew Launcher (ARGV protocol)
	// Modern launchers like TWiLight Menu++ support passing arguments via ARGV
	if (argc > 1)
	{
		CONS_Printf("Received %d arguments from launcher:\n", argc);
		int i;
		for (i = 0; i < argc; i++)
		{
			CONS_Printf("%s\n", argv[i]);
		}
		myargc = argc;
		myargv = argv;
	}
	else
	{
		// Fallback for direct launch or launchers without ARGV support
		// We define required paths explicitly to ensure WADs are found on SD card
		CONS_Printf("No arguments received, using default DSi paths\n");
		static char *dsi_argv[] = {
			"srb2_dsi",
			"-file", 
			"sd:/srb2/srb2.srb",
			"sd:/srb2/zones.wad", 
			NULL
		};
		myargc = 4;
		myargv = dsi_argv;
	}

	// startup SRB2
	CONS_Printf("Setting up SRB2...\n");
	D_SRB2Main();
	
	CONS_Printf("Entering main game loop...\n");

	// never return
	D_SRB2Loop();

	// return to OS (should never reach here)
	return 0;
}

