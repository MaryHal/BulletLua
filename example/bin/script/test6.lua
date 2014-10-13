-- BulletLua Test Script 5

function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

function twirl(dir)
   setDirectionRelative(dir * 2)
   setSpeedRelative(0.075)
end

twirlcw  = bind(twirl, 1)
twirlccw = bind(twirl, -1)

function burst()
   for initialDirection = 1, 360/12 do
      fire(initialDirection * 12, 3, twirlcw)
      fire(initialDirection * 12, 3, twirlccw)

      fire(initialDirection * 12, 2.3, twirlcw)
      fire(initialDirection * 12, 2.3, twirlccw)
   end
   kill()
end

function launch()
   turn = getTurn()

   if (turn < 60) then
      setDirectionRelative(3)
      setSpeedRelative(0.05)
   elseif (turn == 60) then
      setFunction(burst)
   end
end

targetDir = 0

function main()
   turn = getTurn()
   rank = getRank()

   if (math.fmod(turn, math.floor(75 - 30 * rank)) == 0) then
      fire(randFloatRange(0, 90), 1.5, launch)
   end
end
