@echo off
rem Double-click to build the game: writes dist\DragonQuestIV.exe (see README.md).
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1"
pause
