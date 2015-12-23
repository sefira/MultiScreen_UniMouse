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
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include "cv.h"

using namespace std;
namespace fs = boost::filesystem;

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

int EvaluateMedia::CaptureImage(int deviation_num, int image_count)
{
	if (!gray_image.empty())
	{
		if (!faces.empty())
		{
			if (faces[0].area() > 8000)
			{
				cv::Mat to_cnn = cv::Mat(gray_image, faces[0]);
				//cv::imshow("master_control To CNN", to_cnn);
				cv::Mat resized_to_cnn;
				/*
				TODO£º write image
				*/
				cv::Mat augmented_frame[5];
				cv::Mat temp;
				cv::resize(to_cnn, resized_to_cnn, cv::Size(100, 100));
				cv::imshow("resized gray_frame", resized_to_cnn);
				//cout << gray_frame.cols << " " << gray_frame.rows << endl;
				//left_up_frame
				temp = cv::Mat(resized_to_cnn, cv::Rect(0, 0, 91, 91));
				cv::resize(temp, augmented_frame[0], cv::Size(32, 32));
				//left_down_frame
				temp = cv::Mat(resized_to_cnn, cv::Rect(0, 9, 91, 91));
				cv::resize(temp, augmented_frame[1], cv::Size(32, 32));
				//right_up_frame
				temp = cv::Mat(resized_to_cnn, cv::Rect(9, 0, 91, 91));
				cv::resize(temp, augmented_frame[2], cv::Size(32, 32));
				//right_down_frame
				temp = cv::Mat(resized_to_cnn, cv::Rect(9, 9, 91, 91));
				cv::resize(temp, augmented_frame[3], cv::Size(32, 32));
				//center_frame
				temp = cv::Mat(resized_to_cnn, cv::Rect(5, 5, 91, 91));
				cv::resize(temp, augmented_frame[4], cv::Size(32, 32));
				//cv::waitKey();

				string original_filename;
				string path_name;
				string augmented_filename;

				char deviation_str[16];
				char image_count_str[16];
				char augmented_str[16];
				memset(deviation_str, 0, sizeof(deviation_str));
				memset(image_count_str, 0, sizeof(image_count_str));
				sprintf_s(deviation_str, "%d", deviation_num);
				sprintf_s(image_count_str, "%d", image_count); 

				//write the original image 100 * 100, which is for Windows
				path_name = "facedata\\original\\" + string(deviation_str);
				//cout << path_name << endl;
				fs::path full_path(fs::initial_path());
				full_path = fs::system_complete(fs::path(path_name, fs::native));
				//create directory
				if (!fs::exists(full_path))
				{
					bool bRet = fs::create_directories(full_path);
					if (false == bRet)
					{
						cout << "failed to create directory" << endl;
					}
				}
				original_filename = path_name + "\\" + string(image_count_str) + ".jpg";
				cout << original_filename << endl;
				cv::imwrite(original_filename, resized_to_cnn);

				//write the augmented image 5 * 32 * 32, which is for Torch
				path_name = "facedata\\data\\" + string(deviation_str);
				//cout << path_name << endl;
				full_path = fs::system_complete(fs::path(path_name, fs::native));
				//create directory
				if (!fs::exists(full_path))
				{
					bool bRet = fs::create_directories(full_path);
					if (false == bRet)
					{
						cout << "failed to create directory" << endl;
					}
				}
				for (int i = 0; i < 5; i++)
				{
					sprintf_s(augmented_str, "%d", i);
					augmented_filename = path_name + "\\" + string(image_count_str) + "_" +augmented_str + ".jpg";
					cout << augmented_filename << endl;
					cv::imwrite(augmented_filename, augmented_frame[i]);
				}
				return 0;
			}
		}
	}
	return 1;
}

int EvaluateMedia::TrackingFaceFastMode()
{
	cout << "Camera is " << m_videocapture.isOpened() << endl;
	if (!m_videocapture.isOpened())
	{
		cout << "Camera is not Opened, Please check the Webcam!" << endl;
		return 1;
	}

	int original_activated_num = -1;
	int image_file_count = -1;
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
		//cout << deviation << endl;
		if (original_activated_num != deviation)
		{
			image_file_count = 0;
			original_activated_num = deviation;
			SetConsoleColor(COMMANDCOLOR);
			cout << "####################################################" << endl <<
				"####################################################" << endl;
			cout << "num " << deviation << " is activated" << endl;
			cout << "now you should look at NO." << deviation << " screen" << endl;
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
		if (image_file_count < number_of_class)
		{
			if (CaptureImage((int)deviation, image_file_count) == 0)
			{
				image_file_count++;
			}
		}
		else
		{
			cout << "waiting.............................." << endl;
			Sleep(TIMEINTERVAL/10);
		}
		//cout << deviation << endl;
		cv::imshow("Tracking result", gray_image);
		cv::waitKey(TIMEINTERVAL/100);
	}

	return 0;
}
