AutomatedMakefile = am
CC = g++ -std=c++11 -O3
RM = del

GAME_PATH=/$(GAME_DRIVE_LETTER)/game_project

INC_DIRS=-I./
LIB_DIRS=-L./ 
LIBS=

COMPILE = $(CC) $(INC_DIRS) -c 
LINK = $(CC) $(LIB_DIRS) -o
QUAT_FILES = Quaternion.o

#.PHONY: clean

#clean:

#	$(RM) $(QUAT_FILES)

Quaternion:		$(QUAT_FILES)
			$(LINK) Quaternion.exe $(QUAT_FILES)

Quaternion.o:		Quaternion.h Quaternion.cpp
			$(COMPILE) Quaternion.cpp
