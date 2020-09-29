#pragma once
#include "serialport.h"

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0


class SerialComm
{
public:
	SerialComm();
	~SerialComm();

	SerialPort	serial;
	int		connect(const char* _portNum);
	int		sendCommand(char pos);
	void	sendCommand(const char* pos);
	void	disconnect();
};