
Game:logWarn("Key pressed: ")
print(keyPressed)

if keyPressed == 5 then --W

	Scene:applyForce("entire_truck_node", 1, 0, 0)	

elseif keyPressed == 6 then --A



elseif keyPressed == 7 then --S

	Scene:applyForce("entire_truck_node", -1, 0, 0)

elseif keyPressed == 8 then --D



elseif keyPressed == 12 then --UP

	Scene:translateNode("camera_node", 1,0,0)

elseif keyPressed == 13 then --DOWN

	Scene:translateNode("camera_node", -1,0,0)

elseif keyPressed == 14 then --LEFT

	Scene:translateNode("camera_node", 0,0,1)

elseif keyPressed == 15 then --RIGHT

	Scene:translateNode("camera_node", 0,0,-1)

elseif keyPressed == 3 then --SPACE

	Scene:translateNode("camera_node", 0,1,0)

elseif keyPressed == 4 then --LSHIFT

	Scene:translateNode("camera_node", 0,-1,0)

end
