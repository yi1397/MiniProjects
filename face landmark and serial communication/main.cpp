#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include "serialcomm.h"


void drawPolyline(cv::Mat& image, dlib::full_object_detection landmarks, int start, int end, bool isClosed = false)
{
	std::vector<cv::Point> points;
	for (int i = start; i <= end; i++)
	{
		points.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
	}
	cv::polylines(image, points, isClosed, cv::Scalar(0, 255, 255), 2, 16);
}

void drawPolylines(cv::Mat& image, dlib::full_object_detection landmarks)
{
	drawPolyline(image, landmarks, 0, 16);              //ÅÎ¼±
	drawPolyline(image, landmarks, 17, 21);             //¿Þ ´«½ç
	drawPolyline(image, landmarks, 22, 26);             //¿À¸¥ ´«½ç
	drawPolyline(image, landmarks, 27, 30);             //ÄÚ À§ÂÊ
	drawPolyline(image, landmarks, 30, 35, true);       //ÄÚ ¾Æ·¡ÂÊ
	drawPolyline(image, landmarks, 36, 41, true);       //¿Þ´«
	drawPolyline(image, landmarks, 42, 47, true);       //¿À¸¥´«
	drawPolyline(image, landmarks, 48, 59, true);       //ÀÔ¼ú
	drawPolyline(image, landmarks, 60, 67, true);       //ÀÔ¼ú
}

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

			//Æ¯Â¡Á¡µéÀ» ±×·ÁÁÜ
			drawPolylines(frame, faceLandmark);
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