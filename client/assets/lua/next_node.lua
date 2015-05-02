local Node = {}

function Node.new(v)
   return {next = nil, value = v}
end

return Node

