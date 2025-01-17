AutomatedMakefile = am
CC = g++ -std=c++11
RM = del

GAME_PATH=./

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/OIS -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/Ogre/ogredeps/lib/
LIBS=-lboost_system-mgw48-mt-1_57 -lboost_chrono-mgw48-1_57 -lboost_thread-mgw48-mt-1_57 -ltinyxml -lOIS -lbass -llua -lMyGUI.OgrePlatform -lMyGUIEngine -lOgreMain -lBulletMultiThreaded -lBulletDynamics -lBulletCollision -lLinearMath -lPocoNet -lPocoFoundation -lWS2_32 -lfreetype

COMPILE = $(CC) $(INC_DIRS) -O3 -c 
LINK = $(CC) $(LIB_DIRS) -s -O3 -o

GAME_NAME = game.exe
GAME_HEADER = GameManager.h RenderManager.h utilities.h RenderListener.h LogManager.h ResourceManager.h InputManager.h InputListener.h AudioManager.h AudioPlayer.h GameResource.h PathResource.h MeshResource.h AudioResource.h GUIManager.h ScriptManager.h PhysicsManager.h BulletMotionState.h BulletDebugDrawer.h BulletConvexHullCreator.h NetworkManager.h
GAME_SOURCE = main.cpp GameManager.cpp RenderManager.cpp utilities.cpp RenderListener.cpp LogManager.cpp ResourceManager.cpp InputManager.cpp AudioManager.cpp AudioPlayer.cpp GameResource.cpp PathResource.cpp MeshResource.cpp AudioResource.cpp GUIManager.cpp ScriptManager.cpp PhysicsManager.cpp BulletMotionState.cpp BulletDebugDrawer.cpp BulletConvexHullCreator.cpp NetworkManager.cpp
GAME_OBJ = main.o GameManager.o RenderManager.o utilities.o RenderListener.o LogManager.o ResourceManager.o InputManager.o AudioManager.o AudioPlayer.o GameResource.o PathResource.o MeshResource.o AudioResource.o GUIManager.o ScriptManager.o PhysicsManager.o BulletMotionState.o BulletDebugDrawer.o BulletConvexHullCreator.o NetworkManager.o

.PHONY: run all clean

clean:

	$(RM) $(GAME_OBJ)

run: all

	./$(GAME_NAME)

all: link 

link: $(GAME_OBJ)

	$(LINK) $(GAME_NAME) $(GAME_OBJ) $(LIBS)

main.o: main.cpp $(GAME_HEADER)

	$(COMPILE) main.cpp
	
GameManager.o: GameManager.cpp $(GAME_HEADER)

	$(COMPILE) GameManager.cpp

InputManager.o: InputManager.cpp $(GAME_HEADER)

	$(COMPILE) InputManager.cpp

LogManager.o: LogManager.cpp $(GAME_HEADER)

	$(COMPILE) LogManager.cpp

ResourceManager.o: ResourceManager.cpp $(GAME_HEADER)

	$(COMPILE) ResourceManager.cpp

GameResource.o: GameResource.cpp $(GAME_HEADER)

	$(COMPILE) GameResource.cpp

PathResource.o: PathResource.cpp $(GAME_HEADER)

	$(COMPILE) PathResource.cpp

MeshResource.o: MeshResource.cpp $(GAME_HEADER)

	$(COMPILE) MeshResource.cpp

AudioResource.o: AudioResource.cpp $(GAME_HEADER)

	$(COMPILE) AudioResource.cpp
	
RenderManager.o: RenderManager.cpp $(GAME_HEADER)

	$(COMPILE) RenderManager.cpp

RenderListener.o: RenderListener.cpp $(GAME_HEADER)

	$(COMPILE) RenderListener.cpp

GUIManager.o: GUIManager.cpp $(GAME_HEADER)

	$(COMPILE) GUIManager.cpp

AudioManager.o: AudioManager.cpp $(GAME_HEADER)

	$(COMPILE) AudioManager.cpp

AudioPlayer.o: AudioPlayer.cpp $(GAME_HEADER)

	$(COMPILE) AudioPlayer.cpp

ScriptManager.o: ScriptManager.cpp LuaContext.hpp $(GAME_HEADER)

	$(COMPILE) ScriptManager.cpp

PhysicsManager.o: PhysicsManager.cpp $(GAME_HEADER)

	$(COMPILE) PhysicsManager.cpp

BulletMotionState.o: BulletMotionState.cpp $(GAME_HEADER)

	$(COMPILE) BulletMotionState.cpp

BulletDebugDrawer.o: BulletDebugDrawer.cpp $(GAME_HEADER)

	$(COMPILE) BulletDebugDrawer.cpp

BulletConvexHullCreator.o: BulletConvexHullCreator.cpp $(GAME_HEADER)

	$(COMPILE) BulletConvexHullCreator.cpp

NetworkManager.o: NetworkManager.cpp $(GAME_HEADER)

	$(COMPILE) NetworkManager.cpp

utilities.o: utilities.cpp utilities.h

	$(COMPILE) utilities.cpp

