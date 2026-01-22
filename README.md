
# SRB2 3DS / DSi
A fork from the port of Sonic Robo Blast 2 to the Nintendo New3DS/2DS consoles, with a additional port for Nintendo DSi and DSi XL.

## Installing

### DSi/DSi XL Version
* This port is for Nintendo DSi and DSi XL consoles.
* Download the latest release and extract srb2_dsi.nds.
* Copy srb2_dsi.nds to your SD card in the /roms/nds/ folder (if using Twilight Menu++).
* Download SRB2's assets and extract them to the appropriate location on your SD card.
* See [README_DSI.md](README_DSI.md) for detailed installation instructions.

## Building


### DSi/DSi XL Version
* This port is for Nintendo DSi and DSi XL consoles.
* This port is based on the 3DS port and uses a simplified raycasting engine optimized for DSi hardware constraints (133 MHz CPU, 16 MB RAM).
* Building requires nds-dev and maxmod-nds to be installed (use devkitPro's pacman).
* See [README_DSI.md](README_DSI.md) for detailed build instructions.
* Compile with: `make -f Makefile.dsi`

## Known Issues/Limitations

### DSi/DSi XL Version
* This port is for Nintendo DSi and DSi XL consoles.
* This port is based on the 3DS port but uses a simplified raycasting engine (Doom-style) to run on DSi hardware constraints (133 MHz CPU, 16 MB RAM).
* Only Greenflower Zone Act 1 is included.
* Maximum 5-10 enemies active at once.
* No network support (no netplay).
* No split-screen.
* No MIDI music (uses .it or compressed .wav format).
* Static sky (no scrolling).
* No water reflections.
* See [README_DSI.md](README_DSI.md) for full details.

## Thanks
Thanks to fincs, WinterMute, Monster Iestyn, Sryder, AlamTaz, Steel Titanium for help with development.
Thanks to profi200, fincs and WinterMute for testing.
Credit for 3DS homebrew logo goes to [PabloMK7](http://gbatemp.net/members/pablomk7.345712/).

## DSi Port Credits
The Nintendo DSi and DSi XL port was created by **catzdsii** and is based on the 3DS port. It uses a simplified raycasting engine optimized for DSi hardware constraints.
