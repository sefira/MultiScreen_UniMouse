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

#include <algorithm>
#include <limits>

#include "cv.h"
using namespace std;

const string EvaluateMedia::kModelFileName = "model.xml.gz";
const string EvaluateMedia::kSmallModelFileName = "model_small.xml.gz";
const string EvaluateMedia::kAlt2 = "haarcascade_frontalface_alt2.xml";
const string EvaluateMedia::kTestImage = "lena.png";
double EvaluateMedia::deviation = 1000;

EvaluateMedia::EvaluateMedia(bool load_facex) 
{
	if (load_facex)
	{
		face_x = FaceX(kModelFileName);
		landmarks = vector<cv::Point2d>(face_x.landmarks_count());
	}
	m_videocapture = cv::VideoCapture(0);
	m_cascadeclassifier = cv::CascadeClassifier(kAlt2);
}

EvaluateMedia::~EvaluateMedia()
{

}

double EvaluateMedia::GetDeviation()
{
	return deviation;
}

double EvaluateMedia::SetDeviation(double value)
{
	deviation = value;
	return deviation;
}

void SkinDetect(cv::Mat& face_frame_src, cv::Mat& face_frame_dst)
{
	IplImage* src = &IplImage(face_frame_src);
	IplImage* dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* hsv = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	IplImage* tmpH1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* tmpS1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* tmpH2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* tmpS3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* tmpH3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* tmpS2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* H = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* S = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* V = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage* src_tmp1 = cvCreateImage(cvGetSize(src), 8, 3);

	cvSmooth(src, src_tmp1, CV_GAUSSIAN, 3, 3);
	cvCvtColor(src_tmp1, hsv, CV_BGR2HSV);
	cvCvtPixToPlane(hsv, H, S, V, 0);

	cvInRangeS(H, cvScalar(0.0, 0.0, 0, 0), cvScalar(20.0, 0.0, 0, 0), tmpH1);
	cvInRangeS(S, cvScalar(75.0, 0.0, 0, 0), cvScalar(200.0, 0.0, 0, 0), tmpS1);
	cvAnd(tmpH1, tmpS1, tmpH1, 0);

	cvInRangeS(H, cvScalar(0.0, 0.0, 0, 0), cvScalar(13.0, 0.0, 0, 0), tmpH2);
	cvInRangeS(S, cvScalar(20.0, 0.0, 0, 0), cvScalar(90.0, 0.0, 0, 0), tmpS2);
	cvAnd(tmpH2, tmpS2, tmpH2, 0);

	cvInRangeS(H, cvScalar(170.0, 0.0, 0, 0), cvScalar(180.0, 0.0, 0, 0), tmpH3);
	cvInRangeS(S, cvScalar(15.0, 0.0, 0, 0), cvScalar(90., 0.0, 0, 0), tmpS3);
	cvAnd(tmpH3, tmpS3, tmpH3, 0);

	cvOr(tmpH3, tmpH2, tmpH2, 0);
	cvOr(tmpH1, tmpH2, tmpH1, 0);

	cvCopy(tmpH1, dst);
	face_frame_dst = cv::Mat(dst);

	cvReleaseImage(&hsv);
	cvReleaseImage(&tmpH1);
	cvReleaseImage(&tmpS1);
	cvReleaseImage(&tmpH2);
	cvReleaseImage(&tmpS2);
	cvReleaseImage(&tmpH3);
	cvReleaseImage(&tmpS3);
	cvReleaseImage(&H);
	cvReleaseImage(&S);
	cvReleaseImage(&V);
	cvReleaseImage(&src_tmp1);
}

int EvaluateMedia::Evaluate()
{
	deviation = 1000;
	if (!frame.empty())
	{
		if (!faces.empty())
		{
			if (faces[0].area() < 10000)
			{
				return deviation;
			}
			int face_height = faces[0].height;
			int face_width = faces[0].width;
			cv::Mat mask_leftdown = cv::Mat::zeros(face_height, face_width, CV_8U);
			cv::Mat mask_rightdown = cv::Mat::zeros(face_height, face_width, CV_8U);

			int x = 0;
			int y = 0; //face_height * 2 / 3;
			cv::Mat roi = cv::Mat(mask_leftdown, cv::Rect(x, y, face_width / 4, face_height));
			roi = cv::Scalar(255);

			x = face_width - (face_width / 4);
			roi = cv::Mat(mask_rightdown, cv::Rect(x, y, face_width / 4, face_height));
			roi = cv::Scalar(255);

			//imshow("mask_leftdown", mask_leftdown);
			//imshow("mask_rightdown", mask_rightdown);
			//cv::waitKey();

			cv::Scalar mean_leftdown, mean_rightdown;
			cv::Scalar stddev_leftdown, stddev_rightdown;
			cv::Scalar mean_all, stddev_all;
			cv::Mat face_frame_src = cv::Mat(frame, faces[0]);
			cv::Mat face_frame_dst;
			SkinDetect(face_frame_src, face_frame_dst);
			//cv::imshow("face_frame", face_frame_dst);
			cv::meanStdDev(face_frame_dst, mean_leftdown, stddev_leftdown, mask_leftdown);
			cv::meanStdDev(face_frame_dst, mean_rightdown, stddev_rightdown, mask_rightdown);
			cv::meanStdDev(face_frame_dst, mean_all, stddev_all);

			//cout << "leftdown mean:" <<
			//	mean_leftdown.val[0] <<
			//	//"," << mean_leftdown.val[1] <<
			//	//"," << mean_leftdown.val[2] <<
			//	//"," << mean_leftdown.val[3] << 
			//	endl;
			//cout << "rightdown mean:" << 
			//	mean_rightdown.val[0] <<
			//	//"," << mean_rightdown.val[1] <<
			//	//"," << mean_rightdown.val[2] <<
			//	//"," << mean_rightdown.val[3] <<
			//	endl;
			//cout << "all mean:" <<
			//	mean_all.val[0] <<
			//	//"," << mean_all.val[1] <<
			//	//"," << mean_all.val[2] <<
			//	//"," << mean_all.val[3] <<
			//	endl;
			//cout << "leftdown stddev:" << 
			//	stddev_leftdown.val[0] << 
			//	//"," << stddev_leftdown.val[1] << 
			//	//"," << stddev_leftdown.val[2] <<
			//	//"," << stddev_leftdown.val[3] <<
			//	endl;
			//cout << "rightdown stddev:" <<
			//	stddev_rightdown.val[0] <<
			//	//"," << stddev_rightdown.val[1] <<
			//	//"," << stddev_rightdown.val[2] <<
			//	//"," << stddev_rightdown.val[3] << 
			//	endl;
			//cout << "all stddev:" <<
			//	stddev_all.val[0] <<
			//	//"," << stddev_all.val[1] <<
			//	//"," << stddev_all.val[2] <<
			//	//"," << stddev_all.val[3] <<
			//	endl;
			if (mean_leftdown.val[0] > mean_rightdown.val[0])
			{
				//if (mean_rightdown.val[0] <= 0 || stddev_leftdown.val[0] <= 0)
				//{
				//	return deviation;
				//}
				deviation = (mean_leftdown.val[0] + 1) / (mean_rightdown.val[0] + 1);
				deviation += (stddev_rightdown.val[0] + 1) / (stddev_leftdown.val[0] + 1);
			}
			else
			{
				//if (mean_leftdown.val[0] <= 0 || stddev_rightdown.val[0] <= 0)
				//{
				//	return deviation;
				//}
				deviation = (mean_rightdown.val[0] + 1) / (mean_leftdown.val[0] + 1);
				deviation += (stddev_leftdown.val[0] + 1) / (stddev_rightdown.val[0] + 1);
			}
			return deviation;
		}
		else
		{
			return deviation;
		}
	}
	else
	{
		return deviation;
	}
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

bool littlerface(const cv::Rect & face1, const cv::Rect & face2)
{
	return face1.area() > face2.area();
}

int EvaluateMedia::TrackingFace()
{
	while (1)
	{

		m_videocapture >> frame;
		cv::cvtColor(frame, gray_image, cv::COLOR_BGR2GRAY);
		//cv::imshow("Gray image", gray_image);

		m_cascadeclassifier.detectMultiScale(gray_image, faces);

		if (!faces.empty())
		{
			sort(faces.begin(), faces.end(), littlerface);
			//cout << "in the beginning:" << endl;
			//for (cv::Rect face : faces)
			//{
			//	cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
			//	cout << face.area() << endl;
			//}
		}
		Evaluate();
		//cout << deviation << endl;
		//cv::imshow("Tracking result", frame);
		cv::waitKey(1000);
	}
	
	return 1;
}

int EvaluateMedia::AlignImage()
{
	frame = cv::imread(kTestImage);
	cv::cvtColor(frame, gray_image, CV_BGR2GRAY);
	cv::CascadeClassifier m_cascadeclassifier(kAlt2);
	if (m_cascadeclassifier.empty())
	{
		cout << "can not open model file for opencv face detect";
		return 0;
	}

	m_cascadeclassifier.detectMultiScale(gray_image, faces);
	for (cv::Rect face : faces)
	{
		cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
		landmarks = face_x.Alignment(gray_image, face);
		PaintFive(frame);
	}
	cv::imshow("Alignment result", frame);
	cv::waitKey();

	return 1;
}

int EvaluateMedia::AlignVideo()
{
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
		}
		cv::imshow("Alignment result", frame);
		cv::waitKey();
	}

	return 1;
}

int EvaluateMedia::AlignVideoBasedonLast()
{
	cout << "Press \"r\" to re-initialize the face location." << endl;
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
