#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include "serialcomm.h"
#include "feature.h"


int main() try {
	SerialComm serialComm;

	cv::VideoCapture cap;

	cv::Mat frame;

	int eyes_blink_cnt = 0;

	bool eyes_close = false;

	dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();
	dlib::shape_predictor landmarkDetector;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> landmarkDetector;


	if (!serialComm.connect("COM25"))
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

			if (isEyeBlinked(faceLandmark))
			{
				const char* str = "´« ±ôºýÀÓ";
				if (!serialComm.sendCommand(str))
				{
					throw "send command failed";
				}
			}

			//Æ¯Â¡Á¡µéÀ» ±×·ÁÁÜ
			drawPolylines(frame, faceLandmark);
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