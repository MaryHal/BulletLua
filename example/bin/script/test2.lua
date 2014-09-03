-- BulletLua Test Script 2

offset1 = 36.1
offset2 = -36.4

dir1 = offset1
dir2 = offset2

function main()
    local turn = getTurn()
    local rank = getRank()

    fire(dir1, 0.8, fade)
    fire(dir2, 0.8, fade)
    dir1 = dir1 + offset1
    dir2 = dir2 + offset2

    if (turn == 600) then
       kill()
    end
end

function fade()
    local turn = getTurn()
    if (turn == 130) then
       vanish()
    end
end
