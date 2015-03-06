
function main()
   turn = getTurn()
   rank = getRank()
   if (turn % math.floor(120 * (1.2 - rank)) == 0) then
      fire(180, 4, shoot)
      -- fire(160, 3.5, shoot)
      -- fire(200, 4.5, shoot)
   end
end

function shoot()
   aimTarget()
   if (getTurn() == 40) then
      fireCircle(40, 8, blaze)
      kill()
   end
end

function blaze()
   setDirectionRelative(8)

   if (getTurn() == 20) then
      vanish()
   end
end
