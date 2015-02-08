AutomatedMakefile = am
CC = g++ 

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I./tinyxml/include/ -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/tinyxml/lib/
LIBS=-lboost_system-mgw48-mt-1_57 -lOgreMain -ltinyxml

COMPILE = $(CC) $(INC_DIRS) -c 
LINK = $(CC) $(LIB_DIRS) -o

GAME_SOURCE = 
GAME_OBJ = 

.PHONY: run_quat run_game

run_game: link_game

	./game.exe

link_game:

	$(LINK) game.exe GAME_OBJ

compile_game: $(GAME_SOURCE)

	$(COMPILE) GAME_SOURCE

run_quat: 

	mingw32-make -C quaternion
	./quaternion/quaternion.exe
