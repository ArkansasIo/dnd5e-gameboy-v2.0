@echo off
REM Build all C files in this folder into ide_editor.exe using gcc (MinGW or similar required)
setlocal
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c99 -I.
set SRC=*.c
%CC% %CFLAGS% -o ide_editor.exe %SRC%
endlocal
