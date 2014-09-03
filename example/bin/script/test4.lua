-- BulletLua Test Script

function main()
   turn = getTurn()
   setDirection(0)

   if (math.fmod(turn, 30) == 0) then
      setPosition(randIntRange(200, 440), 488)
      fire(randIntRange(-20, 20), randFloat() + 6.5, launch)
   end
end

function launch()
   turn = getTurn()
   setSpeedRelative(-0.07)
   if (turn == 60) then
      for d = 0, 360, 360/30 do
         fire(d, randFloatRange(2, 5), fade)
         kill()
      end
   end
end

function fade()
   if (getTurn() == 30) then
      vanish()
   end
end
