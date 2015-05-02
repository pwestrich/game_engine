local Graph = require "assets/lua_scripts/graph"
local List = require "assets/lua_scripts/linked_list"
local Iterator = require "assets/lua_scripts/linked_list_iterator"

local function split(line, token)
   local tokens = {}

   local count = 1
   while (string.find(line, token)) do
      local j,k = string.find(line, token)
      tokens[count] = line:sub(1, j-1)
      len = #line
      line = line:sub(k+1, len)
      count = count + 1
   end

   tokens[count] = line
   return tokens

end

local function readVertices(graph, file)
   io.input(file)
   local num_vertices = io.read("*l")
   for i = 1, num_vertices do
      local value = io.read("*l")
      Graph.addVertex(graph, value)
   end
   io.input():close()
end

local function readEdges(graph, file)
   io.input(file)
   local num_edges = io.read("*l")

   for i = 1, num_edges do
      local entire_line = io.read("*l") 

      local tokens = split(entire_line, " ")
      start_value = tokens[1]
      end_value = tokens[2]
      weight = tonumber(tokens[3])

      Graph.addEdge(graph, start_value, end_value, weight)
   end

   io.input():close()
end

function graphTest(vertex_file, edge_file)
   graph = Graph.new()
   readVertices(graph, vertex_file)
   readEdges(graph, edge_file)

   --Graph.print(graph)
   dfs_list = Graph.dfs(graph)

   local dfs = {}
   count = 1
   iter = List.iterator(dfs_list)
   while(Iterator.hasNext(iter)) do
      vertex = Iterator.next(iter)
      dfs[count] = vertex.value
      count = count + 1
   end

   bfs_list = Graph.bfs(graph)

   local bfs = {}
   count = 1
   iter = List.iterator(bfs_list)
   while(Iterator.hasNext(iter)) do
      vertex = Iterator.next(iter)
      bfs[count] = vertex.value
      count = count + 1
   end

   return table.unpack(bfs)
end

function findPath(vertex_file, edge_file, start_value, end_value)
   local graph = Graph.new()

   readVertices(graph, vertex_file)
   readEdges(graph, edge_file)

   local path_list = Graph.findPath(graph, start_value, end_value)

   local path = {}
   local count = 1
   local iter = List.iterator(path_list)
   while (Iterator.hasNext(iter)) do
      local next_loc = Iterator.next(iter)
      path[count] = next_loc
      count = count + 1
   end
   
   return table.unpack(path)
end

--findPath("romanian_cities.txt","romanian_mileage.txt", "Arad", "Bucharest")
