---@type number
local t = 0

---@type number
local dir = 0

---@type Opmode[]
Opmodes = {
	{
		name = "testOpmode",
		---@param time Time
		update = function(time)
			setPos(t)
			if dir == 1 then
				t = t + time.delta * 0.5
			else
				t = t - time.delta * 0.5
			end
			if t >= 1 then
				dir = 0
			elseif t < 0 then
				dir = 1
			end
		end,
	},
}