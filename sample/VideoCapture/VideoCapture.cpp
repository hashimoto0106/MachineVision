// VideoCapture.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/legacy/legacy.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	char *window_input = "video";
	cv::namedWindow(window_input,CV_WINDOW_AUTOSIZE);
	FILE *fp;
	fp = fopen("VideoCapture.log", "w");
	
	fprintf(fp, "video capture start\n");

	cv::VideoCapture cap;
	cap.open("MOVIE.mp4");
	if( !cap.isOpened() )
	{
		std::cerr << "cannot find camera or file" << std::endl;
		fprintf(fp, "cannot find camera or file\n");
		fclose(fp);
		return -1;
	}

	//画像サイズ設定
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	fprintf(fp, "画像サイズ幅 : %f[pixel]\n画像サイズ高さ : %f[pixel]\nFPS : %f[frame/s]\n総フレーム数 : %f[frame]\n動画時間 : %f[s]\n",
			cap.get(CV_CAP_PROP_FRAME_WIDTH),
			cap.get(CV_CAP_PROP_FRAME_HEIGHT),
			cap.get(CV_CAP_PROP_FPS),
			cap.get(CV_CAP_PROP_FRAME_COUNT),
			cap.get(CV_CAP_PROP_FRAME_COUNT)/cap.get(CV_CAP_PROP_FPS)
	);

	while(1)
	{
		cv::Mat frame;
		cap >> frame;
		
		fprintf(fp, "frame : %f, ms : %f, ratio : %f\n", 
			cap.get(CV_CAP_PROP_POS_FRAMES),
			cap.get(CV_CAP_PROP_POS_MSEC),
			cap.get(CV_CAP_PROP_POS_AVI_RATIO)
		);
		if(frame.empty())	//動画ファイル終了
		{
			//再生位置を先頭に戻す
			cap.set(CV_CAP_PROP_POS_FRAMES, 0);
			cap >> frame;
		}
		cv::imshow(window_input, frame);

		int key = cv::waitKey(30);
		if(key == 3 || key == 27 || key == 'q')
		{
			fprintf(fp, "waitKey : %d\n", key);
			break;
		}
	}
	
	fprintf(fp, "video capture end\n");
	return 0;
}

