/*
The MIT License(MIT)

Copyright(c) 2015 Bu Xingyuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef SWITCHSCREENBYGAZE_EVALUATEMEDIA_H_
#define SWITCHSCREENBYGAZE_EVALUATEMEDIA_H_

#include<iostream>
#include<string>

//#include<opencv2/opencv.hpp>

#include"face_x.h"

class EvaluateMedia
{
public:
	//load_facex then program will run with landmark model
	//load landmark model file will take a while time 
	//then we can use AlignImage(),AlignVideo(),AlignVideoBasedoLast()
	EvaluateMedia(bool load_facex);

	~EvaluateMedia();

	//return deviation 
	static double GetDeviation(); 
	double SetDeviation(double value);

	//evaluate a skindetected frame by compute 
	//left side and right side mean and stddev ratio
	//the ratio is deviation
	int Evaluate();

	//paint five point from 51 point, which are 
	//corner of eye , nose, corner of mouth
	int PaintFive(cv::Mat frame);

	//just tracking face by using openCV model, update the faces rect
	int TrackingFace();

	//compute the landmarks of lena.png
	int AlignImage();

	//compute the landmarks of a video 
	int AlignVideo();

	//compute the landmarks baseed on last landmarks 
	//without continuous face tracking
	int AlignVideoBasedonLast();

private:
	//landmark model file name
	static const std::string kModelFileName;
	static const std::string kSmallModelFileName;
	
	//OpenCV face model file name 
	static const std::string kAlt2;
	static const std::string kTestImage;
	FaceX face_x;

	cv::CascadeClassifier m_cascadeclassifier;
	std::vector<cv::Point2d> landmarks;
	std::vector<cv::Rect> faces;

	cv::Mat frame;
	cv::Mat gray_image;
	cv::VideoCapture m_videocapture;

	//deviation may be share in two thread
	static double deviation;
	
};

#endif