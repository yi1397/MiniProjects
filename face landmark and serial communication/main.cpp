#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <list>
#include "serialcomm.h"
#include "feature.h"

#define PORT_NUM "COM25"

#define HEAD_SHAKE_T 2000
#define HEAD_SHAKE_CNT 5

#define EYE_BLINK_T 2000
#define EYE_BLINK_CNT 3
#define EYE_CLOSED 150

int main() try {
	SerialComm serialComm;

	std::list<std::pair<bool, time_t>> eyeBlink;

	std::list<std::pair<bool, time_t>> headShake;

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

			bool eyeState = isEyeClosed(faceLandmark, EYE_CLOSED);

			if (eyeBlink.empty)
			{
				eyeBlink.push_front(std::make_pair(eyeState, clock()));
			}
			else
			{
				if (eyeBlink.front().first != eyeState)
				{
					eyeBlink.push_front(std::make_pair(eyeState, clock()));

					if (EYE_BLINK_CNT < eyeBlink.size())
					{
						serialComm.sendCommand("blinked");
					}
				}

				if (EYE_BLINK_T < clock() - eyeBlink.back().second)
				{
					eyeBlink.pop_back();
				}
			}

			//Æ¯Â¡Á¡µéÀ» ±×·ÁÁÜ
			drawPolylines(frame, faceLandmark);
		}
		else
		{
			eyeBlink.clear();
		}

		if (headShake.empty())
		{
			headShake.push_front(std::make_pair(faces_pos.empty(), clock()));
		}
		else
		{
			if (headShake.front().first != faces_pos.empty())
			{
				headShake.push_front(std::make_pair(faces_pos.empty(), clock()));

				if (HEAD_SHAKE_CNT < headShake.size())
				{
					serialComm.sendCommand("shaked");
				}
			}

			if (HEAD_SHAKE_T < clock() - headShake.back().second)
			{
				headShake.pop_back();
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