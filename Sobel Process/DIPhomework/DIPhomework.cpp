// DIPhomework.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("C:/Users/Gragon/Desktop/pic/7.bmp", 1);
	imshow("source image", src);

	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y, dst1, dst2, dst3, dst4;

	cvtColor(src, gray0, CV_BGR2GRAY);
	imshow("gray image", gray0);

	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst1);
	imshow("sobel-process result ", dst1);

	addWeighted(dst1, -1, NULL, 0, 255, dst4);

	src.convertTo(dst2, -1, 0.2, 105);
	imshow("linear-process result", dst2);

	addWeighted(dst4, 0.3, dst2, 0.7, 0, dst3);
	//GaussianBlur(dst3, dst3, Size(3, 3), 0); //GaussianBlur
	imshow("final result", dst3);
	
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	imwrite("C:/Users/Gragon/Desktop/pic/lena256-process.bmp", dst3, compression_params);
	waitKey(-1);
	exit(1);
}