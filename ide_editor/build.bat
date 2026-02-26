@echo off
REM Build all C files in this folder into ide_editor.exe using gcc (MinGW or similar required)
REM SDL2 must be in ..\third_party\SDL2 relative to this folder
setlocal
set CC=gcc
where gcc >nul 2>nul
if errorlevel 1 (
  if exist "D:\llvm-mingw-20260224-msvcrt-i686\llvm-mingw-20260224-msvcrt-i686\bin\gcc.exe" (
    set CC=D:\llvm-mingw-20260224-msvcrt-i686\llvm-mingw-20260224-msvcrt-i686\bin\gcc.exe
    echo [build] Using fallback compiler: D:\llvm-mingw-20260224-msvcrt-i686\llvm-mingw-20260224-msvcrt-i686\bin\gcc.exe
  ) else (
    echo [build] ERROR: gcc not found on PATH and fallback compiler path is missing.
    echo [build] Install MinGW-w64 or add gcc to PATH, then retry.
    exit /b 1
  )
)
set CFLAGS=-Wall -Wextra -std=c99 -I.
set SRC=*.c feature_autogen\*.c
set LDFLAGS=-luser32 -lgdi32
set TMP=%CD%\build_tmp
set TEMP=%CD%\build_tmp
if not exist "%TMP%" mkdir "%TMP%"

if exist "..\third_party\SDL2\include\SDL.h" (
  if exist "..\third_party\SDL2\lib\libSDL2.a" (
    set CFLAGS=%CFLAGS% -I..\third_party\SDL2\include
    set LDFLAGS=-L..\third_party\SDL2\lib -lSDL2 %LDFLAGS%
    echo [build] SDL2 detected, building with SDL2 support.
  ) else (
    echo [build] SDL2 headers found but lib not found. Building without SDL2.
  )
) else (
  echo [build] SDL2 not found. Building without SDL2.
)

%CC% %CFLAGS% -o ide_editor.exe %SRC% %LDFLAGS%
endlocal
