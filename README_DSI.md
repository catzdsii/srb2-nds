# SRB2-Lite for DSi / DSi XL

An ultra-optimized port of Sonic Robo Blast 2 for the Nintendo DSi and DSi XL, using a simplified raycasting engine (Doom-style) to run on limited hardware constraints.

**Port by catzdsii - Based on the 3DS port**

## Hardware Constraints

- **CPU**: ARM9 at 133 MHz
- **RAM**: 16 MB
- **Screen**: 256x192 pixels (top screen)
- **3D Engine**: No OpenGL - uses native DS 2D/3D hardware via libnds

## Technical Features

### Rendering Engine

- **Raycasting**: Doom-style engine for wall rendering
- **2D Sprites**: Sonic and enemies are rendered as 2D billboards in the 3D world
- **Polygon Limit**: Maximum 2000 polygons per scene
- **Textures**: 8-bit palettized format (.pal), max resolution 128x128

### Content

- **Zone**: Greenflower Zone Act 1 only
- **Simplifications**:
  - No water reflections
  - Static sky (fixed image)
  - Maximum 5-10 enemies (Crawlas) active at once
- **Music**: .it (Impulse Tracker) format or highly compressed .wav

## Prerequisites

### Development Tools

1. **devkitPro**: Development toolchain for DS/DSi
   - Download from: https://devkitpro.org/
   - Install using pacman package manager

2. **libnds**: Development library for DS/DSi
   ```bash
   sudo dkp-pacman -S nds-dev
   ```

3. **maxmod-nds**: Audio library for DS (supports .it and .wav)
   ```bash
   sudo dkp-pacman -S maxmod-nds
   ```

### Optional Tools

- **Blender**: For modeling levels (very simply)
- **PicaPic / GraphicsMagick**: To convert SRB2 images to DS format
- **Emulator**: melonDS or DeSmuME for testing

## Compilation

### Environment Setup

Make sure `DEVKITARM` is set in your environment:

```bash
export DEVKITARM=/opt/devkitpro/devkitARM
export DEVKITPRO=/opt/devkitpro
```

### Compile

```bash
make -f Makefile.dsi
```

This will generate:
- `srb2_dsi.nds`: ROM file for DSi
- `srb2_dsi.arm9`: ARM9 binary

## Installation on DSi / DSi XL

### Method 1: Twilight Menu++

1. Install Twilight Menu++ on your DSi / DSi XL (if not already done)
2. Copy `srb2_dsi.nds` to your SD card in the `/roms/nds/` folder
3. Launch the game from Twilight Menu++

### Method 2: Flashcard

1. Copy `srb2_dsi.nds` to your flashcard
2. Launch the game from the flashcard menu

## Required Assets

You must extract SRB2 v2.1.22 assets and place them in the `data/` folder:

- Wall textures (format .pal, 8-bit, max 128x128)
- Sonic sprites (format .nitrostripe / .nanr)
- Greenflower Zone music (format .it or compressed .wav)
- Level data (MAP01 - Greenflower Zone Act 1)

## Code Structure

- `source/dsi/`: DSi-specific code
- `source/`: Core game logic
- `gfx/`: Graphics assets

## DSi Port Credits

This DSi port was created by **catzdsii** and is based on the 3DS port.
- **catzdsii** - DSi / DSi XL port with optimized raycasting engine
- **derrekr** - Original 3DS port foundation
