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

```
source/dsi/
├── i_main.c          # Main entry point
├── i_system.c        # System interface (memory, timers, etc.)
├── i_video.c         # Video interface (initialization, buffers)
├── i_sound.c         # Audio interface (maxmod)
├── i_net.c           # Network interface (not supported)
├── i_cdmus.c         # CD music interface (not supported)
├── r_raycast.c/h     # Raycasting engine (Doom-style)
├── dsi_sprite.c/h    # 2D sprite management (billboards)
├── dsi_texture.c/h   # Palettized texture management
├── dsi_collision.c/h # AABB collision detection
└── dsi_utils.c/h     # Utility functions
```

## Optimizations

### Memory

- Textures limited to 128x128 pixels
- Texture cache limited to 32 textures
- Sprites limited to 32 simultaneous
- Dynamic memory allocation with strict management

### Performance

- Optimized raycasting with DDA (Digital Differential Analyzer)
- Limit of 2000 polygons per scene
- No post-processing
- No split-screen
- No network

### Graphics

- Fixed resolution 256x192
- 16-bit color (RGB555)
- 8-bit palettized textures
- 2D sprites for characters

## Known Limitations

- No network support (no netplay)
- No split-screen
- No MIDI (uses .it or .wav only)
- Only Greenflower Zone Act 1
- No water reflections
- Static sky (no scrolling)
- Maximum 5-10 active enemies

## Troubleshooting

### Error: "Impossible d'initialiser FAT!"

- Check that your SD card is properly inserted
- Make sure Twilight Menu++ is installed
- Verify the SD card is formatted as FAT32

### Game is too slow

- Reduce the number of active enemies
- Simplify textures (reduce resolution)
- Disable some visual effects

### Sound doesn't work

- Verify that maxmod-nds is installed
- Make sure audio files are in the correct format (.it or .wav)

## Credits

- **Sonic Team Junior** for original SRB2
- **devkitPro team** for development tools
- **DS/DSi homebrew community** for support and resources

## DSi Port Credits

This DSi port was created by **catzdsii** and is based on the 3DS port. It uses a simplified raycasting engine optimized for DSi hardware constraints.

### Author
- **catzdsii** - DSi / DSi XL port with optimized raycasting engine

### Based on
- SRB2 3DS port by derrek and STJr team
- Original SRB2 by Sonic Team Junior

## License

This project is licensed under GPL v2, same as original SRB2.

