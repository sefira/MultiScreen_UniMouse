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

#include "dlheadpose.h"

#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class EvaluateMedia
{
public:
	//load_facex then program will run with landmark model
	//load landmark model file will take a while time 
	//then we can use AlignImage(),AlignVideo(),AlignVideoBasedoLast()
	EvaluateMedia();

	~EvaluateMedia();

	//return deviation 
	static double GetDeviation(); 
	double SetDeviation(double value);

	//evaluate a skindetected frame by compute 
	//left side and right side mean and stddev ratio
	//the ratio is deviation
	double Evaluate();
	//just tracking face by using openCV model, update the faces rect
	int TrackingFace();

	//evaluate a frame by CNN 
	//estiamtion the head pose pitch and yaw 
	//use the F1 value combin pitch and yaw
	double EvaluateByCNN();
	//for CNN
	int TrackingFaceFastMode();

private:
	//landmark model file name
	static const std::string kModelFileName;
	static const std::string kSmallModelFileName;
	
	//OpenCV face model file name 
	static const std::string kAlt2;
	static const std::string kTestImage;

	cv::CascadeClassifier m_cascadeclassifier;
	std::vector<cv::Rect> faces;

	cv::Mat frame;
	cv::Mat gray_image;
	cv::VideoCapture m_videocapture;
	CNNHeadPose m_cnnheadpose;

	//deviation may be share in two thread
	static double deviation;
	
};

#endif