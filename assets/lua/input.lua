
moveAmount = 5

if keyPressed == 3 then --SPACE

	Scene:translateNode("camera_node", 0,moveAmount,0)

elseif keyPressed == 4 then --LSHIFT

	Scene:translateNode("camera_node", 0,-moveAmount,0)

elseif keyPressed == 5 then --W

	Scene:translateNode("camera_node", moveAmount,0,0)	

elseif keyPressed == 6 then --A

	Scene:translateNode("camera_node", 0,0,-moveAmount)

elseif keyPressed == 7 then --S

	Scene:translateNode("camera_node", -moveAmount,0,0)		

elseif keyPressed == 8 then --D

	Scene:translateNode("camera_node", 0,0,moveAmount)

elseif keyPressed == 11 then -- F

	Game:playAudioByID(moveAmount0, moveAmount)

elseif keyPressed == 12 then --UP



elseif keyPressed == 13 then --DOWN

	

elseif keyPressed == 14 then --LEFT

	

elseif keyPressed == 15 then --RIGHT

	

end
