@echo off
cls

set DRIVE_LETTER=%1:
set GAME_PATH=.
set PATH=%DRIVE_LETTER%\MinGW\bin;%DRIVE_LETTER%\MinGW\msys\1.0\bin;%DRIVE_LETTER%\Ogre\bin;%DRIVE_LETTER%\boost\bin;%DRIVE_LETTER%\Ogre\ogredeps\bin;c:\Windows;c:\Windows\system32

mingw32-make GAME_DRIVE_LETTER="%DRIVE_LETTER%"