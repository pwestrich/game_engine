AutomatedMakefile = am
CC = g++ -std=c++11

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I./tinyxml/include/ -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/tinyxml/lib/
LIBS=-lboost_system-mgw48-mt-1_57 -lOgreMain -ltinyxml

COMPILE = $(CC) $(INC_DIRS) -g -O0 -c 
LINK = $(CC) $(LIB_DIRS) -g -O0 -o

GAME_SOURCE = main.cpp GameManager.cpp RenderManager.cpp utilities.cpp RenderListener.cpp AnimationRenderListener.cpp LogManager.cpp
GAME_OBJ = main.o GameManager.o RenderManager.o utilities.o RenderListener.o AnimationRenderListener.o LogManager.o

.PHONY: run_quat run_game

run_game: link_game

	./game.exe

link_game: $(GAME_OBJ)

	$(LINK) game.exe $(GAME_OBJ) $(LIBS)
	
main.o: main.cpp

	$(COMPILE) main.cpp
	
GameManager.o: GameManager.cpp GameManager.h

	$(COMPILE) GameManager.cpp

LogManager.o: LogManager.cpp LogManager.h

	$(COMPILE) LogManager.cpp
	
RenderManager.o: RenderManager.cpp RenderManager.h

	$(COMPILE) RenderManager.cpp

RenderListener.o: RenderListener.cpp RenderListener.h

	$(COMPILE) RenderListener.cpp

AnimationRenderListener.o: AnimationRenderListener.cpp AnimationRenderListener.h

	$(COMPILE) AnimationRenderListener.cpp

utilities.o: utilities.cpp utilities.h

	$(COMPILE) utilities.cpp

run_quat: 

	mingw32-make -C quaternion
	./quaternion/quaternion.exe
