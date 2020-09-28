#include "SerialComm.h"


SerialComm::SerialComm() {}
SerialComm::~SerialComm() {}


int SerialComm::connect(const char* portNum)
{
	if (!serial.OpenPort(portNum)) //��Ʈ�� �����ϰ� ���¿� �����Ͽ����� fail�� ��ȯ�Ѵ�.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); //��Ʈ �⺻���� �����Ѵ�.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout�� ����
	return RETURN_SUCCESS;
}


int SerialComm::sendCommand(char pos) //�����͸� �����ϴ� �Լ�
{
	if (serial.WriteByte(pos)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

int SerialComm::sendCommand(const char* pos)
{
	for (int i = 0; i < sizeof(pos); i++)
	{
		if (!SerialComm::sendCommand(pos[i]))
		{
			return RETURN_FAIL;
		}
	}
	return RETURN_SUCCESS;
}

void SerialComm::disconnect() //��Ʈ�� �� ���� ���� �ݴ� �Լ�
{
	serial.ClosePort();
}