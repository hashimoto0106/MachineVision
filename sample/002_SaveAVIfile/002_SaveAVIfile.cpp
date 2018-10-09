// 002_SaveAVIfile.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
    CvCapture	*capture;
	capture = cvCreateFileCapture();
	if(!capture){
		printf( "動画ファイルが見つかりません\n" );
		return -1;
	}

	return 0;
}

