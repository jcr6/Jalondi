-- <License Block>
-- Jalondi
-- Basic initiation Script
-- 
-- 
-- 
-- (c) Jeroen P. Broks, 2024
-- 
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.
-- 
-- Please note that some references to data like pictures or audio, do not automatically
-- fall under this licenses. Mostly this is noted in the respective files.
-- 
-- Version: 24.10.03
-- </License Block>


local Kill = {}
local Jal = { ___true = {}, ___args=JALONDI_ARGS }
local Meta = {}

sprintf = string.format
local sprintf=sprintf

function Meta.__index(s,idx)
	idx=idx:upper()
	if idx=="ARGS" then
		return #s.___args
	elseif type(idx)=="number" then
		return s.___args[idx]
	elseif s.___true[idx] then
		return s.___true[idx]
	else
		error(sprintf("Jalondi does not have a field named \"%s\"!",idx))
	end
end

function Meta.__newindex(s,idx)
	idx = idx:upper()
	if idx=="ARGS" or type(idx)=="number" then
		error("Argument fields are read-only!")
	elseif s.___true[idx] then
		return s.___true[idx]
	else
		error(sprintf("Jalondi does not have a field named \"%s\"!",idx))
	end
end

-- Global!
Jalondi = setmetatable(Jal,Meta)


for k,v in pairs(_G) do
	if k:sub(1,8)=="JALONDI_" then
		Kill[#Kill+1]=k
		Jal.___true[k:sub(8)]=v
	end
end
for _,victim in ipairs(Kill) do _G[victim]=nil end