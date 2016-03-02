#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;

void MyFilledCircle(cv::Mat img, cv::Point center)
{
	int thickness = -1;
	int lineType = 8;
	int radius = 12;
	circle(img,
		center,
		radius,
		cv::Scalar(0, 0, 0),
		thickness,
		lineType);

	return;
}

int main()
{
	int width = 1280;
	int height = 1024;
	int step = 100;
	int count = 0;
	cv::Mat dot_image = cv::imread("C:\\Users\\sefir\\Desktop\\a.png");
	for (int x = 33; x <= width; x++)
	{
		for (int y = 57; y <= height; y++)
		{
			MyFilledCircle(dot_image, cv::Point(x, y));
			count++;
			y += step;
		}
		x += step;
	}

	cout << "total dot : " << count << endl;
	cv::imshow("dot", dot_image);
	cv::imwrite("C:\\Users\\sefir\\Desktop\\dot.jpg", dot_image);
	cv::waitKey();
	return 0;
}
