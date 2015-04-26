@echo off
cls

set DRIVE_LETTER=%1:
set GAME_PATH=%DRIVE_LETTER%
set PATH=%DRIVE_LETTER%\MinGW\bin;%DRIVE_LETTER%\MinGW\msys\1.0\bin;%GAME_PATH%\Ogre\bin;%GAME_PATH%\boost\bin;%GAME_PATH%\Ogre\ogredeps\bin;c:\Windows;c:\Windows\system32

mingw32-make -j 4 GAME_DRIVE_LETTER="%DRIVE_LETTER%"
