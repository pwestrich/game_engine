local Queue = {}
local Node = require "assets/lua_scripts/next_node"

function Queue.new()
   return {tail = nil, size = 0}
end

function Queue.size(queue)
   return queue.size
end

function Queue.isEmpty(queue)
   return (queue.size == 0)
end

function Queue.dequeue(queue)

   if (queue.size == 0) then return nil end

   local tail = queue.tail
   local front = tail.next
   local after = front.next
   local item = front.value
   tail.next = after
   front = nil
   local size = queue.size
   queue.size = size - 1

   return item

end

function Queue.enqueue(queue, value)

   local back = Node.new(value)

   if (queue.size == 0) then
      queue.tail = back
      queue.tail.next = back
   else
      local tail = queue.tail
      local front = tail.next
      tail.next = back
      back.next = front
      queue.tail = back
   end

   local size = queue.size
   queue.size = size + 1

end

return Queue
