#pragma once
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/full_object_detection.h>
#include <opencv2/opencv.hpp>
#include <vector>

typedef std::array<int, 3> points;

auto drawPolyline(
	cv::Mat& image,
	dlib::full_object_detection& landmarks,
	int start, int end,
	bool isClosed = false
)->void
{
	std::vector<cv::Point> points;
	for (int i = start; i <= end; i++)
	{
		points.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
	}
	cv::polylines(image, points, isClosed, cv::Scalar(0, 255, 255), 2, 16);
}

auto drawPolylines(
	cv::Mat& image,
	dlib::full_object_detection& landmarks
)->void
{
	drawPolyline(image, landmarks, 0, 16);              //盼急
	drawPolyline(image, landmarks, 17, 21);             //哭 传界
	drawPolyline(image, landmarks, 22, 26);             //坷弗 传界
	drawPolyline(image, landmarks, 27, 30);             //内 困率
	drawPolyline(image, landmarks, 30, 35, true);       //内 酒贰率
	drawPolyline(image, landmarks, 36, 41, true);       //哭传
	drawPolyline(image, landmarks, 42, 47, true);       //坷弗传
	drawPolyline(image, landmarks, 48, 59, true);       //涝贱
	drawPolyline(image, landmarks, 60, 67, true);       //涝贱
}

auto getAngle(std::vector<cv::Point>& features)->double
{
	double a, b, c;
	double angle;

	a = sqrt(pow(features[0].x - features[2].x, 2) + pow(features[0].y - features[2].y, 2));
	b = sqrt(pow(features[0].x - features[1].x, 2) + pow(features[0].y - features[1].y, 2));
	c = sqrt(pow(features[1].x - features[2].x, 2) + pow(features[1].y - features[2].y, 2));

	angle = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
	angle = angle * (180 / dlib::pi);

	return angle;
}

auto isEyeClosed(dlib::full_object_detection& landmarks, int threshold)->bool
{
	std::vector<points> points_datas;

	double sum(0);

	points_datas.push_back({ 37,36,41 });
	points_datas.push_back({ 38,39,40 });
	points_datas.push_back({ 43,42,47 });
	points_datas.push_back({ 44,45,46 });

	for (points points_data : points_datas)
	{
		std::vector<cv::Point> features;
		features.push_back(cv::Point(landmarks.part(points_data[0]).x(), landmarks.part(points_data[0]).y()));
		features.push_back(cv::Point(landmarks.part(points_data[1]).x(), landmarks.part(points_data[1]).y()));
		features.push_back(cv::Point(landmarks.part(points_data[2]).x(), landmarks.part(points_data[2]).y()));
		sum += getAngle(features);
	}

	//std::cout << sum << std::endl;

	if (sum < threshold) return true;

	return false;
}
