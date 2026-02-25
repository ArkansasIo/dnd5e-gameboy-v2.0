# Building the IDE Editor on Windows

## Option 1: Using build.bat (Recommended for Windows)
1. Install [MinGW-w64](https://www.mingw-w64.org/) or [TDM-GCC](https://jmeubank.github.io/tdm-gcc/) to get gcc.
2. Open a Command Prompt in this folder.
3. Run:

    build.bat

This will produce ide_editor.exe if gcc is installed and in your PATH.

## Option 1B: Build 64-bit Windows executable
1. Install MinGW-w64 **x86_64** toolchain.
2. Ensure the 64-bit compiler is available (for example `x86_64-w64-windows-gnu-gcc`).
3. Run:

    build_x64.bat

This produces `ide_editor_x64.exe` and validates that the compiler target is 64-bit.

## Option 2: Using GNU Make (MSYS2/WSL)
1. Install [MSYS2](https://www.msys2.org/) or [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/).
2. Open the MSYS2/WSL shell in this folder.
3. Run:

    make

This will produce an executable (ide_editor or ide_editor.exe).

## Troubleshooting
- If you get 'gcc not found', ensure MinGW-w64 or TDM-GCC is installed and added to your PATH.
- If `build_x64.bat` says compiler is not 64-bit, install MinGW-w64 x86_64 and use that compiler path.
- If you get 'make not found', install MSYS2 and run 'pacman -S make gcc' in the MSYS2 shell.
