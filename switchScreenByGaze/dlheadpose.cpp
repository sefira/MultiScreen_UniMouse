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

#include "dlheadpose.h"
#include "tiny_cnn.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace std;

const std::string CNNHeadPose::weights_filename = "xbu-weights_71";

tiny_cnn::network<tiny_cnn::mse, tiny_cnn::adagrad> nn;

// convert tiny_cnn::image to cv::Mat and resize
cv::Mat Image2Mat(image<>& img) {
	cv::Mat ori(img.height(), img.width(), CV_8U, &img.at(0, 0));
	cv::Mat resized;
	cv::resize(ori, resized, cv::Size(), 3, 3, cv::INTER_AREA);
	return resized;
}

int ResizeImage(cv::Mat img, double minv, double maxv,
	int w,
	int h,
	vec_t& data)
{
	cv::Mat_<uint8_t> resized;
	cv::resize(img, resized, cv::Size(w, h));
	for (int i = 0; i < resized.size().width; i++)
	{
		for (int j = 0; j < resized.size().height; j++)
		{
			double temp = (resized[i][j]) * (maxv - minv) / 255.0 + minv;
			data.push_back(temp);
			//cout << temp << endl;
		}
	}

	return 0;
}
CNNHeadPose::CNNHeadPose()
{
	nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6)
		<< average_pooling_layer<tan_h>(28, 28, 6, 2)
		<< convolutional_layer<tan_h>(14, 14, 5, 6, 16)
		<< average_pooling_layer<tan_h>(10, 10, 16, 2)
		<< convolutional_layer<tan_h>(5, 5, 5, 16, 120)
		//2 output which are pitch and yaw
		<< fully_connected_layer<tiny_cnn::activation::identity>(120, 2);

	// load nets
	ifstream ifs(weights_filename);
	ifs >> nn;
}

CNNHeadPose::~CNNHeadPose()
{

}

int CNNHeadPose::ColouringWeightBias()
{
	for (int i = 0; i < nn.depth(); i++)
	{
		vec_t& weight = nn[i]->weight();
		vec_t& bias = nn[i]->bias();

		for (int j = 0; j < weight.size(); j++)
		{
			weight[j] = i * 2;
		}
		for (int j = 0; j < bias.size(); j++)
		{
			bias[j] = i * 2 + 1;
		}
	}
	ofstream ofs("Colouring-weights");
	ofs << nn;

	return 0;
}

int CNNHeadPose::GetImageDataFromVideo(cv::Mat frame, vec_t& data)
{
	// cannot open, or it's not an image
	if (frame.data == nullptr)
	{
		return 0;
	}

	//resize a image 
	ResizeImage(frame, -1.0, 1.0, 32, 32, data);

	return 0;
}

int CNNHeadPose::GetImageDataFromPicture(const std::string &picutrefilename, vec_t& data)
{
	auto img = cv::imread(picutrefilename, cv::IMREAD_GRAYSCALE);
	// cannot open, or it's not an image
	if (img.data == nullptr)
	{
		return 0;
	}
	ResizeImage(img, -1.0, 1.0, 32, 32, data);
	return 0;
}

double CNNHeadPose::Recognize(cv::Mat gray_frame) 
{
	vec_t data[5];
	vec_t result[5];
	cv::Mat augmented_frame[5];
	cv::resize(gray_frame, gray_frame, cv::Size(100, 100));
	cv::imshow("resized gray_frame",gray_frame);
	//cout << gray_frame.cols << " " << gray_frame.rows << endl;
	//left_up_frame
	augmented_frame[0] = cv::Mat(gray_frame, cv::Rect(0, 0, 91, 91));
	//left_down_frame
	augmented_frame[1] = cv::Mat(gray_frame, cv::Rect(0, 9, 91, 91));
	//right_up_frame
	augmented_frame[2] = cv::Mat(gray_frame, cv::Rect(9, 0, 91, 91));
	//right_down_frame
	augmented_frame[3] = cv::Mat(gray_frame, cv::Rect(9, 9, 91, 91));
	//center_frame
	augmented_frame[4] = cv::Mat(gray_frame, cv::Rect(5, 5, 91, 91));
	cv::imshow("left_up", augmented_frame[0]);
	cv::imshow("left_down", augmented_frame[1]);
	cv::imshow("right_up", augmented_frame[2]);
	cv::imshow("right_down", augmented_frame[3]);
	cv::imshow("center_frame", augmented_frame[4]);
	cv::waitKey();

	//estimation
	double pitch = 0;
	double yaw = 0;
	for (int i = 0; i < 5; i++)
	{
		GetImageDataFromVideo(augmented_frame[i], data[i]);

		// recognize
		result[i] = nn.predict(data[i]);
		pitch += result[i][0];
		yaw += result[i][1];
	}
	pitch = pitch / 5;
	yaw = yaw / 5;
	cout << "pitch:" << pitch << "  yaw:" << yaw << endl;
	pitch = fabs(pitch);
	yaw = fabs(yaw);
	double F1 = 2 * (pitch * yaw) / (pitch + yaw + 1);

	//vec_t test[4];
	//GetImageDataFromPicture("down_down\\image0\\1_1.jpg", test[0]);
	//GetImageDataFromPicture("down_up\\image-30\\1_1.jpg", test[1]);
	//GetImageDataFromPicture("up_down\\image45\\1_1.jpg", test[2]);
	//GetImageDataFromPicture("up_up\\image60\\1_1.jpg", test[3]);

	//vec_t result[4];

	//for (int i = 0; i < 4; i++)
	//{
	//	result[i] = nn.predict(test[i]);
	//	cout << result[i][0] << ' ' << result[i][1] << endl;
	//}

	return pitch;
}
