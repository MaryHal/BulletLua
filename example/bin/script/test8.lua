theta = 180

function main()
   turn = getTurn()

   if (math.fmod(turn, 2) == 0) then
      fire(theta, 10, shoot)
      theta = theta + 27
   end
end

function shoot()
   turn = getTurn()
   rank = getRank()

   dir = getDirection()
   fire(dir, 3.0 + 0.4 * rank, nullfunc)

   for i = 1, 10 do
      fire(dir + i * 30, 3.0 + 0.4 * rank, turnaround)
   end

   kill()
end

function turnaround()
   turn = getTurn()
   if (turn == 30) then
      setDirection(getDirection() + 180)
   end
end
