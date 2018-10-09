// 102_ReadWrite.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

//デフォルトパラメータ書き出し
void write_default_parameter(void)
{
	const std::string base = "./lenna";
	std::vector<std::string> files;

	files.push_back(base + ".bmp");
	files.push_back(base + ".dib");
	files.push_back(base + ".jpg");
	files.push_back(base + ".jp2");
	files.push_back(base + ".png");
	files.push_back(base + ".pbm");
	files.push_back(base + ".pgm");
	files.push_back(base + ".ppm");
	files.push_back(base + ".ras");
	files.push_back(base + ".tiff");
	
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	std::vector<std::string>::iterator it = files.begin();
	for(;it!=files.end();++it)
	{
		if(cv::imwrite(*it, src_img))
		{
			std::cout << "imwrite;" << *it <<"...success" << std::endl;
		}
		else
		{
			std::cout << "imwrite;" << *it <<"...failure" << std::endl;
		}
	}
}


//パラメータ書き出し
void write_parameter(void)
{
	const std::string base = "./lenna";
	std::vector<std::string> files;

	files.push_back(base + "_q30.jpg");
	files.push_back(base + "_l9.png");
	files.push_back(base + "_ascii.ppm");
	
	cv::Mat src_img = cv::imread("lenna.bmp",1);
	if(src_img.empty())
	{
		puts("file not found");
		return;
	}

	std::vector<int> params(2);

	//jpg
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 10;	//画像品質(0-100)
	cv::imwrite(base + "_q10.jpg", src_img, params);
	
	//png
	params[0] = CV_IMWRITE_PNG_COMPRESSION;
	params[1] = 9;	//圧縮率(0-9)
	cv::imwrite(base + "_l9.png", src_img, params);
	
	//ppm
	params[0] = CV_IMWRITE_PXM_BINARY;
	params[1] = 0;
	cv::imwrite(base + "_ascii.ppm", src_img, params);
}


//カメラ画像キャプチャ
void capture(void)
{
	cv::VideoCapture cap(0);

	//画像サイズ設定
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	
	//カメラオープンチェック
	if( !cap.isOpened() )
	{
		std::cerr << "cannot find camera or file" << std::endl;
		return;
	}
	
	cv::namedWindow("capture",CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	
	while(1)
	{
		cv::Mat frame;
		cap >> frame;
		cv::imshow("capture", frame);
		int key = cv::waitKey(30);
		if(key == 3 || key == 27 || key == 'q')
		{
			cv::imwrite("capture.png", frame);
			break;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	//write_default_parameter();			//デフォルトパラメータ書き出し
	//write_parameter();			//パラメータ書き出し
	capture();			//カメラ画像キャプチャ

	return 0;
}

