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

#include<opencv2/opencv.hpp>

#include"face_x.h"

class EvaluateMedia
{
private:
	static const std::string kModelFileName;
	static const std::string kSmallModelFileName;
	static const std::string kAlt2;
	static const std::string kTestImage;
	static const FaceX face_x;
	
public:
	EvaluateMedia();

	~EvaluateMedia();

	static int PaintFive(std::vector<cv::Point2d> landmarks , cv::Mat frame);

	int AlignImage();

	int AlignVideo();

	int AlignVideoBasedonLast();
};

#endif