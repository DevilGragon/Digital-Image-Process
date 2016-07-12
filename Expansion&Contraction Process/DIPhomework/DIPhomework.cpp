// DIPhomework.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define PROCESS_GRAY 0
#define PROCESS_COLOR 1

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma);
void make_img_broder(InputArray _src, OutputArray _dst, int top, int bottom, int left, int right);
void make_img_contract(InputArray _src, OutputArray _dst, int top, int bottom, int left, int right);
void filter(InputArray _src, OutputArray _dst);

int main()
{
	Mat src = imread("C:/Users/Gragon/Desktop/pic/7.bmp", 1);
	imshow("原图", src);

	int top = 0, bottom = 0, left = 0, right = 0;
	printf("输入要扩展的各边大小：\n");
	printf("上：");
	scanf("%d", &top);
	printf("\n下：");
	scanf("%d", &bottom);
	printf("\n左：");
	scanf("%d", &left);
	printf("\n右：");
	scanf("%d", &right);

	int width = src.cols;
	int heigh = src.rows;
	Mat gray;
	Mat dst1, dst2, dst3, dst4, dst5;

	cvtColor(src, src, CV_BGR2GRAY);

	filter(src, dst1);
	imshow("原图高斯滤波", dst1);
	make_img_broder(src, dst2, top, bottom, left, right);
	imshow("灰度边界扩展", dst2);
	filter(dst2, dst3);
	imshow("扩展高斯滤波", dst3);
	make_img_contract(dst3, dst4, top, bottom, left, right);
	imshow("灰度边界收缩", dst4);

	waitKey(-1);
	exit(0);
}

void make_img_broder(InputArray _src, OutputArray _dst, int top, int bottom, int left, int right)
{
	CV_Assert(top >= 0 && bottom >= 0 && left >= 0 && right >= 0); //broder expansion should have limitation

	Mat src = _src.getMat();
	int type = src.type();
	_dst.create(src.rows + top + bottom, src.cols + left + right, type);
	Mat dst = _dst.getMat();

	if (top == 0 && left == 0 && bottom == 0 && right == 0)
	{
		if (src.data != dst.data || src.step != dst.step)
			src.copyTo(dst);
		return;
	}

	if (src.channels() == 1)
	{
		uchar left_top, left_bottom, right_top, right_bottom;

		for (int i = 0; i < src.cols; i++)
		{
			for (int j = 0; j < src.rows; j++)
			{
				if ((i == 0) && (j == 0))
				{
					left_top = src.at<uchar>(j, i);
				}
				else if ((i == src.cols - 1) && (j == 0))
				{
					right_top = src.at<uchar>(j, i);
				}
				else if ((i == 0) && (j == src.rows - 1))
				{
					left_bottom = src.at<uchar>(j, i);
				}
				else if ((i == src.cols - 1) && (j == src.rows - 1))
				{
					right_bottom = src.at<uchar>(j, i);
				}
			}
		}
		for (int m = 0; m < dst.cols; m++)
		{
			for (int n = 0; n < dst.rows; n++)
			{
				if (((m >= 0) && (m < left)) && ((n >= 0) && (n < top)))
				{
					dst.at<uchar>(n, m) = left_top;
				}
				else if (((m >= src.cols + left) && (m < dst.cols)) && ((n >= 0) && (n < top)))
				{
					dst.at<uchar>(n, m) = right_top;
				}
				else if (((m >= 0) && (m < left)) && ((n >= src.rows + top) && (n < dst.rows)))
				{
					dst.at<uchar>(n, m) = left_bottom;
				}
				else if (((m >= src.cols + left) && (m < dst.cols)) && ((n >= src.rows + top) && (n < dst.rows)))
				{
					dst.at<uchar>(n, m) = right_bottom;
				}
				else if (((m < src.cols + left) && (m >= left)) && ((n >= 0) && (n < top)))
				{
					dst.at<uchar>(n, m) = src.at<uchar>(0, m - left);
				}
				else if (((m < src.cols + left) && (m >= left)) && ((n >= src.rows + top) && (n < dst.rows)))
				{
					dst.at<uchar>(n, m) = src.at<uchar>(src.rows - 1, m - left);
				}
				else if (((m < left) && (m >= 0)) && ((n >= top) && (n < src.rows + top)))
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n - top, 0);
				}
				else if (((m < dst.cols) && (m >= src.cols + left)) && ((n >= top) && (n < src.rows + top)))
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n - top, src.cols - 1);
				}
				else
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n - top, m - left);
				}
			}
		}
	}
	//else if (src.channels() == 3)
	//{
	//	Vec3b left_top, left_bottom, right_top, right_bottom;

	//	for (int i = 0; i < src.cols; i++)
	//	{
	//		for (int j = 0; j < src.rows; j++)
	//		{
	//			if ((i == 0) && (j == 0))
	//			{
	//				left_top[0] = src.at<Vec3b>(j, i)[0];
	//				left_top[1] = src.at<Vec3b>(j, i)[1];
	//				left_top[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == src.cols - 1) && (j == 0))
	//			{
	//				right_top[0] = src.at<Vec3b>(j, i)[0];
	//				right_top[1] = src.at<Vec3b>(j, i)[1];
	//				right_top[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == 0) && (j == src.rows - 1))
	//			{
	//				left_bottom[0] = src.at<Vec3b>(j, i)[0];
	//				left_bottom[1] = src.at<Vec3b>(j, i)[1];
	//				left_bottom[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == src.cols - 1) && (j == src.rows - 1))
	//			{
	//				right_bottom[0] = src.at<Vec3b>(j, i)[0];
	//				right_bottom[1] = src.at<Vec3b>(j, i)[1];
	//				right_bottom[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//		}
	//	}
	//	for (int m = 0; m < dst.cols; m++)
	//	{
	//		for (int n = 0; n < dst.rows; n++)
	//		{
	//			if (((m >= 0) && (m < left)) && ((n >= 0) && (n < top)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = left_top[0];
	//				dst.at<Vec3b>(n, m)[1] = left_top[1];
	//				dst.at<Vec3b>(n, m)[2] = left_top[2];
	//			}
	//			else if (((m >= src.cols + left) && (m < dst.cols)) && ((n >= 0) && (n < top)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = right_top[0];
	//				dst.at<Vec3b>(n, m)[1] = right_top[1];
	//				dst.at<Vec3b>(n, m)[2] = right_top[2];
	//			}
	//			else if (((m >= 0) && (m < left)) && ((n >= src.rows + top) && (n < dst.rows)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = left_bottom[0];
	//				dst.at<Vec3b>(n, m)[1] = left_bottom[1];
	//				dst.at<Vec3b>(n, m)[2] = left_bottom[2];
	//			}
	//			else if (((m >= src.cols + left) && (m < dst.cols)) && ((n >= src.rows + top) && (n < dst.rows)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = right_bottom[0];
	//				dst.at<Vec3b>(n, m)[1] = right_bottom[1];
	//				dst.at<Vec3b>(n, m)[2] = right_bottom[2];
	//			}
	//			else if (((m < src.cols + left) && (m >= left)) && ((n >= 0) && (n < top)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(0, m - left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(0, m - left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(0, m - left)[2];
	//			}
	//			else if (((m < src.cols + left) && (m >= left)) && ((n >= src.rows + top) && (n < dst.rows)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(src.rows - 1, m - left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(src.rows - 1, m - left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(src.rows - 1, m - left)[2];
	//			}
	//			else if (((m < left) && (m >= 0)) && ((n >= top) && (n < src.rows + top)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n - top, 0)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n - top, 0)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n - top, 0)[2];
	//			}
	//			else if (((m < dst.cols) && (m >= src.cols + left)) && ((n >= top) && (n < src.rows + top)))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n - top, src.cols - 1)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n - top, src.cols - 1)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n - top, src.cols - 1)[2];
	//			}
	//			else
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n - top, m - left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n - top, m - left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n - top, m - left)[2];
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	printf("不支持的图片通道类型");
	//	exit(-1);
	//}
}

void make_img_contract(InputArray _src, OutputArray _dst, int top, int bottom, int left, int right)
{
	CV_Assert(top >= 0 && bottom >= 0 && left >= 0 && right >= 0); //broder expansion should have limitation

	Mat src = _src.getMat();
	int type = src.type();
	_dst.create(src.rows - top - bottom, src.cols - left - right, type);
	Mat dst = _dst.getMat();

	if (top == 0 && left == 0 && bottom == 0 && right == 0)
	{
		if (src.data != dst.data || src.step != dst.step)
			src.copyTo(dst);
		return;
	}

	if (src.channels() == 1)
	{
		uchar left_top, left_bottom, right_top, right_bottom;

		for (int i = 0; i < src.cols; i++)
		{
			for (int j = 0; j < src.rows; j++)
			{
				if ((i == left) && (j == top))
				{
					left_top = src.at<uchar>(j, i);
				}
				else if ((i == src.cols - right - 1) && (j == top))
				{
					right_top = src.at<uchar>(j, i);
				}
				else if ((i == left) && (j == src.rows - bottom - 1))
				{
					left_bottom = src.at<uchar>(j, i);
				}
				else if ((i == src.cols - right - 1) && (j == src.rows - bottom - 1))
				{
					right_bottom = src.at<uchar>(j, i);
				}
			}
		}
		for (int m = 0; m < dst.cols; m++)
		{
			for (int n = 0; n < dst.rows; n++)
			{
				if ((m == 0) && (n == 0))
				{
					dst.at<uchar>(n, m) = left_top;
				}
				else if ((m == dst.cols - 1) && (n == 0))
				{
					dst.at<uchar>(n, m) = right_top;
				}
				else if ((m == 0) && (n == dst.rows - 1))
				{
					dst.at<uchar>(n, m) = left_bottom;
				}
				else if ((m == dst.cols - 1) && (n == dst.rows - 1))
				{
					dst.at<uchar>(n, m) = right_bottom;
				}
				else if ((m == 0) && ((n > 0) && (n < dst.rows - 1))) //contract left into a-line
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n + top, left);
				}
				else if (((m > 0) && (m < dst.cols - 1)) && (n == dst.rows - 1)) //contract bottom into a-line
				{
					dst.at<uchar>(n, m) = src.at<uchar>(src.rows - bottom - 1, m + left);
				}
				else if (((m > 0) && (m < dst.cols - 1)) && (n == 0)) //contract top into a-line
				{
					dst.at<uchar>(n, m) = src.at<uchar>(top, m + left);
				}
				else if ((m == dst.cols - 1) && ((n > 0) && (n < dst.rows - 1))) //contract right into a-line
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n + top, src.cols - right - 1);
				}
				else
				{
					dst.at<uchar>(n, m) = src.at<uchar>(n + top, m + left);
				}
			}
		}
	}
	//else if (src.channels() == 3)
	//{
	//	Vec3b left_top, left_bottom, right_top, right_bottom;

	//	for (int i = 0; i < src.cols; i++)
	//	{
	//		for (int j = 0; j < src.rows; j++)
	//		{
	//			if ((i == left - 1) && (j == top - 1))
	//			{
	//				left_top[0] = src.at<Vec3b>(j, i)[0];
	//				left_top[1] = src.at<Vec3b>(j, i)[1];
	//				left_top[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == src.cols - right) && (j == top - 1))
	//			{
	//				right_top[0] = src.at<Vec3b>(j, i)[0];
	//				right_top[1] = src.at<Vec3b>(j, i)[1];
	//				right_top[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == left - 1) && (j == src.rows - bottom))
	//			{
	//				left_bottom[0] = src.at<Vec3b>(j, i)[0];
	//				left_bottom[1] = src.at<Vec3b>(j, i)[1];
	//				left_bottom[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//			else if ((i == src.cols - right) && (j == src.rows - bottom))
	//			{
	//				right_bottom[0] = src.at<Vec3b>(j, i)[0];
	//				right_bottom[1] = src.at<Vec3b>(j, i)[1];
	//				right_bottom[2] = src.at<Vec3b>(j, i)[2];
	//			}
	//		}
	//	}
	//	for (int m = 0; m < dst.cols; m++)
	//	{
	//		for (int n = 0; n < dst.rows; n++)
	//		{
	//			if ((m == 0) && (n == 0))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = left_top[0];
	//				dst.at<Vec3b>(n, m)[1] = left_top[1];
	//				dst.at<Vec3b>(n, m)[2] = left_top[2];
	//			}
	//			else if ((m == dst.cols - 1) && (n == 0))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = right_top[0];
	//				dst.at<Vec3b>(n, m)[1] = right_top[1];
	//				dst.at<Vec3b>(n, m)[2] = right_top[2];
	//			}
	//			else if ((m == 0) && (n == dst.rows - 1))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = left_bottom[0];
	//				dst.at<Vec3b>(n, m)[1] = left_bottom[1];
	//				dst.at<Vec3b>(n, m)[2] = left_bottom[2];
	//			}
	//			else if ((m == dst.cols - 1) && (n == dst.rows - 1))
	//			{
	//				dst.at<Vec3b>(n, m)[0] = right_bottom[0];
	//				dst.at<Vec3b>(n, m)[1] = right_bottom[1];
	//				dst.at<Vec3b>(n, m)[2] = right_bottom[2];
	//			}
	//			else if ((m == 0) && ((n > 0) && (n < dst.rows - 1))) //contract left into a-line
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n + top, left - 1)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n + top, left - 1)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n + top, left - 1)[2];
	//			}
	//			else if (((m > 0) && (m < dst.cols - 1)) && (n == dst.rows - 1)) //contract bottom into a-line
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(src.rows - bottom, m + left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(src.rows - bottom, m + left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(src.rows - bottom, m + left)[2];
	//			}
	//			else if (((m > 0) && (m < dst.cols - 1)) && (n == 0)) //contract top into a-line
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(top - 1, m + left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(top - 1, m + left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(top - 1, m + left)[2];
	//			}
	//			else if ((m == dst.cols - 1) && ((n > 0) && (n < dst.rows - 1))) //contract right into a-line
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n + top, src.cols - right)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n + top, src.cols - right)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n + top, src.cols - right)[2];
	//			}
	//			else
	//			{
	//				dst.at<Vec3b>(n, m)[0] = src.at<Vec3b>(n + top, m + left)[0];
	//				dst.at<Vec3b>(n, m)[1] = src.at<Vec3b>(n + top, m + left)[1];
	//				dst.at<Vec3b>(n, m)[2] = src.at<Vec3b>(n + top, m + left)[2];
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	printf("不支持的图片通道类型");
	//	exit(-1);
	//}
}

void filter(InputArray _src, OutputArray _dst)
{
	float mask[5][5] = { 
		{1, 4, 7, 4, 1}, 
		{4, 16, 26, 16, 4}, 
		{7, 26, 41, 26, 7}, 
		{4, 16, 26, 16, 4}, 
		{1, 4, 7, 4, 1}
	};
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			mask[i][j] = mask[i][j] / 273;
		}
	}

	Mat src = _src.getMat();
	int type = src.type();
	_dst.create(src.rows, src.cols, type);
	Mat dst = _dst.getMat();

	int width = src.cols;
	int heigh = src.rows;

	if (src.channels() == 1)
	{
		for (int y = 2; y < heigh - 2; y++)
		{
			for (int x = 2; x < width - 2; x++)
			{
				float s = 0;
				for (int n = -2; n <= 2; n++)
				{
					for (int m = -2; m <= 2; m++)
					{
						s = s + src.at<uchar>(x + m, y + n) * mask[2 + m][2 + n];
						if (s > 255)
						{
							s = 255;
						}
						else if (s < 0)
						{
							s = 0;
						}
						else
						{
							s = uchar(s);
						}
					}
				}
				dst.at<uchar>(x, y) = uchar(s);
			}
		}
	}
	//else if (src.channels() == 3)
	//{
	//	for (int y = 2; y < heigh - 2; y++)
	//	{
	//		for (int x = 2; x < width - 2; x++)
	//		{
	//			Vec3b value;
	//			for (int n = -2; n <= 2; n++)
	//			{
	//				for (int m = -2; m <= 2; m++)
	//				{
	//					value[0] = value[0] + src.at<Vec3b>(x + m, y + n)[0] * mask[2 + m][2 + n];
	//					value[1] = value[1] + src.at<Vec3b>(x + m, y + n)[1] * mask[2 + m][2 + n];
	//					value[2] = value[2] + src.at<Vec3b>(x + m, y + n)[2] * mask[2 + m][2 + n];
	//					for (int o = 0; o < 3; o++)
	//					{
	//						if (value[o] > 255)
	//						{
	//							value[o] = 255;
	//						}
	//						else if (value[o] < 0)
	//						{
	//							value[o] = 0;
	//						}
	//						else
	//						{
	//							value[o] = value[o];
	//						}
	//					}
	//				}
	//			}
	//			dst.at<Vec3b>(x, y)[0] = value[0];
	//			dst.at<Vec3b>(x, y)[1] = value[1];
	//			dst.at<Vec3b>(x, y)[2] = value[2];
	//		}
	//	}
	//}
}