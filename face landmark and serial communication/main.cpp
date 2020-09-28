#include <iostream>
#include <opencv2/opencv.hpp>
#include "serialcomm.h"

int main() try {
	SerialComm serialComm;

	cv::VideoCapture cap;

	cv::Mat frame;

	cap.open(0);

	if (!cap.isOpened())
	{
		throw "unable to open camera";
	}

	if (!serialComm.connect("COM25"))
	{
		throw "connect faliled";
	}

	while (1)
	{

		cap.read(frame);

		if (frame.empty())
		{
			throw "blank frame grabbed";
		}

		cv::imshow("Live", frame);

		if (cv::waitKey(1) == 27)
			break;

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