function twosComp(input, pointer)
	local result
	
	if (input == "0") then 
		result = "0"
	else
		nextState(1, pointer)
		result = "1"
	end
	
	return result
end
