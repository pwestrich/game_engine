AutomatedMakefile = am
CC = g++ -std=c++11 -g -O0
RM = del

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project/server
PROJ_NAME = server.exe

INC_DIRS=-I./ -I$(GAME_DRIVE_LETTER)/Ogre/include/ -I$(GAME_DRIVE_LETTER)/boost/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/ -I$(GAME_DRIVE_LETTER)/Ogre/ogredeps/include/OIS -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_DRIVE_LETTER)/Ogre/lib/ -L$(GAME_DRIVE_LETTER)/boost/lib/ -L$(GAME_DRIVE_LETTER)/Ogre/ogredeps/lib/
LIBS= -lPocoNet -lPocoFoundation -lWS2_32

COMPILE = $(CC) $(INC_DIRS) -c 
LINK = $(CC) $(LIB_DIRS) -o

SERVER_FILES = main.cpp TCPServer.cpp TCPConnectionThread.cpp
SERVER_HEADER = TCPServer.h TCPConnectionThread.h
SERVER_OBJ = main.o TCPServer.o TCPConnectionThread.o

.PHONY: all run clean

clean:

	$(RM) $(SERVER_OBJ)

run: all

	./$(PROJ_NAME)

all: $(SERVER_OBJ)

	$(LINK) $(PROJ_NAME) $(SERVER_OBJ) $(LIBS)

main.o: main.cpp $(SERVER_HEADER)

	$(COMPILE) main.cpp

TCPServer.o: TCPServer.cpp $(SERVER_HEADER)

	$(COMPILE) TCPServer.cpp

TCPConnectionThread.o: TCPConnectionThread.cpp $(SERVER_HEADER)

	$(COMPILE) TCPConnectionThread.cpp
