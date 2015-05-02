local PQLinkedList = {}
local Node = require "assets/lua_scripts/next_node"
local Iterator = require "assets/lua_scripts/linked_list_iterator"
local PathNode = require "assets/lua_scripts/path_node"

function PQLinkedList.find(pq_linked_list, value)
   local curr = pq_linked_list.head
   local prev = nil

   while (curr ~= nil and PathNode.compare(value, curr.value) >= 0) do
      prev = curr
      curr = curr.next
   end

   return prev
end

function PQLinkedList.new()
   return {head = nil, size = 0}
end

function PQLinkedList.isEmpty(pq_linked_list)
   return (PQLinkedList.size(pq_linked_list) == 0)
end

function PQLinkedList.size(pq_linked_list)
   return pq_linked_list.size
end

function PQLinkedList.add(pq_linked_list, value)

   local prev = PQLinkedList.find(pq_linked_list, value)
   local node = Node.new(value)

   if (prev == nil) then
      node.next = pq_linked_list.head
      pq_linked_list.head = node
   else
      curr = prev.next
      prev.next = node
      node.next = curr
   end

   size = pq_linked_list.size
   size = size + 1
   pq_linked_list.size = size

end

function PQLinkedList.remove(pq_linked_list)
   if (PQLinkedList.size(pq_linked_list) == 0) then return nil end

   local new_head = pq_linked_list.head.next
   local value = pq_linked_list.head.value
   pq_linked_list.head = new_head

   size = pq_linked_list.size
   size = size - 1
   pq_linked_list.size = size

   return value
end

function PQLinkedList.iterator(pq_linked_list)
   local iter = Iterator.new(pq_linked_list.head)
   return iter
end

return PQLinkedList
