# Building the IDE Editor on Windows

## Option 1: Using build.bat (Recommended for Windows)
1. Install [MinGW-w64](https://www.mingw-w64.org/) or [TDM-GCC](https://jmeubank.github.io/tdm-gcc/) to get gcc.
2. Open a Command Prompt in this folder.
3. Run:

    build.bat

This will produce ide_editor.exe if gcc is installed and in your PATH.

## Option 2: Using GNU Make (MSYS2/WSL)
1. Install [MSYS2](https://www.msys2.org/) or [Windows Subsystem for Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/).
2. Open the MSYS2/WSL shell in this folder.
3. Run:

    make

This will produce an executable (ide_editor or ide_editor.exe).

## Troubleshooting
- If you get 'gcc not found', ensure MinGW-w64 or TDM-GCC is installed and added to your PATH.
- If you get 'make not found', install MSYS2 and run 'pacman -S make gcc' in the MSYS2 shell.
