gwait = 20

function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

function main()
   circle = bind(fireCircle, 40)
   setPosition(320, 240)
   setFunction(bind(go, 10))
end

function go(wait)
   local turn = getTurn()
   if (math.fmod(turn, wait) == 0) then
      -- speeds = {4, 5, 6}
      circle(21 - gwait, curve)

      setFunction(bind(go, gwait))
      gwait = gwait - 1
      if gwait < 18 then
         gwait = 20
      end
   end
end

function curve()
   local turn = getTurn()
   setDirectionRelative(5)
   setSpeedRelative(0.1)

   if (turn > 180) then
      vanish()
   end
end
