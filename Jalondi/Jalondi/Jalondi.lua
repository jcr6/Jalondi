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
-- Version: 24.10.04
-- </License Block>


local Kill = {}
local Jal = { ___true = {}, ___args=JALONDI_ARGS }
local Meta = {}

sprintf = string.format
local sprintf=sprintf
function printf(fmt,...)
	print(fmt:format(...))
end
local printf=printf

function JALONDI_CHOPFILE(f)
	local ret = {}
	ret.Dir,ret.File,ret.Ext=Jalondi.FSplit(f)
	return ret
end

function JALONDI_DIR(f)
	local s=Jalondi.DIR_TRUE_IGNORE(1,f or Jalondi.PWD)
	local r={}
	for i=0,s-1 do
		r[#r+1]=Jalondi.DIR_TRUE_IGNORE(2,i)
	end
	return r
end

function Meta.__index(s,idx)	
	if type(idx)=='number' then
		return s.___args[idx]
	end
	assert(type(idx)=='string','Illegal Jalondi call')
	idx = idx:upper()
	if idx=='ARGS' then
		return #s.___args
	elseif idx=='SCRIPT' then
		return s.___args.script		
	elseif idx=='PWD' or idx=='PLATFORM' then
		return s.___true[idx]()
	elseif s.___true[idx] then
		return s.___true[idx]
	else
		error(sprintf('Jalondi does not have a field named -%s-!',idx))
	end
end

function Meta.__newindex(s,idx)
	idx = idx:upper()
	if idx=='ARGS' or type(idx)=='number' then
		error('Argument fields are read-only!')
	elseif s.___true[idx] then
		print('???') for k,v in pairs(s.___true) do print(k,v) end -- debug
		error('All fields are read-only and so is: '..idx)		
		--return s.___true[idx]
	else
		error(sprintf('Jalondi does not have a field named -%s-!',idx))
	end
end

-- Global!
Jalondi = setmetatable(Jal,Meta)


for k,v in pairs(_G) do
     --             12345678
	-- print(k,k:sub(1,8))
	if k:sub(1,8)=='JALONDI_' then
		--print('Added:',k:sub(9))
		Kill[#Kill+1]=k
		Jal.___true[k:sub(9)]=v
	end
end
for _,victim in ipairs(Kill) do _G[victim]=nil end