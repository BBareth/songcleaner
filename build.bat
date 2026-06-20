@echo off
setlocal enabledelayedexpansion
set GEODE_SDK=C:\Users\exilium\geode-sdk
cd /d C:\Users\exilium\geode-mods\songcleaner
mkdir build 2>nul
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
