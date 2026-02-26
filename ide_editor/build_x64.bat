@echo off
REM Build 64-bit ide_editor_x64.exe on Windows using MinGW-w64 x86_64 toolchain
setlocal

set CC=
set CC_MACHINE=

where x86_64-w64-mingw32-gcc >nul 2>nul
if not errorlevel 1 (
  set CC=x86_64-w64-mingw32-gcc
)

if not defined CC (
  if exist "D:\llvm-mingw-20260224-msvcrt-i686\llvm-mingw-20260224-msvcrt-i686\bin\x86_64-w64-mingw32-gcc.exe" (
    set CC=D:\llvm-mingw-20260224-msvcrt-i686\llvm-mingw-20260224-msvcrt-i686\bin\x86_64-w64-mingw32-gcc.exe
  )
)

if not defined CC (
where x86_64-w64-windows-gnu-gcc >nul 2>nul
if not errorlevel 1 (
  set CC=x86_64-w64-windows-gnu-gcc
)
)

if not defined CC (
  where gcc >nul 2>nul
  if not errorlevel 1 (
    for /f %%M in ('gcc -dumpmachine') do set CC_MACHINE=%%M
    echo %CC_MACHINE% | findstr /i "x86_64 amd64" >nul
    if not errorlevel 1 (
      set CC=gcc
    )
  )
)

if not defined CC (
  for %%P in (
    "C:\msys64\mingw64\bin\gcc.exe"
    "C:\mingw64\bin\gcc.exe"
    "C:\mingw-w64\bin\gcc.exe"
    "D:\llvm-mingw-20260224-msvcrt-x86_64\llvm-mingw-20260224-msvcrt-x86_64\bin\gcc.exe"
  ) do (
    if exist %%~fP (
      set CC=%%~fP
      goto :cc_found
    )
  )
)

:cc_found
if not defined CC (
  echo [build-x64] ERROR: no x86_64 MinGW compiler found.
  echo [build-x64] Install MinGW-w64 x86_64 and add it to PATH.
  echo [build-x64] Expected compiler examples:
  echo [build-x64]   x86_64-w64-windows-gnu-gcc
  echo [build-x64]   C:\msys64\mingw64\bin\gcc.exe
  exit /b 1
)

for /f %%M in ('"%CC%" -dumpmachine') do set CC_MACHINE=%%M
echo [build-x64] Using compiler: %CC%
echo [build-x64] Target machine: %CC_MACHINE%
echo %CC_MACHINE% | findstr /i "x86_64 amd64" >nul
if errorlevel 1 (
  echo [build-x64] ERROR: selected compiler is not 64-bit.
  echo [build-x64] Please use an x86_64 MinGW-w64 compiler.
  exit /b 1
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
    echo [build-x64] SDL2 detected, building with SDL2 support.
  ) else (
    echo [build-x64] SDL2 headers found but lib not found. Building without SDL2.
  )
) else (
  echo [build-x64] SDL2 not found. Building without SDL2.
)

"%CC%" %CFLAGS% -o ide_editor_x64.exe %SRC% %LDFLAGS%
if errorlevel 1 exit /b 1
echo [build-x64] Built ide_editor_x64.exe
endlocal
