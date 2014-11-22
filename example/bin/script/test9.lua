function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

centerX = 320
centerY = 240

radius = 200
theta = 0

function trap(offset)
   -- tx, ty = getTargetPosition()

   x = centerX + radius * math.sin(theta + math.rad(offset))
   y = centerY + radius * math.cos(theta + math.rad(offset))

   setPosition(x, y)

   if (randIntRange(0, 400) == 0) then
      fireAtTarget(4, launch)
   end

   theta = theta + 0.01
end

function launch()
   thisX, thisY = getPosition()

   dx = thisX - centerX
   dy = thisY - centerY

   -- We should test a tiny bit past the bounds of our circle because on some frames,
   -- on launch, this bullet will immediately be within this boundary.
   radiusError = 8

   -- Vanish when bullet leaves trapping circle. Thank you, Pythagoras.
   if (dx*dx + dy*dy > radius*radius+radiusError) then
      vanish()
   end
end

function main()
   turn = getTurn()

   for i = 1, 90 do
      trapi = bind(trap, i * 4)
      fire(0, 0, trapi)
   end

   kill()
end
