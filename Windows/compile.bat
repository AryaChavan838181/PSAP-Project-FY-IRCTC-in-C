@echo off
gcc main.c -o main.exe ^
-I"include" ^
-L"lib" ^
-lcurl ^
-lws2_32 ^
-lwinmm

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
) else (
    echo Compilation failed!
)
