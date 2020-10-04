#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <list>
#include "serialcomm.h"
#include "feature.h"
#include "commands.h"

#define DRAW_LANDMARKS false

#define PORT_NUM "COM25"

#define EYE_CLOSED 100

#define COMMAND_TIME 2000

std::map<command_t, const char*> commands;

int main() try {

	command_init();

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
					command |= 1 << commandTime.size();
				}

				headState = true;
			}

			else if (eyeState != isEyeClosed(faceLandmark, EYE_CLOSED))
			{
				if (commandTime.size() != sizeof(unsigned int) * 8)
				{
					commandTime.push_front(clock());
				}
				eyeState = !eyeState;
			}

			if (COMMAND_TIME < clock() - commandTime.back())
			{
				command = command >> 1;
				commandTime.pop_back();
			}
			

#if DRAW_LANDMARKS
			//Æ¯Â¡Á¡µéÀ» ±×·ÁÁÜ
			drawPolylines(frame, faceLandmark);
#endif
		}

		else
		{
			headState = false;
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