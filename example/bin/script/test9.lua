function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

theta = 0

function trap(offset)
   -- tx, ty = getTargetPosition()

   x = 320 + 200 * math.sin(theta + math.rad(offset))
   y = 240 + 200 * math.cos(theta + math.rad(offset))

   setPosition(x, y)

   if (randIntRange(0, 400) == 0) then
      fireAtTarget(4, nullfunc)
   end

   theta = theta + 0.01
end

function main()
   turn = getTurn()

   for i = 0, 39 do
      print (i)
      trapi = bind(trap, i * 9)
      fire(0, 0, trapi)
   end

   kill()
end
