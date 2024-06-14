local M = {}

-- [[ Global enums ]]
-- -- zero_timeout_behavior
-- UseDefault = {}
-- NeverExpire = {}
--
-- -- focus_follows
-- Mouse = {}
-- Window = {}
--
-- Not using these because it's a bit complicated to implement;
-- I'll use strings instead

-- Anchors
TL = { 'TL' }
TM = { 'TM' }
TR = { 'TR' }
ML = { 'ML' }
MM = { 'MM' }
MR = { 'MR' }
BL = { 'BL' }
BM = { 'BM' }
BR = { 'BR' }

-- [[ Helper functions ]]
function M.log(...)
    local args = {...}
    for i, v in ipairs(args) do
        if type(v) == 'table' then
            for k, v1 in pairs(v) do print(k, v1) end
        else
            print(v)
        end
    end
end

-- [[ Blocks ]]
M.block = {}

return M
