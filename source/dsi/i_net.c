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
/// \file  i_net.c
/// \brief Network interface for DSi (not supported)

#include "../doomdef.h"
#include "../i_net.h"

// Network not supported on DSi
boolean I_InitNetwork(void) { return false; }
void I_ShutdownNetwork(void) {}
INT32 I_GetLocalAddress(void) { return 0; }
const char *I_GetLocalAddressString(void) { return "127.0.0.1"; }

// Network function pointers
void (*I_NetSend)(void) = NULL;
boolean (*I_NetGet)(void) = NULL;
boolean (*I_NetCanGet)(void) = NULL;
boolean (*I_NetCanSend)(void) = NULL;
void (*I_NetFreeNodenum)(INT32 nodenum) = NULL;
SINT8 (*I_NetMakeNodewPort)(const char *address, const char *port) = NULL;
boolean (*I_NetOpenSocket)(void) = NULL;
void (*I_NetCloseSocket)(void) = NULL;
boolean (*I_Ban)(INT32 node) = NULL;
void (*I_ClearBans)(void) = NULL;
const char *(*I_GetNodeAddress)(INT32 node) = NULL;
const char *(*I_GetBanAddress)(size_t ban) = NULL;
const char *(*I_GetBanMask)(size_t ban) = NULL;
boolean (*I_SetBanAddress)(const char *address, const char *mask) = NULL;
boolean *bannednode = NULL;

SINT8 I_NetMakeNode(const char *address) { (void)address; return -1; }
boolean I_NetMakeAddress(const char *address, INT32 *netaddress) { (void)address; (void)netaddress; return false; }

