AutomatedMakefile = am
CC = g++ -std=c++11

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/OIS -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/Ogre/ogredeps/lib/
LIBS=-lboost_system-mgw48-mt-1_57 -lOgreMain -ltinyxml -lOIS -lbass

COMPILE = $(CC) $(INC_DIRS) -g -O0 -c 
LINK = $(CC) $(LIB_DIRS) -g -O0 -o

GAME_SOURCE = main.cpp GameManager.cpp RenderManager.cpp utilities.cpp RenderListener.cpp LogManager.cpp ResourceManager.cpp InputManager.cpp
GAME_OBJ = main.o GameManager.o RenderManager.o utilities.o RenderListener.o LogManager.o ResourceManager.o InputManager.o

.PHONY: run_quat run_game

run_game: link_game

	./game.exe

link_game: $(GAME_OBJ)

	$(LINK) game.exe $(GAME_OBJ) $(LIBS)
	
main.o: main.cpp

	$(COMPILE) main.cpp
	
GameManager.o: GameManager.cpp GameManager.h

	$(COMPILE) GameManager.cpp

InputManager.o: InputManager.cpp InputManager.h

	$(COMPILE) InputManager.cpp

LogManager.o: LogManager.cpp LogManager.h

	$(COMPILE) LogManager.cpp

ResourceManager.o: ResourceManager.cpp ResourceManager.h

	$(COMPILE) ResourceManager.cpp
	
RenderManager.o: RenderManager.cpp RenderManager.h

	$(COMPILE) RenderManager.cpp

RenderListener.o: RenderListener.cpp RenderListener.h

	$(COMPILE) RenderListener.cpp

utilities.o: utilities.cpp utilities.h

	$(COMPILE) utilities.cpp

run_quat: 

	mingw32-make -C quaternion
	./quaternion/quaternion.exe
