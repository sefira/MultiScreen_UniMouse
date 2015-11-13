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

#ifndef SWITCHSCREENBYGAZE_DLHEADPOSE_H_
#define SWITCHSCREENBYGAZE_DLHEADPOSE_H_

#include <iostream>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class CNNHeadPose
{
public:
	CNNHeadPose();

	~CNNHeadPose();

	//for some unknow network we use this funciont 
	//to color the weight and bias in different layer
	int ColouringWeightBias();

	//get a frame 
	int GetImageDataFromVideo(cv::Mat frame, std::vector<double>& data);

	//get a file name 
	int GetImageDataFromPicture(const std::string &picutrefilename, std::vector<double>& data);
	
	//through the network
	double Recognize(cv::Mat gray_frame);

private:
	//weights filename from which we load the weight and bias
	static const std::string weights_filename;

};


#endif