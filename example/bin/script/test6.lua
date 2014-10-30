-- BulletLua Test Script 6

function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

function twirl(dir)
   turn = getTurn()

   setDirectionRelative(dir * 2)
   setSpeedRelative(0.08)

   if (turn == 90) then
      vanish()
   end
end

twirlcw  = bind(twirl, 1)
twirlccw = bind(twirl, -1)

function burst()
   divs = 15
   for initialDirection = 1, 360/divs do
      fire(initialDirection * divs, 3.7, twirlcw)
      fire(initialDirection * divs, 3.7, twirlccw)

      fire(initialDirection * divs, 3.0, twirlcw)
      fire(initialDirection * divs, 3.0, twirlccw)

      fire(initialDirection * divs, 2.3, twirlcw)
      fire(initialDirection * divs, 2.3, twirlccw)
   end
   kill()
end

function launch()
   turn = getTurn()

   if (turn < 60) then
      setDirectionRelative(3)
      setSpeedRelative(0.05)

      -- vx, vy = getVelocity()
      -- s = getSpeed()
      -- d = getDirection()
      -- print (turn, vx, vy, s, d)
   elseif (turn == 60) then
      burst()
   end
end

shotCount = 0
initialDirections = {0, 40, 80, 120, 160}

function main()
   turn = getTurn()

   if (math.fmod(turn, 120) == 0) then
      -- Cycle between directions
      direction = initialDirections[math.fmod(shotCount, #initialDirections) + 1]
      shotCount = shotCount + 1
      fire(direction, 1.5, launch)
   end
end
