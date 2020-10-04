#pragma once
#include <string.h>
#include <map>


typedef struct _command_t
{
	unsigned int code;
	unsigned char code_len;

	_command_t(unsigned int _code, unsigned char _code_len) : code(_code), code_len(_code_len) {}

	bool operator<(const _command_t& a) const
	{
		if (a.code <code) return false;
		if (a.code_len < code_len) return false;
		return true;
	}
	bool operator==(const _command_t& a) const
	{
		if (a.code != code) return false;
		if (a.code_len != code_len) return false;
		return true;
	}

} command_t;

typedef std::map<command_t, const char*> commands_t;

void command_init(commands_t& commands)
{
	commands.insert(std::make_pair(command_t(3, 4), "문이 열립니다"));
	commands.insert(std::make_pair(command_t(15, 4), "창문이 열립니다"));
}