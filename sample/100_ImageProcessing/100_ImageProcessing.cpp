// 100_�摜����.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <windows.h>

/***** �摜���� *****/
//�摜�P�F�h��Ԃ�
void fill(void)
{
	//���������ɓh��Ԃ�
	cv::Mat red_img(cv::Size(640,480),CV_8UC3, cv::Scalar(0, 0, 255));
	cv::Mat white_img(cv::Size(640,480),CV_8UC3, cv::Scalar::all(255));
	cv::Mat black_img = cv::Mat::zeros(cv::Size(640,480),CV_8UC3);

	//��������ɓh��Ԃ�
	cv::Mat green_img = red_img.clone();
	green_img = cv::Scalar(0, 255, 0);
	
	cv::namedWindow("red img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("white img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("black img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("green img",CV_WINDOW_AUTOSIZE);
	cv::imshow("red img",red_img);
	cv::imshow("white img",white_img);
	cv::imshow("black img",black_img);
	cv::imshow("green img",green_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//�F��ԕϊ�
void convertColor(void)
{
	cv::Mat bgr_img = cv::imread("lenna.bmp",1);
	if(bgr_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::imshow("lenna img",bgr_img);
	cv::waitKey(0);
	cv::destroyAllWindows();

	cv::Mat dst_img;

	//BGR->HSV
	cv::cvtColor(bgr_img, dst_img, CV_BGR2HSV);
	cv::imshow("CV_BGR2HSV",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
	
	//BGR->Lab
	cv::cvtColor(bgr_img, dst_img, CV_BGR2Lab);
	cv::imshow("CV_BGR2Lab",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
	
	//BGR->YCrCb
	cv::cvtColor(bgr_img, dst_img, CV_BGR2YCrCb);
	cv::imshow("CV_BGR2YCrCb",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//�摜�T�C�Y�ύX(��Ԏ�@���w�肵�ĉ摜�T�C�Y�ύX)
void changeSize(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::imshow("lenna img",src_img);
	cv::waitKey(0);
	cv::destroyAllWindows();

	cv::Mat dst_img1;
	cv::Mat dst_img2(src_img.rows*0.5, src_img.cols*2.0, src_img.type());

	//INTER_LINER(�o�C���j�A���)
	cv::resize(src_img, dst_img1, cv::Size(), 0.5, 0.5);
	cv::resize(src_img, dst_img2, dst_img2.size(), cv::INTER_CUBIC);
	
	cv::namedWindow("dst_img1",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("dst_img2",CV_WINDOW_AUTOSIZE);
	cv::imshow("dst_img1",dst_img1);
	cv::imshow("dst_img2",dst_img2);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//�����E�������]
void convert(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}
	
	cv::Mat v_img;
	cv::Mat h_img;
	cv::Mat b_img;

	cv::flip(src_img, v_img, 0);
	//���������](�������])
	cv::flip(src_img, h_img, 1);	//���������](�������])
	cv::flip(src_img, b_img, -1);	//�����E���������]
	
	cv::namedWindow("v_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("h_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("b_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("v_img",v_img);
	cv::imshow("h_img",h_img);
	cv::imshow("b_img",v_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//�l�K�|�W���]
void cnvNegPos(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat dst_img = ~src_img;
	cv::namedWindow("src_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("dst_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("src_img",src_img);
	cv::imshow("dst_img",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//2�l��
void cnvThresh(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",0);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat bin_img;
	cv::Mat adaptive_img;

	//�Œ�臒l����
	cv::threshold(src_img, bin_img, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
	
	//�K��臒l����
	cv::adaptiveThreshold(src_img, adaptive_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
		cv::THRESH_BINARY, 7, 8);

	cv::namedWindow("src_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("dst_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("bin_img",bin_img);
	cv::imshow("adaptive_img",adaptive_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//�؂蔲���ۑ�
void saveRect(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat roi_img(src_img, cv::Rect(200, 200, 100, 100));
	cv::namedWindow("roi_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("roi_img",roi_img);
	cv::waitKey(0);
	cv::destroyAllWindows();

	cv::imwrite("lena_clipped.png", roi_img);
}


//�ꕔ����
void procROI(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat dst_img = src_img.clone();

	cv::Rect roi_rect(200, 200, 100, 100);
	cv::Mat src_roi = src_img(roi_rect);
	cv::Mat dst_roi = dst_img(roi_rect);

	cv::blur(src_roi, dst_roi, cv::Size(30, 30));

	cv::namedWindow("dst_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("dst_img",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}



//�摜�s���~�b�h
void pyramid(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	//level2�܂ł̉摜�s���~�b�h����
	std::vector<cv::Mat> dst_img;
	cv::buildPyramid(src_img, dst_img, 2);

	cv::namedWindow("level0",CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("level1",CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::namedWindow("level2",CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("level0",dst_img[0]);
	cv::imshow("level1",dst_img[1]);
	cv::imshow("level2",dst_img[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//������
void smoothing(void)
{
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	//�K�E�V�A���t�B���^��p����������
	cv::Mat gaussian1_img;
	cv::Mat gaussian2_img;
	cv::GaussianBlur(src_img, gaussian1_img, cv::Size(11, 11), 10, 10);
	cv::GaussianBlur(src_img, gaussian2_img, cv::Size(51, 3), 80, 3);
	cv::namedWindow("gaussian1_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("gaussian2_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("gaussian1_img",gaussian1_img);
	cv::imshow("gaussian2_img",gaussian2_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
	
	//���f�B�A���t�B���^��p����������
	cv::Mat median1_img;
	cv::Mat median2_img;
	cv::medianBlur(src_img, median1_img, 11);
	cv::medianBlur(src_img, median2_img, 51);
	cv::namedWindow("median1_img",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("median2_img",CV_WINDOW_AUTOSIZE);
	cv::imshow("median1_img",median1_img);
	cv::imshow("median2_img",median2_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//�O�ڋ�`
void CircumscribedRectangle(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//��l���z�����ō��W�𐶐�
	const int rand_num = 50;
	cv::Mat_<int> points(rand_num, 2);
	cv::randu(points, cv::Scalar(100), cv::Scalar(400));
	for(int i; i<rand_num; ++i)
	{
		cv::circle(img, cv::Point(points(i,0), points(i,1)), 2, cv::Scalar(200, 200, 0), -1, CV_AA);
	}

	//�O�ڋ�`
	cv::Rect brect = cv::boundingRect(cv::Mat(points).reshape(2));
	cv::rectangle(img, brect.tl(), brect.br(), cv::Scalar(100, 100, 200), 2, CV_AA);

	//�O�ډ~
	cv::Point2f center;
	float radius;
	cv::minEnclosingCircle(cv::Mat(points).reshape(2), center, radius);
	cv::circle(img, center, radius, cv::Scalar(100, 100, 200), 2, CV_AA);

	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//�ÓT�IHough�ϊ�(�������o)
void cnvHoughLines(void)
{
	cv::Mat src_img = cv::imread("building.jpg",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat dst_img, work_img;
	dst_img = src_img.clone();
	cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
	cv::Canny(work_img, work_img, 50, 200, 3);

	//�ÓT�IHough�ϊ�(�������o)
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(
		work_img,		//���͉摜
		lines,			//�o��
		1,				//��������\
		CV_PI/180,		//�p�x����\
		200,			//臒l
		0,
		0
	);
	
	std::vector<cv::Vec2f>::iterator it = lines.begin();
	for(; it != lines.end(); ++it)
	{
		float rho = (*it)[0], theta = (*it)[1];
		cv::Point pt1, pt2;
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho;
		double y0 = b * rho;
		pt1.x = cv::saturate_cast<int>(x0 + 1000*(-b));
		pt2.x = cv::saturate_cast<int>(y0 + 1000*(a));
		pt1.y = cv::saturate_cast<int>(x0 - 1000*(-b));
		pt2.y = cv::saturate_cast<int>(y0 - 1000*(a));
		cv::line(dst_img, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
	}

	cv::namedWindow("HoughLines",CV_WINDOW_AUTOSIZE);
	cv::imshow("HoughLines",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


//�m���IHough�ϊ�(�������o)
void cnvHoughLinesP(void)
{
	cv::Mat src_img = cv::imread("building.jpg",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	cv::Mat dst_img, work_img;
	dst_img = src_img.clone();
	cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
	cv::Canny(work_img, work_img, 50, 200, 3);

	//�m���IHough�ϊ�(�������o)
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(
		work_img,		//���͉摜
		lines,			//�o��
		1,				//��������\
		CV_PI/180,		//�p�x����\
		50,				//臒l
		50,				//�����̍ŏ�����
		10				//2�_���������ɂ���Ƃ݂Ȃ��ꍇ�ɋ��e�����ő勗��
	);
	
	std::vector<cv::Vec4i>::iterator it = lines.begin();
	for(; it != lines.end(); ++it)
	{
		cv::Vec4i l = *it;
		cv::line(dst_img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 2, CV_AA);
	}

	cv::namedWindow("HoughLinesP",CV_WINDOW_AUTOSIZE);
	cv::imshow("HoughLinesP",dst_img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//fill();			//�摜�P�F�h��Ԃ�
	//convertColor();	//�F��ԕϊ�
	//changeSize();		//�摜�T�C�Y�ύX(��Ԏ�@���w�肵�ĉ摜�T�C�Y�ύX)
	//convert();		//�����E�������]
	//cnvNegPos();		//�l�K�|�W���]
	//cnvThresh();		//2�l��
	//saveRect();			//�؂蔲���ۑ�
	//procROI();			//�ꕔ����
	//pyramid();			//�摜�s���~�b�h
	//smoothing();			//������
	//CircumscribedRectangle();			//�O�ڋ�`
	//cnvHoughLines();			//�ÓT�IHough�ϊ�(�������o)
	cnvHoughLinesP();			//�m���IHough�ϊ�(�������o)

	return 0;
}
