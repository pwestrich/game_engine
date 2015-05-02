local Vertex = {}
local Edge = require "assets/lua_scripts/edge"
local List = require "assets/lua_scripts/linked_list"

function Vertex.new(v)
   return {edges = List.new(), value = v, marked = false}
end

function Vertex.print(vertex, j)
   local edges = vertex.edges
   io.write(vertex.value, "\n")
   
   for i = 1, List.size(edges) do
      local edge = List.get(edges, i)
      Edge.print(edge, j)
   end
end

function Vertex.printValue(vertex)
   io.write(vertex.value, "\n")
end

return Vertex
