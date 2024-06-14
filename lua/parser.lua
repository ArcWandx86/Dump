-- Parses a wired.lua config, and automatically generates a wired.ron

-- [[ Setup ]]
local b -- pcall status

-- Initialize api
local wired
b, wired = pcall(require, 'api')
if not b then error('Could not find wired lua api') end

local default, user_config
-- Get default config
b, default = pcall(require, 'default')
if not b then error('Could not load default config') end
-- Get user config
b, user_config = pcall(require, 'wired')
user_config = b and user_config or {}

-- Initialize output file
local out = arg[1] and io.open(arg[1], 'w') or io.stdout
if not out then error('Failed to open output file') end

-- Values in table a take priority
local function merge_config(t1, t2)
    local ret = t2
    for k, v in pairs(t1) do
        ret[k] = v
    end
    return ret
end

local get_type = {
    max_notifications = 'number',
    timeout = 'number',
    zero_timeout_behavior = 'Enum',
    poll_interval = 'number',
    idle_poll_interval = 'number',
    idle_threshold = 'number',
    notifications_spawn_paused = 'boolean',
    unpause_on_input = 'boolean',
    replacing_enabled = 'boolean',
    replacing_resets_timeout = 'boolean',
    closing_enabled = 'boolean',
    history_length = 'number',
    focus_follows = 'Enum',
    print_to_file = 'Path',
    min_window_width = 'number',
    min_window_height = 'number',
    trim_whitespace = 'boolean',
    debug = 'boolean',
    debug_color = 'Color',
    debug_color_alt = 'Color',
    layout_blocks = 'Block[]',
    shortcuts = 'ShortcutsConfig',
}

-- [[ Helpers ]]

local function as_key_value(v, t)
    if     t == 'number' then
        return v
    elseif t == 'boolean' then
        return (b and 'true' or 'false')
    elseif t == 'string' then
        return '"' .. v .. '"'
    elseif t == 'Path' then
        if v == 'None' then return v end
        return '"' .. v .. '"'
    elseif t == 'Enum' then
        return v
    elseif t == 'Color' then
        if v.hex then
            return 'Color(hex: "' .. v.hex .. '")'
        elseif v.r and v.g and v.b then
            if not v.a then v.a = 1.0 end
            return ('Color(r: %d, g: %d, b: %d, a: %d)'):format(v.r, v.g, v.b, v.a)
        end
    elseif t == 'ShortcutsConfig' then
        local s = 'ShortcutsConfig (\n'
        for action, button in pairs(v) do
            s = s .. '\t\t' .. action .. ': ' .. button .. ',\n'
        end
        return s .. '\t)'
    elseif t == 'Block[]' then
        error('Blocks are not supported')
    else
        error('Parsing error for key-value ' .. v .. ' of type ' .. t)
    end
end

local function write_config(config)
    out:write('(\n')
    for k, v in pairs(config) do
        out:write(('\t%s: %s,\n'):format(k, as_key_value(v, get_type[k])))
    end
    out:write(')\n')
    out:close()
end

-- [[ Main ]]

local config = merge_config(user_config, default)
write_config(config)

