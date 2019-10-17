
#include<cv.h>  
#include<highgui.h>  
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

CvMemStorage *stroage;
IplImage *contourimage;
CvSeq *seq = NULL;
int ku = -1;
double pi = 3.141592;

int main()
{
	stroage = cvCreateMemStorage();
	Mat src = imread("F:\\lesson.png");
	Mat I;
	medianBlur(src, I, 3);
	//blur(src, I, Size(3, 3), Point(-1, -1));
	//GaussianBlur(src, I, Size(3, 3), 0, 0);
	//bilateralFilter(src, I, 5, 0.0, 10.0);
	contourimage = cvCreateImage(cvGetSize(&(CvMat)I), 8, 1);
	cvtColor(I, I, CV_BGR2GRAY);

	Mat contours;
	Canny(I, contours, 100, 255);
	threshold(contours, contours, 100, 255, CV_THRESH_TRUNC);
	int numcontours = cvFindContours(&(CvMat)contours, stroage, &seq, sizeof(CvContour), CV_RETR_LIST);

	CvMoments moments;
	CvHuMoments hu;
	cvMoments(&(CvMat)I, &moments, 0);
	cvGetHuMoments(&moments, &hu);

	if (ku != numcontours)
	{
		ku = numcontours;
		printf("contournum:::: %d \n", numcontours);
	}

	CvSeq *c = 0;
	int zz = 0;
	int totl = 0;

	cvSet(contourimage, cvScalar(255, 0, 255));
	cvSet(contourimage, cvScalar(125, 0, 125));
	CvPoint2D32f center;
	float radius;
	CvPoint2D32f rectpoint[4];

	CvContour *testcontour = 0;
	//c为轮廓顶点数组
	for (c = seq; c != NULL; c = c->h_next)
	{
		// 取得轮廓面积
		double testdbArea = fabs(cvContourArea(c, CV_WHOLE_SEQ));
		//取得轮廓长度
		double testdbLength = cvArcLength(c);
		c->block_max;

		if (testdbArea >= 50 && testdbLength <= 5000)
		{
			//点集的最外面（up-right）矩形边界
			CvRect testrect = cvBoundingRect(c);
			//轮廓最小外界矩形
			CvBox2D testbox = cvMinAreaRect2(c);
			//cvDrawContours(&(CvMat)contours, c, cvScalar(0, 255, 255), cvScalar(0, 255, 0), 0, 2);

			cvRectangle(&(CvMat)contours, cvPoint(testrect.x, testrect.y + testrect.height), cvPoint(testrect.x + testrect.width, testrect.y), cvScalar(255, 0, 255), 1);

			double width = testrect.width;
			double height = testrect.height;
			double juxingmianji = width*height;

			//找外界圆
			//cvMinEnclosingCircle(c, &center, &radius); 
			//cout <<  radius << endl;
			//cout << center.x << "；" << center.y << endl;
			//画外接圆
			//cvCircle(&(CvMat)contours, cvPointFrom32f(center), (int)radius, cvScalar(255, 0, 255), 2);

			//特征1矩形度
			float mianjibi = testdbArea / juxingmianji;

			//特征2延长度
			float changkuanbi = width / height;

			//特征3周长比
			float zhouchangbi = 2 * (height + width) / testdbLength;

			//特征4似圆度
			float r = 4 * pi*testdbArea / (testdbLength*testdbLength);

			//特征5形状复杂性
			float e = (testdbLength*testdbLength) / testdbArea;

			//不动点特征 hu.hu1    hu.hu2

			cout << "矩形度：" << mianjibi << endl;
			cout << "延长度：" << changkuanbi << endl;
			cout << "周长比：" << zhouchangbi << endl;
			cout << "似圆度：" << r << endl;
			cout << "形状复杂性：" << e << endl;
			cout << "一阶矩：" << hu.hu1 << endl;
			cout << "二阶矩：" << hu.hu2 << endl;
		}
	}

	namedWindow("Canny");
	imshow("Canny", contours);

	waitKey();
	return 0;

}
