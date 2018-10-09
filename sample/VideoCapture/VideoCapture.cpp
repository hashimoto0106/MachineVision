// VideoCapture.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

	//�摜�T�C�Y�ݒ�
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	
	fprintf(fp, "�摜�T�C�Y�� : %f[pixel]\n�摜�T�C�Y���� : %f[pixel]\nFPS : %f[frame/s]\n���t���[���� : %f[frame]\n���掞�� : %f[s]\n",
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
		if(frame.empty())	//����t�@�C���I��
		{
			//�Đ��ʒu��擪�ɖ߂�
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

