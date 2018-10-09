// 101_Drawing.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <windows.h>

/***** �`�揈�� *****/
//����
void drawLine(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, ����3, 4�ߖT�A��
	cv::line(img, cv::Point(100, 100), cv::Point(400, 105), cv::Scalar(0, 0, 200), 3, 4);

	//Green, ����5 8�ߖT�A��
	cv::line(img, cv::Point(100, 200), cv::Point(400, 205), cv::Scalar(0, 200, 0), 5, 8);

	//Blue, ����10, �A���`�G�C���A�X
	cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 10, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//��`
void drawRectangle(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, ����3, 4�ߖT�A��
	cv::rectangle(img, cv::Point(200, 50), cv::Point(300, 150), cv::Scalar(0, 0, 200), 3, 4);

	//Green, ����5 8�ߖT�A��
	cv::rectangle(img, cv::Point(200, 200), cv::Point(300, 300), cv::Scalar(0, 200, 0), 5, 8);

	//Blue, �h��Ԃ�, �A���`�G�C���A�X
	cv::rectangle(img, cv::Point(200, 350), cv::Point(300, 450), cv::Scalar(200, 0, 0), -1, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//�~
void drawCircle(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, ����3, 4�ߖT�A��
	cv::circle(img, cv::Point(300, 100), 100, cv::Scalar(0, 0, 200), 3, 4);

	//Green, ����5 8�ߖT�A��
	cv::circle(img, cv::Point(200, 250), 120, cv::Scalar(0, 200, 0), 5, 8);

	//Blue, �h��Ԃ�, �A���`�G�C���A�X
	cv::circle(img, cv::Point(300, 400), 80, cv::Scalar(200, 0, 0), -1, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//�e�L�X�g1
void drawText(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//�摜�A�e�L�X�g�A�ʒu�A�t�H���g�A�X�P�[���A�F�A�������A���
	cv::putText(img, "OpenCV", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 200), 2, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


int _tmain(int argc, _TCHAR* argv[])
{
	drawLine();		//����
	//drawRectangle();	//��`
	//drawCircle();		//�~
	//drawText();		//�e�L�X�g

	return 0;
}
