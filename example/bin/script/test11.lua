theta = 0
radius = 100

function main()
   if (math.fmod(getTurn(), 50) == 0) then
      for i = 0, 30 do
         theta = randFloatRange(0, 6.28)
         fire(0, 0, surround)
      end
   end
end

function surround()
   tx, ty = getTargetPosition()

   nx = tx + radius * math.sin(theta);
   ny = ty + radius * math.cos(theta)
   theta = theta + 0.05

   linearInterpolate(nx, ny, 3)
   setFunction(freeze)
end

function freeze()
   if (getTurn() == 3) then
      setSpeed(0)
      aimTarget()
      setFunction(launch)
   end
end

function launch()
   if (getTurn() == 60) then
      setSpeed(8)
   end
end
