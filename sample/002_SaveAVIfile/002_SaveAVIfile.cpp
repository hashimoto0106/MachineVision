// 002_SaveAVIfile.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
    CvCapture	*capture;
	capture = cvCreateFileCapture();
	if(!capture){
		printf( "����t�@�C����������܂���\n" );
		return -1;
	}

	return 0;
}

