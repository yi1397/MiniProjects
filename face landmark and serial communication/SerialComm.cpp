#include "SerialComm.h"


SerialComm::SerialComm() {}
SerialComm::~SerialComm() {}


int SerialComm::connect(const char* portNum)
{
	if (!serial.OpenPort(portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout값 설정
	return RETURN_SUCCESS;
}


int SerialComm::sendCommand(char pos) //데이터를 전송하는 함수
{
	if (serial.WriteByte(pos)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

void SerialComm::sendCommand(const char* pos)
{
	for (int i = 0; i < sizeof(pos); i++)
	{
		if (!SerialComm::sendCommand(pos[i]))
		{
			throw "send command failed";
		}
	}
}

void SerialComm::disconnect() //포트를 다 쓰고 난뒤 닫는 함수
{
	serial.ClosePort();
}