// 101_Drawing.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <windows.h>

/***** 描画処理 *****/
//線分
void drawLine(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, 太さ3, 4近傍連結
	cv::line(img, cv::Point(100, 100), cv::Point(400, 105), cv::Scalar(0, 0, 200), 3, 4);

	//Green, 太さ5 8近傍連結
	cv::line(img, cv::Point(100, 200), cv::Point(400, 205), cv::Scalar(0, 200, 0), 5, 8);

	//Blue, 太さ10, アンチエイリアス
	cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 10, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//矩形
void drawRectangle(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, 太さ3, 4近傍連結
	cv::rectangle(img, cv::Point(200, 50), cv::Point(300, 150), cv::Scalar(0, 0, 200), 3, 4);

	//Green, 太さ5 8近傍連結
	cv::rectangle(img, cv::Point(200, 200), cv::Point(300, 300), cv::Scalar(0, 200, 0), 5, 8);

	//Blue, 塗りつぶし, アンチエイリアス
	cv::rectangle(img, cv::Point(200, 350), cv::Point(300, 450), cv::Scalar(200, 0, 0), -1, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//円
void drawCircle(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//Red, 太さ3, 4近傍連結
	cv::circle(img, cv::Point(300, 100), 100, cv::Scalar(0, 0, 200), 3, 4);

	//Green, 太さ5 8近傍連結
	cv::circle(img, cv::Point(200, 250), 120, cv::Scalar(0, 200, 0), 5, 8);

	//Blue, 塗りつぶし, アンチエイリアス
	cv::circle(img, cv::Point(300, 400), 80, cv::Scalar(200, 0, 0), -1, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

//テキスト1
void drawText(void)
{
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//画像、テキスト、位置、フォント、スケール、色、線太さ、種類
	cv::putText(img, "OpenCV", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 200), 2, CV_AA);
	
	cv::namedWindow("img",CV_WINDOW_AUTOSIZE);
	cv::imshow("img",img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}


int _tmain(int argc, _TCHAR* argv[])
{
	drawLine();		//線分
	//drawRectangle();	//矩形
	//drawCircle();		//円
	//drawText();		//テキスト

	return 0;
}
