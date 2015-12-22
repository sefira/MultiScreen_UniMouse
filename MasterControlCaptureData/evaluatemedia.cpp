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

#include "evaluatemedia.h"
#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")

#include "computercomponent.h"
#include "computer.h"

#include <algorithm>
#include <limits>

#include "cv.h"
using namespace std;

double EvaluateMedia::deviation = 0;
EvaluateMedia::EvaluateMedia(bool load_facex) 
{
	m_videocapture = cv::VideoCapture(0);
}

EvaluateMedia::~EvaluateMedia()
{

}

double EvaluateMedia::GetDeviation()
{
	return deviation;
}

bool littlerface(const cv::Rect & face1, const cv::Rect & face2)
{
	return face1.area() > face2.area();
}

int EvaluateMedia::EvaluateByCNN()
{
	if (!gray_image.empty())
	{
		if (!faces.empty())
		{
			if (faces[0].area() > 10000)
			{
				cv::Mat to_cnn = cv::Mat(gray_image, faces[0]);
				cv::imshow("master_control To CNN", to_cnn);
				/*
				TODO£º write image
				*/
				cout << "now save image as " << deviation << " diraction" << endl;
			}
		}
	}
	return 0;
}

int EvaluateMedia::TrackingFaceFastMode()
{
	cout << "Camera is " << m_videocapture.isOpened() << endl;
	if (!m_videocapture.isOpened())
	{
		cout << "Camera is not Opened, Please check the Webcam!" << endl;
		return 1;
	}

	int original_activated_num = 0;
	int * pResults = NULL;
	while (1)
	{
		m_videocapture >> frame;

		cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);
		//cv::imshow("Gray image", gray_image);
		pResults = facedetect_multiview_reinforce((unsigned char*)(gray_image.ptr(0)), gray_image.cols, gray_image.rows, gray_image.step,
			1.2f, 5, 24);

		//print the detection results
		faces.clear();
		for (int i = 0; i < (pResults ? *pResults : 0); i++)
		{
			short * p = ((short*)(pResults + 1)) + 6 * i;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int neighbors = p[4];
			int angle = p[5];

			cv::Rect face_rect = cv::Rect(p[0], p[1], p[2], p[3]);
			faces.push_back(cv::Rect(p[0], p[1], p[2], p[3]));
			//cv::rectangle(frame, face_rect, cv::Scalar(0, 0, 255));
		}
		if (faces.empty())
		{
			//cout << "empty faces vector" << endl;
		}
		if (!faces.empty())
		{
			sort(faces.begin(), faces.end(), littlerface);
			//cout << "in the beginning:" << endl;
			//for (cv::Rect face : faces)
			//{
			//cv::rectangle(gray_image, faces[0], cv::Scalar(0, 0, 255), 2);
			//cout << face.area() << endl;
			//}
		}
		deviation = Computer::GetDeviation();
		if (deviation == 0)
		{
			continue;
		}
		cout << deviation << endl;
		if (original_activated_num != deviation)
		{
			original_activated_num = deviation;
			SetConsoleColor(COMMANDCOLOR);
			cout << "####################################################" << endl <<
				"####################################################" << endl;
			cout << "num " << deviation << " is activated" << endl;
			cout << "now you should look at NO." << deviation << "screen" << endl;
			cout << "####################################################" << endl <<
				"####################################################" << endl;

			for (int i = 0; i < 3; i++)
			{
				cout << "####################################################" << endl <<
					"####################         #######################" << endl;
				cout << "####################    " << i << "    #######################" << endl;
				cout << "####################         #######################" << endl <<
					"####################################################" << endl << endl;
				Sleep(TIMEINTERVAL);
			}
			SetConsoleColor(INITCOLOR);
		}
		EvaluateByCNN();
		//cout << deviation << endl;
		//cv::imshow("Tracking result", gray_image);
		cv::waitKey(TIMEINTERVAL);
	}

	return 0;
}
