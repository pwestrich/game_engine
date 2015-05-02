local List = {}
local Node = require "assets/lua_scripts/next_node"
local Iterator = require "assets/lua_scripts/linked_list_iterator"

function List.new()
   return {head = nil, size = 0}
end

function List.size(list)
   return list.size
end

function List.get(list, index)

   if (index < 1 or index > List.size(list)) then return nil end
   local curr = List.find(list, index)
   return curr.value

end

function List.add(list, index, value)

   local sz = list.size
   if (index < 1 or index > (sz + 1)) then return end

   local node = Node.new(value)
   list.size  = sz + 1

   if (index == 1) then

      node.next = list.head
      list.head = node

   else

      local prev = List.find(list, index-1)
      local curr = prev.next

      prev.next = node
      node.next = curr
      
   end

end

function List.find(list, index)

   local curr = list.head
   local count = 1

   while (count < index) do
       curr = curr.next
       count = count + 1
   end

   return curr

end

function List.iterator(list)
   local iter = Iterator.new(list.head)
   return iter
end

return List
