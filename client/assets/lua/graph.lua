local Graph = {}
local Edge = require "assets/lua_scripts/edge"
local Vertex = require "assets/lua_scripts/vertex"
local List = require "assets/lua_scripts/linked_list"
local PQLinkedList = require "assets/lua_scripts/pq_linked_list"
local Iterator = require "assets/lua_scripts/linked_list_iterator"
local Queue = require "assets/lua_scripts/queue"
local PathNode = require "assets/lua_scripts/path_node"

function Graph.new()
   return {vertices = List.new()}
end

function Graph.size(graph)
   return graph.vertices.size
end

function Graph.addVertex(graph, value)
   local vertex = Vertex.new(value)
   List.add(graph.vertices, List.size(graph.vertices) + 1, vertex)
end

function Graph.findVertex(graph, value)
   iter = List.iterator(graph.vertices)

   index = 1
   while(Iterator.hasNext(iter)) do
      vertex = Iterator.next(iter)
      vertex_value = vertex.value

      if (value == vertex_value) then
         return index
      end
      index = index + 1
   end

   return -1  --not found
end

function Graph.addEdge(graph, start_value, end_value, weight)
   local start_index = Graph.findVertex(graph, start_value)
   local end_index = Graph.findVertex(graph, end_value)
   Graph.addEdgeByIndex(graph, start_index, end_index, weight)
end

function Graph.addEdgeByIndex(graph, start_index, end_index, weight)
   local vertex = Graph.getVertex(graph, start_index)
   local edge = Edge.new()
   edge.end_index = end_index
   edge.weight = weight
   List.add(vertex.edges, List.size(vertex.edges) + 1, edge)
end

function Graph.getVertex(graph, index)
   local vertex = List.get(graph.vertices, index)
   return vertex
end

function Graph.resetVertices(graph)
   local vertices = graph.vertices
   local iter = List.iterator(vertices)

   while (Iterator.hasNext(iter)) do
      vertex = Iterator.next(iter)
      vertex.marked = false
   end
end

function Graph.dfs(graph)
   local size = Graph.size(graph)
   local dfs_list = List.new()

   local vertices = graph.vertices
   local iter = List.iterator(vertices)

   --make sure to pick up all disconnected components
   while (Iterator.hasNext(iter)) do
      vertex = Iterator.next(iter)
      if (not vertex.marked) then
         vertex.marked = true;
         List.add(dfs_list, List.size(dfs_list) + 1, vertex)
         Graph.dfs_rec(graph, dfs_list, vertex)
      end
   end

   Graph.resetVertices(graph)
   return dfs_list
end

function Graph.dfs_rec(graph, dfs_list, vertex)
   local edges = vertex.edges
   local iter = List.iterator(edges)

   while (Iterator.hasNext(iter)) do
      local edge = Iterator.next(iter)
      local end_index = edge.end_index
      local vertex = Graph.getVertex(graph, end_index)
      if (not vertex.marked) then
         List.add(dfs_list, List.size(dfs_list) + 1, vertex)
         vertex.marked = true
         Graph.dfs_rec(graph, dfs_list, vertex)
      end
   end
end

function Graph.bfs(graph)

   local bfs_list = List.new()
   local bfs_queue = Queue.new()

   local vertices = graph.vertices
   local connected_iter = List.iterator(vertices)

   --make sure to pick up all disconnected components
   while (Iterator.hasNext(connected_iter)) do
      local connected_vertex = Iterator.next(connected_iter)

      if (not connected_vertex.marked) then
         connected_vertex.marked = true

         Queue.enqueue(bfs_queue, connected_vertex)

         while(not Queue.isEmpty(bfs_queue)) do
            local vertex = Queue.dequeue(bfs_queue)
            List.add(bfs_list, List.size(bfs_list) + 1, vertex)
            local edges = vertex.edges
            local iter = List.iterator(edges)
            while (Iterator.hasNext(iter)) do
               local edge = Iterator.next(iter)
               local end_index = edge.end_index
               local v_end = Graph.getVertex(graph, end_index)
               if (not v_end.marked) then
                  v_end.marked = true
                  Queue.enqueue(bfs_queue, v_end)
               end
             end
          end

      end
   end

   Graph.resetVertices(graph)
   return bfs_list

end

function Graph.findPath(graph, start_value, end_value)
   local path_pq = PQLinkedList.new()
   local vertices = graph.vertices

   local start_index = Graph.findVertex(graph, start_value)
   local end_index = Graph.findVertex(graph, end_value)

   local start_vertex = Graph.getVertex(graph, start_index)
   local start_path_node = PathNode.new(start_vertex, nil, 0)
   local end_vertex = Graph.getVertex(graph, end_index)

   PQLinkedList.add(path_pq, start_path_node)

   while(not PQLinkedList.isEmpty(path_pq)) do
      local path_node = PQLinkedList.remove(path_pq)
      local vertex = PathNode.getVertex(path_node)
      vertex.marked = true

            if (vertex.value == end_vertex.value) then
               local path_list = List.new()
               List.add(path_list, 1, end_vertex.value)
               path_node = PathNode.getParentNode(path_node)

               while (not (path_node == nil)) do
                  vertex = PathNode.getVertex(path_node)
                  List.add(path_list, 1, vertex.value)
                  path_node = PathNode.getParentNode(path_node)
               end

               Graph.resetVertices(graph)
               return path_list
            end

      local edges = vertex.edges
      local edge_iter = List.iterator(edges)

      while (Iterator.hasNext(edge_iter)) do
         local edge = Iterator.next(edge_iter)
         local end_index = edge.end_index
         local v_end = Graph.getVertex(graph, end_index)

         if (not v_end.marked) then
            local p_node = PathNode.new(v_end, path_node, PathNode.getCost(path_node) + Edge.getWeight(edge))
            PQLinkedList.add(path_pq, p_node)
         end
      end
   end

   Graph.resetVertices(graph)
   return nil

end

function Graph.print(graph)
   for i = 1, Graph.size(graph) do
      local vertex = Graph.getVertex(graph, i)
      Vertex.print(vertex, i)
   end
end

return Graph

