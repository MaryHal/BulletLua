theta = 0
function main()
   turn = getTurn()
   setVelocity(10 * math.sin(theta), 10 * math.cos(theta))
   theta = theta + 0.1
end
