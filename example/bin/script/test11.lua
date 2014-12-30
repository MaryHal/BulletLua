theta = 0
prevTheta = 0
radius = 100

-- Time in frames
reactivateTime = 50
travelTime = 4
freezeTime = 60

function main()
   if (math.fmod(getTurn() + 10, reactivateTime) == 0) then
      theta = randFloatRange(prevTheta + 1.6, prevTheta + 6.28 - 1.6)
      prevTheta = theta
      for i = 0, 40 do
         fire(0, 0, surround)
      end
   end
end

function surround()
   setCollision(false)
   tx, ty = getTargetPosition()

   nx = tx + radius * math.sin(theta);
   ny = ty + radius * math.cos(theta)
   theta = theta + 0.04

   linearInterpolate(nx, ny, travelTime)
   setFunction(freeze)
end

function freeze()
   if (getTurn() == travelTime) then
      setCollision(true)
      setSpeed(0)
      aimTarget()
      setFunction(launch)
   end
end

function launch()
   if (getTurn() == freezeTime) then
      setSpeed(8)
   end
end
