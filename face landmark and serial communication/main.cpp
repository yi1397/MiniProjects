#include <iostream>
#include "serialcomm.h"

int main() try {
	SerialComm serialComm;

	std::cout << "test" << std::endl;

	if (!serialComm.connect("COM25"))
	{
		throw "connect faliled";
	}

	while (1)
	{
		const char* str = "test";
		if (!serialComm.sendCommand(str))
		{
			throw "send command failed";
		}
	}

	serialComm.disconnect();

	return 0;
}
catch (const std::exception & e)
{
	std::cerr << e.what() << std::endl;
}
catch (const char* e)
{
	std::cerr << e << std::endl;
}
catch (...)
{
	std::cerr << "Unkown exception\n" << std::endl;
}