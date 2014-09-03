-- BulletLua Test Script 3

function main()
   local turn = getTurn()

   d = math.fmod(turn, 6)
   if (d == 0) then
      fire(0, 1, curve)
      fire(90, 1, curve)
      fire(180, 1, curve)
      fire(270, 1, curve)

      fire(0, 3, curve2)
      fire(90, 3, curve2)
      fire(180, 3, curve2)
      fire(270, 3, curve2)
   end

   if (turn >= 300) then
      vanish()
   end
end

function curve()
   if (getSpeed() <= 1.5) then
      setSpeedRelative(0.06)
   end

   setDirectionRelative(3)

   if (getTurn() > 200) then
      vanish()
   end
end

function curve2()
   setDirectionRelative(3)

   if (getTurn() > 200) then
      vanish()
   end
end
