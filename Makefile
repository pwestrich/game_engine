AutomatedMakefile = am

GAME_PATH=./

.PHONY: run_server run_quat run_game all

#clean:

#	mingw32-make -C client clean
#	mingw32-make -C server clean
#	mingw32-make -C quaternion clean

all: link_game link_server link_quat

run_game:

	mingw32-make -C client run

run_server:

	mingw32-make -C server run

run_quat: link_quat

	./quaternion/quaternion.exe

link_server:

	mingw32-make -C server all

link_game: $(GAME_OBJ) $(GAME_HEADER)

	mingw32-make -C client all

link_quat: 

	mingw32-make -C quaternion
