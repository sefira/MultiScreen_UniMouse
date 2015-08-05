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

using namespace std;

const string EvaluateMedia::kModelFileName = "model.xml.gz";
const string EvaluateMedia::kSmallModelFileName = "model_small.xml.gz";
const string EvaluateMedia::kAlt2 = "haarcascade_frontalface_alt2.xml";
const string EvaluateMedia::kTestImage = "lena.png";
const FaceX EvaluateMedia::face_x = FaceX(kModelFileName);

EvaluateMedia::EvaluateMedia()
{
	landmarks = vector<cv::Point2d> (face_x.landmarks_count());
}

EvaluateMedia::~EvaluateMedia()
{

}

int EvaluateMedia::PaintFive(cv::Mat frame)
{
	if (landmarks.size() == 51)
	{
		cv::circle(frame, landmarks[0], 1, cv::Scalar(0, 255, 0), 2);
		cv::circle(frame, landmarks[9], 1, cv::Scalar(0, 255, 0), 2);
		cv::circle(frame, landmarks[13], 1, cv::Scalar(0, 255, 0), 2);
		cv::circle(frame, landmarks[31], 1, cv::Scalar(0, 255, 0), 2);
		cv::circle(frame, landmarks[37], 1, cv::Scalar(0, 255, 0), 2);
		return 1;
	}
	if (landmarks.size() == 5)
	{
		for (cv::Point2d landmark : landmarks)
		{
			cv::circle(frame, landmark, 1, cv::Scalar(0, 255, 0), 2);
		}
		return 1;
	}
	return 0;
}

int EvaluateMedia::Evaluate(cv::Mat &frame)
{
	char msg[64], diff_value_str[64];
	double diff_value031, diff_value379, diff_value_ratio;
	const char *showMsg;

	memset(msg, 0, sizeof(char));
	strcat(msg, "num of faces:");
	sprintf(diff_value_str, "%lf", faces.size());
	strcat(msg, diff_value_str);
	showMsg = msg;
	cout << showMsg << endl;
	//cvPutText(&frame, "hahahhaha", cvPoint(10, 10), &font, CV_RGB(255, 0, 0));

	memset(msg, 0, sizeof(char));
	strcat(msg, "0 - 31:");
	diff_value031 = landmarks[0].x - landmarks[31].x;
	sprintf(diff_value_str, "%lf", diff_value031);
	strcat(msg, diff_value_str);
	showMsg = msg;
	cout << showMsg << endl;
	//cvPutText(&(CvMat)frame, showMsg, cvPoint(10,10), &font, CV_RGB(255, 0, 0));

	memset(msg, 0, sizeof(char));
	strcat(msg, "37 - 9:");
	diff_value379 = landmarks[37].x - landmarks[9].x;
	sprintf(diff_value_str, "%lf", diff_value379);
	strcat(msg, diff_value_str);
	showMsg = msg;
	cout << showMsg << endl;
	//cvPutText(&(CvMat)frame, showMsg, cvPoint(20, 10), &font, CV_RGB(255, 0, 0));

	memset(msg, 0, sizeof(char));
	strcat(msg, "(0-31)/(37-9):");
	diff_value_ratio = diff_value031 / diff_value379;
	sprintf(diff_value_str, "%lf", diff_value_ratio);
	strcat(msg, diff_value_str);
	showMsg = msg;
	cout << showMsg << endl;
	//cvPutText(&(CvMat)frame, showMsg, cvPoint(30, 10), &font, CV_RGB(255, 0, 0));

	return 1;
}

int EvaluateMedia::AlignImage()
{
	cv::Mat image = cv::imread(kTestImage);
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	cv::CascadeClassifier m_cascadeclassifier(kAlt2);
	if (m_cascadeclassifier.empty())
	{
		cout << "can not open model file for opencv face detect";
		return 0;
	}

	m_cascadeclassifier.detectMultiScale(gray_image, faces);
	for (cv::Rect face : faces)
	{
		cv::rectangle(image, face, cv::Scalar(0, 0, 255), 2);
		landmarks = face_x.Alignment(gray_image, face);
		PaintFive(image);
		Evaluate(image);
	}
	cv::imshow("Alignment result", image);
	cv::waitKey();

	return 1;
}

int EvaluateMedia::AlignVideo()
{
	cv::Mat frame;
	cv::Mat gray_image;
	cv::VideoCapture m_videocapture(0);

	m_videocapture >> frame;
	cv::CascadeClassifier m_cascadeclassifier(kAlt2);

	for (;;)
	{
		m_videocapture >> frame;
		cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);
		//cv::imshow("Gray image", gray_image);

		m_cascadeclassifier.detectMultiScale(gray_image, faces);

		if (!faces.empty())
		{
			cv::rectangle(frame, faces[0], cv::Scalar(0, 0, 255), 2);
			landmarks = face_x.Alignment(gray_image, faces[0]);
			PaintFive(frame);
			Evaluate(frame);
		}
		cv::imshow("Alignment result", frame);
		cv::waitKey();
	}

	return 1;
}

int EvaluateMedia::AlignVideoBasedonLast()
{
	cout << "Press \"r\" to re-initialize the face location." << endl;
	cv::Mat frame;
	cv::Mat gray_image;
	cv::VideoCapture m_videocapture(0);
	m_videocapture >> frame;
	cv::CascadeClassifier cc(kAlt2);

	for (;;)
	{
		m_videocapture >> frame;
		cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);
		//cv::imshow("Gray image", gray_image);

		vector<cv::Point2d> original_landmarks = landmarks;
		landmarks = face_x.Alignment(gray_image, landmarks);

		for (int i = 0; i < landmarks.size(); ++i)
		{
			landmarks[i].x = (landmarks[i].x + original_landmarks[i].x) / 2;
			landmarks[i].y = (landmarks[i].y + original_landmarks[i].y) / 2;
		}
		PaintFive(frame);
		Evaluate(frame);

		cv::imshow("\"r\" to re-initialize, \"q\" to exit", frame);
		int key = cv::waitKey(10);
		if (key == 'q')
			break;
		else if (key == 'r')
		{
			cc.detectMultiScale(gray_image, faces);
			if (!faces.empty())
			{
				landmarks = face_x.Alignment(gray_image, faces[0]);
				cv::rectangle(frame, faces[0], cv::Scalar(0, 0, 255), 2);
			}
		}
	}
	return 1;
}