
-- BulletLua Test Script 10

function bind(f,...)
   local args = {...}
   return function(...)
      return f(unpack(args),...)
   end
end

function runTable(commands)
   turn = getTurn()
   cmd = commands[turn]

   if cmd ~= nil then
      cmd()
   end
end

function main()
   commands = {}
   commands[30] = bind(setSpeed, 2)
   commands[60] = vanish

   setFunction(bind(runTable, commands))
end
