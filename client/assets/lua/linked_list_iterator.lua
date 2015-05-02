local Iterator = {}
local Node = require "assets/lua_scripts/next_node"

function Iterator.new(head)
   return {curr = head}
end

function Iterator.hasNext(iterator)
   return (iterator.curr ~= nil)
end

function Iterator.next(iterator)

   if (Iterator.hasNext(iterator)) then
      local node = iterator.curr
      local value = node.value
      iterator.curr = node.next
      return value
   else
      return nil
   end

end

return Iterator
