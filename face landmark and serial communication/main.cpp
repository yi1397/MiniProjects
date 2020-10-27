#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <list>
#include "serialcomm.h"
#include "feature.h"
//#include "commands.h"

#define DRAW_LANDMARKS false

#define PORT_NUM "COM25"

#define EYE_CLOSED 100

#define COMMAND_TIME 2000

//commands_t commands;

int main() try {

	//command_init(commands);

	std::vector<std::pair<std::pair<int, int>, const char*>> commands;

	commands.push_back(std::make_pair(std::make_pair(225, 8), "1"));
	commands.push_back(std::make_pair(std::make_pair(56, 7), "2"));
	commands.push_back(std::make_pair(std::make_pair(4092, 15), "3"));
	commands.push_back(std::make_pair(std::make_pair(63, 12), "4"));
	commands.push_back(std::make_pair(std::make_pair(230399, 23), "5"));

	SerialComm serialComm;

	bool eyeState;

	bool headState;

	std::list<clock_t> commandTime;

	unsigned int command = 0;

	cv::VideoCapture cap;

	cv::Mat frame;


	dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();
	dlib::shape_predictor landmarkDetector;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> landmarkDetector;


	if (!serialComm.connect(PORT_NUM))
	{
		throw "connect faliled";
	}

	cap.open(0);

	if (!cap.isOpened())
	{
		throw "unable to open camera";
	}

	while (1)
	{

		cap.read(frame);

		if (frame.empty())
		{
			throw "blank frame grabbed";
		}

		dlib::cv_image<dlib::bgr_pixel> dlib_img(frame);

		std::vector<dlib::rectangle> faces_pos;

		faces_pos = faceDetector(dlib_img);

		if (!faces_pos.empty())
		{
			dlib::full_object_detection faceLandmark = landmarkDetector(dlib_img, faces_pos.front());

			if (headState == false)
			{
				if (commandTime.size() != sizeof(unsigned int) * 8)
				{
					commandTime.push_front(clock());
					command |= 1 << (commandTime.size() - 1);
				}

				headState = true;
			}

			else if (eyeState != isEyeClosed(faceLandmark, EYE_CLOSED))
			{
				if (commandTime.size() != sizeof(unsigned int) * 8)
				{
					if (eyeState)
					{
						commandTime.push_front(clock());
					}
				}
				eyeState = !eyeState;
			}
			

#if DRAW_LANDMARKS
			//특징점들을 그려줌
			drawPolylines(frame, faceLandmark);
#endif
		}

		else
		{
			headState = false;
		}

		if (commandTime.empty())
		{
			command = 0;
		}
		else if (COMMAND_TIME < clock() - commandTime.back())
		{
			command = command >> 1;
			commandTime.pop_back();
		}
		/*
		std::cout << "명령어 갯수" << commandTime.size() << std::endl;
		std::cout << "명령어: " << command << std::endl;
		
		for (int i = 0; i < commandTime.size(); i++)
		{
			std::cout << ((command & (1 << i)) ? "고개흔듬 " : "눈 감빡임");
		}
		std::cout<<std::endl;
		*/
		for (const auto& command_data : commands)
		{
			if (command_data.first.first == command && command_data.first.second == commandTime.size())
			{
				std::cout << command_data.second << std::endl;
				command = 0;
				commandTime.clear();
			}
		}
		
		cv::imshow("Live", frame);

		if (cv::waitKey(1) == 27)
		{
			break;
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
