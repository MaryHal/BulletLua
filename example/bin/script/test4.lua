-- BulletLua Test Script

function main()
   turn = getTurn()
   setDir(0)

   if (math.fmod(turn, 40) == 0) then
      setPos(randIntRange(100, 220), 248)
      fire(randIntRange(-15, 15), randFloat() + 4.5, "launch")
   end
end

function launch()
   turn = getTurn()
   setSpeedRel(-0.07)
   if (turn == 60) then
      for d = 0, 360, 360/40 do
         fire(d, randFloatRange(2, 5), "fade")
         kill()
      end
   end
end

function fade()
   if (getTurn() == 20) then
      vanish()
   end
end
