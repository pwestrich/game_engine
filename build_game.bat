@echo off
cls

set DRIVE_LETTER=C:
set GAME_PATH=%DRIVE_LETTER%
set PATH=%DRIVE_LETTER%\MinGW\bin;%DRIVE_LETTER%\MinGW\msys\1.0\bin;%GAME_PATH%\Ogre\bin;%GAME_PATH%\boost\bin;%GAME_PATH%\Ogre\ogredeps\bin;%GAME_PATH%\bass_2.4;%GAME_PATH%\MyGUI_3.2.2\bin;%GAME_PATH%\lua_5.3.0\bin;c:\Windows;c:\Windows\system32

mingw32-make GAME_DRIVE_LETTER="%DRIVE_LETTER%"
