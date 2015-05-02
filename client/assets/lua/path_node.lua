local PathNode = {}
local Vertex = require "assets/lua_scripts/vertex"

function PathNode.new(v, p, c)
   return {vertex = v, parent = p, cost = c}
end

function PathNode.getVertex(path_node)
   return path_node.vertex
end

function PathNode.getParentNode(path_node)
   return path_node.parent
end

function PathNode.getCost(path_node)
   return path_node.cost
end

function PathNode.compare(pn1, pn2)
	local cost1 = PathNode.getCost(pn1)
	local cost2 = PathNode.getCost(pn2)
    local result = nil

	return cost1 - cost2
end

return PathNode

