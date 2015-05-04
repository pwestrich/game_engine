
CAMERA_NAME = "LuffyVision"	--name of camera to move
MOVE_AMOUNT = 5				--how far to move it
ROTATE_AMOUNT = 10			--how many degrees to rotate it

if keyPressed == 0 then 	 --INVALID

	--should never happen
	print("Invalid key pressed: " + keyPressed)

elseif keyPressed == 1 then --TAB

	--no action

elseif keyPressed == 2 then --ESC

	--quits the game, also should never happen

elseif keyPressed == 3 then --SPACE

	--rotate the camera about the y axis
	Scene:rotateCamera(CAMERA_NAME, -ROTATE_AMOUNT, 0, 1, 0)

elseif keyPressed == 4 then --LSHIFT

	--rotate the camera about the y axis the other way
	Scene:rotateCamera(CAMERA_NAME, ROTATE_AMOUNT, 0, 1, 0)

elseif keyPressed == 5 then --W

	--move the camera along the +z axis
	Scene:translateCamera(CAMERA_NAME, 0, 0, MOVE_AMOUNT)

elseif keyPressed == 6 then --A

	--move the camera along the +x axis
	Scene:translateCamera(CAMERA_NAME, MOVE_AMOUNT, 0, 0)

elseif keyPressed == 7 then --S

	--move the camera along the -z axis
	Scene:translateCamera(CAMERA_NAME, 0, 0, -MOVE_AMOUNT)

elseif keyPressed == 8 then --D

	--move the camera slong the -x axis
	Scene:translateCamera(CAMERA_NAME, -MOVE_AMOUNT, 0, 0)

elseif keyPressed == 9 then --Q

	--move the camera along the +y axis
	Scene:translateCamera(CAMERA_NAME, 0, MOVE_AMOUNT, 0)

elseif keyPressed == 10 then --Z

	--move the camera along the -y axis 
	Scene:translateCamera(CAMERA_NAME, 0, -MOVE_AMOUNT, 0)

elseif keyPressed == 11 then -- E

	--rotate the camera about the x axis
	Scene:rotateCamera(CAMERA_NAME, -ROTATE_AMOUNT, 1, 0, 0)

elseif keyPressed == 12 then --C

	--rotate the camera about the x axis the other way
	Scene:rotateCamera(CAMERA_NAME, ROTATE_AMOUNT, 1, 0, 0)

elseif keyPressed == 13 then --F

	--no action

elseif keyPressed == 14 then --UPARROW

	--no action

elseif keyPressed == 15 then --DOWNARROW

	--no action

elseif keyPressed == 16 then --LEFTARROW

	--no action

elseif keyPressed == 17 then --RIGHTARROW

	--no action

elseif keyPressed == 18 then --RETURN

	--no action

end
