#include "stdio.h"
#include<opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int main(){

	Mat img = imread("F:\\lwp.jpg");
	imshow("原图显示", img);

	//保存
	Mat img1, img2;
	const char* path;
	path = "F:\\temp\\bak.bmp";
	imwrite(path,img);

	//缩放
	Mat temp = img.clone();
	resize(temp, img1, Size(temp.cols / 2, temp.rows / 2), 0, 0, INTER_LINEAR);
	resize(temp, img2, Size(temp.cols *1.5, temp.rows *1.5), 0, 0, INTER_LINEAR);
	imshow("缩小", img1);
	imshow("放大", img2);

	//边缘检测
	Mat canny,sobel,prewitt,log;
	Canny(temp,canny,50,150);
	imshow("canny",canny);

	Mat grad;
	int x, y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	cvtColor(temp, grad, CV_RGB2GRAY);//灰度图像
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	//x方向梯度计算
	Sobel(grad, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	//y方向梯度计算
	Sobel(grad, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel);//加权和
	imshow("sobel",sobel);

	float prewittx[9] =
	{
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1
	};
	float prewitty[9] =
	{
		1, 1, 1,
		0, 0, 0,
		-1, -1, -1
	};
	Mat px = Mat(3, 3, CV_32F, prewittx);
	Mat py = Mat(3, 3, CV_32F, prewitty);
	Mat dstx = Mat(grad.size(), grad.type(), grad.channels());
	Mat dsty = Mat(grad.size(), grad.type(), grad.channels());
	prewitt = Mat(grad.size(), grad.type(), grad.channels());
	filter2D(grad, dstx, grad.depth(), px);
	filter2D(grad, dsty, grad.depth(), py);
	float tempx, tempy, tem;
	for (int i = 0; i<grad.rows; i++)
	{
		for (int j = 0; j<grad.cols; j++)
		{
			tempx = dstx.at<uchar>(i, j);
			tempy = dsty.at<uchar>(i, j);
			tem = sqrt(tempx*tempx + tempy*tempy);
			prewitt.at<uchar>(i, j) = tem;
		}
	}
	imshow("prewitt", prewitt);

	GaussianBlur(temp, temp, Size(3, 3), 0, 0, BORDER_DEFAULT);//高斯模糊降噪
	Mat log_gray,temp_laplace;
	cvtColor(temp, log_gray, CV_RGB2GRAY);
	Laplacian(log_gray, temp_laplace, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(temp_laplace, log);
	imshow("LoG",log);

	waitKey(0);

	return 0;
}