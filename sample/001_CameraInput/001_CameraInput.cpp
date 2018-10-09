// test.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#if 0
//int _tmain(int argc, _TCHAR* argv[])
{
	cv::Mat redImg(cv::Size(320, 240),CV_8UC3, cv::Scalar(0, 0, 255));
	cv:namedWindow("red",cv::WINDOW_AUTOSIZE);
	cv::imshow("red",redImg);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
#endif

//---------------------------------------------------------------------
//�T�v		: �摜���̕��̗̂֊s�𒊏o����
//File Name	: findcontours.cpp
//Library	: OpenCV 1.1
//---------------------------------------------------------------------

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/legacy/legacy.hpp>

#define		SEGMENT				150		//	cvSnakeImage�ŗp���鐧��_�̐�
#define		WINDOW_WIDTH		17		//	cvSnakeImage�ōŏ��l��T������ߖT�̈�̕�
#define		WINDOW_HEIGHT		17		//	cvSnakeImage�ōŏ��l��T������ߖT�̈�̍���
#define		HISTIMAGE_WIDTH		320		//	�q�X�g�O�����摜�̕�
#define		HISTIMAGE_HEIGHT	200		//	�q�X�g�O�����摜�̍���
#define		H_DIMENSION		16		//	�q�X�g�O�����̎�����
#define		H_RANGE_MIN		0
#define		H_RANGE_MAX		180
#define		V_MIN	10		//	���x�̍ŏ��l
#define		V_MAX	256		//	���x�̍ő�l
#define		S_MIN	30		//	�ʓx�̍ŏ��l
#define		S_MAX	256		//	�ʓx�̍ŏ��l
#define		HIDDEN_BACKPROJECTION	0	//	�o�b�N�v���W�F�N�V�����摜��\�������Ȃ��t���O�l
#define		SHOW_BACKPROJECTION		1	//	�o�b�N�v���W�F�N�V�����摜��\��������t���O�l
#define		SELECT_OFF				0	//	�����ǐ՗̈悪�ݒ肳��Ă��Ȃ��Ƃ��̃t���O�l
#define		SELECT_ON				1	//	�����ǐ՗̈悪�ݒ肳��Ă���Ƃ��̃t���O�l
#define		TRACKING_STOP			0	//	�g���b�L���O���~�߂�t���O�l
#define		TRACKING_START			-1	//	�g���b�L���O���J�n����t���O�l
#define		TRACKING_NOW			1	//	�g���b�L���O���������t���O�l
#define		HIDDEN_HISTOGRAM		0	//	�q�X�g�O������\�������Ȃ��t���O�l
#define		SHOW_HISTOGRAM			1	//	�q�X�g�O������\��������t���O�l
#define		ITERATION_SNAKE			10	//	cvSnakeImage�̔�����

IplImage	*resultImage = NULL;			//	�������ʕ\���pIplImage
IplImage	*hsvImage = NULL;			//	HSV�\�F�n�pIplImage
IplImage	*hueImage = NULL;			//	HSV�\�F�n��H�`�����l���pIplImage
IplImage	*maskImage = NULL;			//	�}�X�N�摜�pIplImage
IplImage	*backprojectImage = NULL;	//	�o�b�N�v���W�F�N�V�����摜�pIplImage
IplImage	*histImage = NULL;			//	�q�X�g�O�����`��pIplImage
IplImage	*grayImage = NULL;			//	�O���[�X�P�[���摜�pIplImage

CvHistogram	*hist = NULL;				//	�q�X�g�O���������p�\����

//	�������[�h�I��p�t���O
int	backprojectMode = HIDDEN_BACKPROJECTION;
int	selectObject = SELECT_OFF;
int	trackObject = TRACKING_STOP;
int showHist = SHOW_HISTOGRAM;

//	CamShift�g���b�L���O�p�ϐ�
CvPoint			origin;
CvRect			selection;
CvRect			trackWindow;
CvBox2D			trackRegion;
CvConnectedComp	trackComp;

//	�q�X�g�O�����p�ϐ�
int		hdims = H_DIMENSION;		//	�q�X�g�O�����̎�����
float	hRangesArray[] = {H_RANGE_MIN, H_RANGE_MAX};	//�q�X�g�O�����̃����W
float	*hRanges = hRangesArray;
int		vmin = V_MIN;
int		vmax = V_MAX;

//
//	�}�E�X�h���b�O�ɂ���ď����ǐ՗̈���w�肷��
//
//	����:
//		event	: �}�E�X���{�^���̏��
//		x		: �}�E�X�����݃|�C���g���Ă���x���W
//		y		: �}�E�X�����݃|�C���g���Ă���y���W
//		flags	: �{�v���O�����ł͖��g�p
//		param	: �{�v���O�����ł͖��g�p
//
void on_mouse( int event, int x, int y, int flags, void* param ){
	//	�摜���擾����Ă��Ȃ���΁A�������s��Ȃ�
	if( resultImage == NULL ){
        return;
	}
	//	���_�̈ʒu�ɉ�����y�̒l�𔽓]�i�摜�̔��]�ł͂Ȃ��j
	if( resultImage->origin == 1 ){
        y = resultImage->height - y;
	}
	//	�}�E�X�̍��{�^����������Ă���Έȉ��̏������s��
    if( selectObject == SELECT_ON ){
        selection.x = MIN( x, origin.x );
        selection.y = MIN( y, origin.y );
        selection.width = selection.x + CV_IABS( x - origin.x );
        selection.height = selection.y + CV_IABS( y - origin.y );
        
        selection.x = MAX( selection.x, 0 );
        selection.y = MAX( selection.y, 0 );
        selection.width = MIN( selection.width, resultImage->width );
        selection.height = MIN( selection.height, resultImage->height );
        selection.width = selection.width - selection.x;
        selection.height = selection.height - selection.y;
    }
	//	�}�E�X�̍��{�^���̏�Ԃɂ���ď����𕪊�
    switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			//	�}�E�X�̍��{�^���������ꂽ�̂ł���΁A
			//	���_����ёI�����ꂽ�̈��ݒ�
			origin = cvPoint( x, y );
			selection = cvRect( x, y, 0, 0 );
			selectObject = SELECT_ON;
			break;
		case CV_EVENT_LBUTTONUP:
			//	�}�E�X�̍��{�^���������ꂽ�Ƃ��Awidth��height���ǂ�������ł���΁A
			//	trackObject�t���O��TRACKING_START�ɂ���
			selectObject = SELECT_OFF;
			if( selection.width > 0 && selection.height > 0 ){
				trackObject = TRACKING_START;
			}
			break;
    }
}

//
//	���͂��ꂽ1�̐F���l��RGB�ɕϊ�����
//
//	����:
//		hue		: HSV�\�F�n�ɂ�����F���lH
//	�߂�l�F
//		CvScalar: RGB�̐F���BGR�̏��Ŋi�[���ꂽ�R���e�i
//
CvScalar hsv2rgb( float hue ){
	IplImage *rgbValue, *hsvValue;
	rgbValue = cvCreateImage( cvSize(1,1), IPL_DEPTH_8U, 3 );
	hsvValue = cvCreateImage( cvSize(1,1), IPL_DEPTH_8U, 3 );

	hsvValue->imageData[0] = hue;	//	�F���lH
	hsvValue->imageData[1] = 255;	//	�ʓx�lS
	hsvValue->imageData[2] = 255;	//	���x�lV
	
	//	HSV�\�F�n��RGB�\�F�n�ɕϊ�����
	cvCvtColor( hsvValue, rgbValue, CV_HSV2BGR );

	return cvScalar(	(unsigned char)rgbValue->imageData[0], 
						(unsigned char)rgbValue->imageData[1], 
						(unsigned char)rgbValue->imageData[2], 
						0 );

	//	���������������
	cvReleaseImage( &rgbValue );
	cvReleaseImage( &hsvValue );
}


//
//	�}�E�X�I�����ꂽ�����ǐ՗̈�ɂ�����HSV��H�l�Ńq�X�g�O�������쐬���A�q�X�g�O�����̕`��܂ł��s��
//
//	����:
//		hist		: main�Ő錾���ꂽ�q�X�g�O�����p�\����
//		hsvImage	: ���͉摜��HSV�\�F�n�ɕϊ����ꂽ���IplImage
//		maskImage	: �}�X�N�摜�pIplImage
//		selection	: �}�E�X�őI�����ꂽ��`�̈�
//
void CalculateHist( CvHistogram	*hist, IplImage *hsvImage, IplImage *maskImage, CvRect selection ){
	int		i;
	int		binW;	//	�q�X�g�O�����̊e�r���́A�摜��ł̕�
	int		val;	//	�q�X�g�O�����̕p�x
	float	maxVal;	//	�q�X�g�O�����̍ő�p�x


	//	hsv�摜�̊e��f���l�͈͓̔��ɓ����Ă��邩�`�F�b�N���A
	//	�}�X�N�摜maskImage���쐬����
	cvInRangeS( hsvImage, 
				cvScalar( H_RANGE_MIN, S_MIN, MIN(V_MIN,V_MAX), 0 ),
				cvScalar( H_RANGE_MAX, S_MAX, MAX(V_MIN,V_MAX), 0 ), 
				maskImage );
	//	hsvImage�̂����A�Ƃ��ɕK�v��H�`�����l����hueImage�Ƃ��ĕ�������
	cvSplit( hsvImage, hueImage, 0, 0, 0 );
	//	trackObject��TRACKING_START��ԂȂ�A�ȉ��̏������s��
	if( trackObject == TRACKING_START ){
		//	�ǐ՗̈�̃q�X�g�O�����v�Z��histImage�ւ̕`��
		maxVal = 0.0;

		cvSetImageROI( hueImage, selection );
        cvSetImageROI( maskImage, selection );
        //	�q�X�g�O�������v�Z���A�ő�l�����߂�
		cvCalcHist( &hueImage, hist, 0, maskImage );
		cvGetMinMaxHistValue( hist, 0, &maxVal, 0, 0 );
        //	�q�X�g�O�����̏c���i�p�x�j��0-255�̃_�C�i�~�b�N�����W�ɐ��K��
		if( maxVal == 0.0 ){
			cvConvertScale( hist->bins, hist->bins, 0.0, 0 );
		} else{
			cvConvertScale( hist->bins, hist->bins, 255.0 / maxVal, 0 );
		}
		//	hue,mask�摜�ɐݒ肳�ꂽROI�����Z�b�g
		cvResetImageROI( hueImage );
        cvResetImageROI( maskImage );

        trackWindow = selection;
        //	trackObject��TRACKING_NOW�ɂ���
		trackObject = TRACKING_NOW;

		//	�q�X�g�O�����摜���[���N���A
        cvSetZero( histImage );
		//	�e�r���̕������߂�
        binW = histImage->width / hdims;
		//	�q�X�g�O������`�悷��
        for( i = 0; i < hdims; i++ ){
			val = cvRound( cvGetReal1D(hist->bins,i) * histImage->height / 255 );
            CvScalar color = hsv2rgb( i * 180.0 / hdims );
            cvRectangle(	histImage, 
							cvPoint( i * binW, histImage->height ), 
							cvPoint( (i+1) * binW, histImage->height - val ),
							color,
							-1, 
							8, 
							0	);
		}
	}
}


int main( void ){
	char		windowNameHistogram[] = "Histogram";			//	�q�X�g�O������\������E�C���h�E��
	char		windowNameObjectTracking[] = "ObjectTracking";	//	���̒ǐՌ��ʂ�\������E�C���h�E��

	IplImage	*frameImage;	//	�L���v�`���摜�pIplImage
    CvCapture	*capture;		//	�L�[���͌��ʂ��i�[����ϐ�
    
    int		i;
	int		j;
	int		key;	//	�L�[���͌��ʂ��i�[����ϐ�
	//	Snake�p�̃p�����[�^
	float alpha = 1.0;		//	�A���G�l���M�[�̏d�݃p�����[�^
	float beta = 0.5;		//	�ȗ��̏d�݃p�����[�^
	float gamma = 1.5;		//	�摜�G�l���M�[�̏d�݃p�����[�^
	CvPoint pt[SEGMENT];	//	����_�̍��W
	CvSize window;			//	�ŏ��l��T������ߖT�T�C�Y
	window.width = WINDOW_WIDTH;	
	window.height = WINDOW_HEIGHT;
	CvTermCriteria crit;
	crit.type = CV_TERMCRIT_ITER;		//	�I�������̐ݒ�
	crit.max_iter = ITERATION_SNAKE;	//	�֐��̍ő唽����

	//	�J����������������
	if ( ( capture = cvCreateCameraCapture( 0 ) ) == NULL ){
		//	�J������������Ȃ������ꍇ
		printf( "�J������������܂���\n" );
		return -1;
	}

    printf( "Hot keys: \n"
        "\tq - quit the program\n"
        "\tc - stop the tracking\n"
        "\tb - switch to/from backprojection view\n"
        "\th - show/hide object histogram\n"
        "To initialize tracking, select the object with mouse\n" );

	//	�E�C���h�E�𐶐�����
	cvNamedWindow( windowNameHistogram, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameObjectTracking, CV_WINDOW_AUTOSIZE );
	
	//	�g���b�N�o�[�𐶐�����
	cvSetMouseCallback( windowNameObjectTracking, on_mouse, 0 );
	cvCreateTrackbar( "Vmin", windowNameObjectTracking, &vmin, 256, 0 );
	cvCreateTrackbar( "Vmax", windowNameObjectTracking, &vmax, 256, 0 );
	cvCreateTrackbar( "Smin", windowNameObjectTracking, &vmin, 256, 0 );

    while( 1 ){
		//	�L���v�`���摜�̎擾�Ɏ��s�����珈���𒆒f
        frameImage = cvQueryFrame( capture );
		if( frameImage == NULL ){
            break;
		}
		//	�L���v�`���ɐ��������珈�����p������
        if( resultImage == NULL ){
            //	�e��摜�̊m��
            resultImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 3 );
			resultImage->origin = frameImage->origin;
            hsvImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 3 );
            hueImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
            maskImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
            backprojectImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
			grayImage = cvCreateImage(cvGetSize(frameImage), IPL_DEPTH_8U, 1);
			//	�q�X�g�O�����\���̂̎g�p��錾
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hRanges, 1 );
			//	�q�X�g�O�����p�̉摜���m�ۂ��A�[���N���A
            histImage = cvCreateImage( cvSize(HISTIMAGE_WIDTH, HISTIMAGE_HEIGHT), IPL_DEPTH_8U, 3 );
            cvSetZero( histImage );
		}
		//	�L���v�`�����ꂽ�摜��resultImage�ɃR�s�[���AHSV�\�F�n�ɕϊ�����hsvImage�Ɋi�[
        cvCopy( frameImage, resultImage, NULL );
        cvCvtColor( resultImage, hsvImage, CV_BGR2HSV );

		//	trackObject�t���O��TRACKING_STOP�ȊO�Ȃ�A�ȉ��̏������s��
        if( trackObject != TRACKING_STOP ){

			//�ǐ՗̈�̃q�X�g�O�����v�Z�ƕ`��
			CalculateHist(	hist, hsvImage, maskImage, selection );

			//	�o�b�N�v���W�F�N�V�������v�Z����
            cvCalcBackProject( &hueImage, backprojectImage, hist );
            //	backProjection�̂����A�}�X�N��1�ł���Ƃ��ꂽ�����̂ݎc��
			cvAnd( backprojectImage, maskImage, backprojectImage, 0 );

			//	CamShift�@�ɂ��̈�ǐՂ����s����
			cvCamShift( backprojectImage, 
						trackWindow, 
						cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), 
						&trackComp, 
						&trackRegion );
			
			trackWindow = trackComp.rect;

			//	SnakeImage�p�̃O���[�X�P�[���摜���쐬����
			cvCvtColor( resultImage, grayImage, CV_BGR2GRAY );

			if( backprojectMode == SHOW_BACKPROJECTION ){
                cvCvtColor( backprojectImage, resultImage, CV_GRAY2BGR );
			}
			if( resultImage->origin == 1 ){
                trackRegion.angle = -trackRegion.angle;
			}

			//	CamShift�ł̗̈�ǐՌ��ʂ�Snake�̏����ʒu�ɐݒ肷��
			for( i=0; i<SEGMENT; i++ ){
				pt[i].x = cvRound(	trackRegion.size.width 
									* cos(i * 6.28 / SEGMENT + trackRegion.angle) 
									/ 2.0 + trackRegion.center.x );
				pt[i].y = cvRound(	trackRegion.size.height 
									* sin(i * 6.28 / SEGMENT + trackRegion.angle) 
									/ 2.0 + trackRegion.center.y );
			}
			//	Snake�ɂ��֊s���o�����s����
			for( i=0; i<ITERATION_SNAKE; i++ ){
				cvSnakeImage(	grayImage, 
								pt, 
								SEGMENT, 
								&alpha, 
								&beta, 
								&gamma, 
								CV_VALUE, 
								window, 
								crit, 
								1);
				//	�e�֊s�_�̊Ԃɐ����Ђ��ė֊s����`�悷��
				for( j=0; j<SEGMENT; j++ ){
					if( j < SEGMENT-1 ){
						cvLine( resultImage, pt[j], pt[j+1], 
						  cvScalar(0,0,255,0), 2, 8, 0 );
					}
					else{ 
						cvLine( resultImage, pt[j], pt[0], 
						  cvScalar(0,0,255,0),  2, 8, 0 );
					}
				}
			}
        }
		//	�}�E�X�őI�𒆂̏����ǐ՗̈�̐F�𔽓]������
        if( selectObject == SELECT_ON && selection.width > 0 && selection.height > 0 ){
            cvSetImageROI( resultImage, selection );
            cvXorS( resultImage, cvScalarAll(255), resultImage, 0 );
            cvResetImageROI( resultImage );
        }
		//	backprojectImage�̍��W���_������̏ꍇ�A�㉺�𔽓]������
		//if( backprojectImage->origin == 0 ){
			cvFlip( backprojectImage, backprojectImage, 1 );
		//}
 
		//	�摜��\������
		cvShowImage( windowNameObjectTracking, resultImage );
        cvShowImage( windowNameHistogram, histImage );
		
		//	�L�[���͂�҂��A�����ꂽ�L�[�ɂ���ď����𕪊򂳂���
        key = cvWaitKey(10);
        if( (char) key == 'q' )
			//	while�������[�v����E�o�i�v���O�������I���j
            break;
        switch( (char) key ){
        case 'b':
			//	�\���摜���o�b�N�v���W�F�N�V�����摜�ɐ؂�ւ���
			if( backprojectMode == HIDDEN_BACKPROJECTION ){
				backprojectMode = SHOW_BACKPROJECTION;
			}
			else{
				backprojectMode = HIDDEN_BACKPROJECTION;
			}
			break;
        case 'c':
			//	�g���b�L���O�𒆎~����
            trackObject = TRACKING_STOP;
            cvSetZero( histImage );
            break;
        case 'h':
			//	�q�X�g�O�����̕\��/��\����؂�ւ���
			if( showHist == HIDDEN_HISTOGRAM ){
				showHist = SHOW_HISTOGRAM;
			}
			else{
				showHist = HIDDEN_HISTOGRAM;
			}
			if( showHist == NULL ){
                cvDestroyWindow( windowNameHistogram );
			}
			else{
                cvNamedWindow( windowNameHistogram, CV_WINDOW_AUTOSIZE );
			}
			break;
        default:
            ;
        }
    }
	//	�L���v�`�����������
	cvReleaseCapture( &capture );
	//	���������������
	cvReleaseImage( &resultImage );
	cvReleaseImage( &hsvImage );
	cvReleaseImage( &hueImage);
	cvReleaseImage( &maskImage );
	cvReleaseImage( &backprojectImage );
	cvReleaseImage( &histImage );
	cvReleaseImage( &grayImage );
	//	�E�C���h�E��j������
	cvDestroyWindow( windowNameObjectTracking );

	return 0;
}
