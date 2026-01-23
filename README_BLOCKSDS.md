# Building with BlocksDS

This project now supports building with [BlocksDS](https://blocksds.skylyrac.net/), an alternative SDK for Nintendo DS development.

## Prerequisites

1.  **Install BlocksDS**: Follow the [installation guide](https://blocksds.skylyrac.net/docs/introduction).
    *   Ensure the `BLOCKSDS` environment variable is set.
    *   **Windows (PowerShell)**:
        ```powershell
        $env:BLOCKSDS = "C:\path\to\blocksds\core"
        # Optional: Set external directory if different from default
        $env:BLOCKSDSEXT = "C:\path\to\blocksds\external"
        ```
    *   **Linux / WSL / MSYS2**:
        ```bash
        export BLOCKSDS=/opt/blocksds/core
        ```

2.  **Install Dependencies**:
    *   **Nitro Engine**: Required for 3D rendering features.
        ```bash
        # If using BlocksDS package manager (if available)
        wf-pacman -Sy blocksds-nitroengine
        # OR build and install from source: https://codeberg.org/SkyLyrac/nitro-engine
        ```
    *   You may need `libmm9` (Maxmod) installed in your BlocksDS environment.
    *   Check `$(BLOCKSDSEXT)/lib` for `libmm9.a`.

## Compilation

Run the following command in your terminal:

```bash
make -f Makefile.blocksds
```

This will create `srb2_dsi.nds` in the project root.

## Configuration

The build configuration is located in `Makefile.blocksds`. Key variables:

*   `SOURCES`: List of source directories.
*   `DEFINES`: Preprocessor definitions (same as original DSi port).
*   `LIBS`: Libraries to link (`mm9`, `nds9`, `fat`, `m`).
