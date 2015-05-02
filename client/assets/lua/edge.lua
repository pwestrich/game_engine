local Edge = {}

function Edge.new()
   return {end_index = nil, weight = nil}
end

function Edge.print(edge, j)
   io.write("  (" , j , "," , edge.end_index , " = " , edge.weight , ") \n")
end

function Edge.getWeight(edge)
	return edge.weight
end

return Edge





