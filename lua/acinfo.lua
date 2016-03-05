--[[
    acinfo: X-Plane aircraft info plugin.
    See LICENSE file for copyright and license details.

    acinfo.lua: Sets aircraft type and name in FlyWithLua.
--]]
require("bit")
require("string")


-- Acquire the VSCL custom datarefs.
DataRef("ac_name", "vscl/sim/aircraft/name", "writable", 0)
DataRef("ac_type", "vscl/sim/aircraft/type", "writable")
DataRef("prop_rpm", "sim/cockpit2/engine/indicators/prop_speed_rpm",
        "readonly", 0)
DataRef("manifold_press", "sim/cockpit2/engine/indicators/MPR_in_hg",
        "readonly", 0)


-- Define an array of aircraft types. These values are used as a bitmask.
local aircraft_types = {
    ["unknown"] = 0,
    ["jet"] = 1,
    ["prop"] = 2,
    ["airliner"] = 4,
    ["attack"] = 8,
    ["cargo"] = 16,
    ["bomber"] = 32,
    ["ew"] = 64;
    ["fighter"] = 128,
    ["ga"] = 256,
    ["glider"] = 512,
    ["helicopter"] = 1024,
    ["recon"] = 2048,
    ["seaplane"] = 4096,
    ["spacecraft"] = 8192,
    ["tanker"] = 16384,
    ["uav"] = 32768,
    ["vtol"] = 65536,
    ["xplane"] = 131072
}


-- Define an array of path names mapping to the above types.
local type_from_path = {
    ["Attackers"] = "attack",
    ["Bombers"] = "bomber",
    ["EW"] = "ew",
    ["Fighters"] = "fighter",
    ["General Aviation"] = "ga",
    ["Gliders"] = "glider",
    ["Heavy Metal"] = "airliner",  -- Assume for now that HM is an airliner.
    ["Helicopters"] = "helicopter",
    ["Mega-Planes!"] = "bomber", -- Assume for now that MP! are bombers.
    ["Radio Control"] = "unknown", -- We don't care about RC aircraft.
    ["Science Fiction"] = "fighter", -- AFAIK, this is just the anime fighter.
    ["Seaplanes"] = "ga", -- It's sort of GA.
    ["Space Ships"] = "spacecraft",
    ["Tankers"] = "tanker",
    ["Transports"] = "cargo",
    ["UAS"] = "uav",
    ["VTOL"] = "vtol",
    ["X-Planes"] = "xplane"
}


-- Checks to see if a string is empty.
function isempty(s)
    return s == nil or s == ''
end


-- from http://lua-users.org/wiki/SplitJoin
function string:split(sSeparator, nMax, bRegexp)
    assert(sSeparator ~= '')
    assert(nMax == nil or nMax >= 1)

    local aRecord = {}

    if self:len() > 0 then
        local bPlain = not bRegexp
        nMax = nMax or -1

        local nField, nStart = 1, 1
        local nFirst,nLast = self:find(sSeparator, nStart, bPlain)
        while nFirst and nMax ~= 0 do
            aRecord[nField] = self:sub(nStart, nFirst-1)
            nField = nField+1
            nStart = nLast+1
            nFirst,nLast = self:find(sSeparator, nStart, bPlain)
            nMax = nMax-1
        end
        aRecord[nField] = self:sub(nStart)
    end

    return aRecord
end


-- Determine the best name for the aircraft.
function get_acname()
    if isempty(PLANE_ICAO) then
        local name = string.gsub(AIRCRAFT_FILENAME, ".acf", "")
        -- TODO: Add mapping from filename to a "human-friendly" name.
        ac_name = name
    else
        ac_name = PLANE_ICAO
    end
end


-- Determine the type of the aircraft as best as possible.
function get_actype()
    ac_type = aircraft_types["unknown"]
    --[[
        Determine if the aircraft is jet powered by seing if the manifold
        pressure and propeller RPM are nonzero. Note that we classify
        turboprop engines as jets.
    --]]
    if manifold_press > 0 and prop_rpm > 0 then
        ac_type = bit.bxor(ac_type, aircraft_types["prop"])
    else
        ac_type = bit.bxor(ac_type, aircraft_types["jet"])
    end
    --[[
        Use the file path of the aircraft file to determine the type based on
        common X-Plane numbering schemes.
    --]]
    for _, v in pairs(string.split(AIRCRAFT_PATH, "/")) do
        local tmp = type_from_path[v]
        if tmp ~= nil then
            ac_type = bit.bxor(ac_type, aircraft_types[tmp])
        end
    end
end


-- Update the name and type of aircraft. We should only need to do this on
-- aircraft loads.
function main()
    -- Write the current values to the log file.
    logMsg(string.format("ACInfoLua Info: current name <%s>", ac_name))
    logMsg(string.format("ACInfoLua Info: current type <%s>", ac_type))

    -- Update the datarefs.
    get_acname()
    get_actype()

    -- Log the updated values.
    logMsg(string.format("ACInfoLua Info: name set to <%s>", ac_name))
    logMsg(string.format("ACInfoLua Info: type set to <%s>", ac_type))
end


-- Run the main function.
main()
