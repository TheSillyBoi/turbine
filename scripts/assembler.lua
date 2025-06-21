#!/usr/bin/lua
local lookup = {
	LOAD = 0x0,
	DUMP = 0x1,
	MOVE = 0x2,
	LDD = 0x3,
	PUSH = 0x4,
	POP = 0x5,
	ADD = 0x6,
	ADC = 0x7,
	SUB = 0x8,
	SBB = 0x9,
	NOT = 0xA,
	OR = 0xB,
	AND = 0xC,
	CMP = 0xD,
	JUMP = 0xE,
	HLT = 0xF,

	RA_BYTE = 0x0,
	RA_TWO_BYTES = 0x1,
	RB_BYTE = 0x2,
	RB_TWO_BYTES = 0x3,
	RC_BYTE = 0x4,
	RC_TWO_BYTES = 0x5,
	RD_BYTE = 0x6,
	RD_TWO_BYTES = 0x7,
	RE_BYTE = 0x8,
	RE_TWO_BYTES = 0x9,
	STACK_PTR = 0xa,
	BASE_PTR = 0xb,
	ACCUMULATOR_BYTE = 0xc,
	ACCUMULATOR_TWO_BYTES = 0xd,
	STATUS = 0xd,

	CMP_EQUAL_TO = 0x0,
	CMP_GREATER_THAN = 0x1,
	CMP_LESS_THAN = 0x2,
	ADD_CARRY = 0x3,
	ADD_NO_CARRY = 0x4,
	SUB_BORROW = 0x5,
	SUB_NO_BORROW = 0x6,
}

if arg[1] == nil or arg[2] == nil then
	print("Usage: assembler [input file] [output file]")
	return 1
end

local file = io.open(arg[1], "r")

if file == nil then
	print("File doesn't exist.")
	return 1
end

local binary = {}

for line in file:lines() do
	for token in string.gmatch(line, "[^%s]+") do
		if lookup[token] == nil then
			table.insert(binary, tonumber(token))
			print(string.format("%s: 0x%x", token, tonumber(token)))
		else
			table.insert(binary, tonumber(lookup[token]))
			print(string.format("%s: 0x%x", token, tonumber(lookup[token])))
		end
	end
end

local function write_format(...)
	local res = ""
	local values = { ... }

	for _, value in ipairs(values) do
		value = value % 256

		res = res .. string.char(value)
	end

	return res
end

local compiled = io.open(arg[2], "wb")
if compiled ~= nil then
	compiled.write(compiled, write_format(table.unpack(binary)))
end
print("Assembled!")
