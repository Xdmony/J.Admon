
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
	//cΪ������������
	for (c = seq; c != NULL; c = c->h_next)
	{
		// ȡ���������
		double testdbArea = fabs(cvContourArea(c, CV_WHOLE_SEQ));
		//ȡ����������
		double testdbLength = cvArcLength(c);
		c->block_max;

		if (testdbArea >= 50 && testdbLength <= 5000)
		{
			//�㼯�������棨up-right�����α߽�
			CvRect testrect = cvBoundingRect(c);
			//������С������
			CvBox2D testbox = cvMinAreaRect2(c);
			//cvDrawContours(&(CvMat)contours, c, cvScalar(0, 255, 255), cvScalar(0, 255, 0), 0, 2);

			cvRectangle(&(CvMat)contours, cvPoint(testrect.x, testrect.y + testrect.height), cvPoint(testrect.x + testrect.width, testrect.y), cvScalar(255, 0, 255), 1);

			double width = testrect.width;
			double height = testrect.height;
			double juxingmianji = width*height;

			//�����Բ
			//cvMinEnclosingCircle(c, &center, &radius); 
			//cout <<  radius << endl;
			//cout << center.x << "��" << center.y << endl;
			//�����Բ
			//cvCircle(&(CvMat)contours, cvPointFrom32f(center), (int)radius, cvScalar(255, 0, 255), 2);

			//����1���ζ�
			float mianjibi = testdbArea / juxingmianji;

			//����2�ӳ���
			float changkuanbi = width / height;

			//����3�ܳ���
			float zhouchangbi = 2 * (height + width) / testdbLength;

			//����4��Բ��
			float r = 4 * pi*testdbArea / (testdbLength*testdbLength);

			//����5��״������
			float e = (testdbLength*testdbLength) / testdbArea;

			//���������� hu.hu1    hu.hu2

			cout << "���ζȣ�" << mianjibi << endl;
			cout << "�ӳ��ȣ�" << changkuanbi << endl;
			cout << "�ܳ��ȣ�" << zhouchangbi << endl;
			cout << "��Բ�ȣ�" << r << endl;
			cout << "��״�����ԣ�" << e << endl;
			cout << "һ�׾أ�" << hu.hu1 << endl;
			cout << "���׾أ�" << hu.hu2 << endl;
		}
	}

	namedWindow("Canny");
	imshow("Canny", contours);

	waitKey();
	return 0;

}
