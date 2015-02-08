AutomatedMakefile = am
CC = g++ 

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I$(GAME_DRIVE_LETTER)/tinyxml/include/ -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/tinyxml/lib/
LIBS=-lboost_system-mgw48-mt-1_57 -lOgreMain -ltinyxml

COMPILE = $(CC) $(INC_DIRS) -c 
LINK = $(CC) $(LIB_DIRS) -o
QUAT_FILES = Quaternion.o

Quaternion:		$(QUAT_FILES)
			$(LINK) Quaternion.exe $(QUAT_FILES)

Quaternion.o:		Quaternion.h Quaternion.cpp
			$(COMPILE) Quaternion.cpp
