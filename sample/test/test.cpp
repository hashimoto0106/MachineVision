// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
//概要		: 画像中の物体の輪郭を抽出する
//File Name	: findcontours.cpp
//Library	: OpenCV 1.1
//---------------------------------------------------------------------

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/legacy/legacy.hpp>

#define		SEGMENT				150		//	cvSnakeImageで用いる制御点の数
#define		WINDOW_WIDTH		17		//	cvSnakeImageで最小値を探索する近傍領域の幅
#define		WINDOW_HEIGHT		17		//	cvSnakeImageで最小値を探索する近傍領域の高さ
#define		HISTIMAGE_WIDTH		320		//	ヒストグラム画像の幅
#define		HISTIMAGE_HEIGHT	200		//	ヒストグラム画像の高さ
#define		H_DIMENSION		16		//	ヒストグラムの次元数
#define		H_RANGE_MIN		0
#define		H_RANGE_MAX		180
#define		V_MIN	10		//	明度の最小値
#define		V_MAX	256		//	明度の最大値
#define		S_MIN	30		//	彩度の最小値
#define		S_MAX	256		//	彩度の最小値
#define		HIDDEN_BACKPROJECTION	0	//	バックプロジェクション画像を表示させないフラグ値
#define		SHOW_BACKPROJECTION		1	//	バックプロジェクション画像を表示させるフラグ値
#define		SELECT_OFF				0	//	初期追跡領域が設定されていないときのフラグ値
#define		SELECT_ON				1	//	初期追跡領域が設定されているときのフラグ値
#define		TRACKING_STOP			0	//	トラッキングを止めるフラグ値
#define		TRACKING_START			-1	//	トラッキングを開始するフラグ値
#define		TRACKING_NOW			1	//	トラッキング中を示すフラグ値
#define		HIDDEN_HISTOGRAM		0	//	ヒストグラムを表示させないフラグ値
#define		SHOW_HISTOGRAM			1	//	ヒストグラムを表示させるフラグ値
#define		ITERATION_SNAKE			10	//	cvSnakeImageの反復回数

IplImage	*resultImage = NULL;			//	処理結果表示用IplImage
IplImage	*hsvImage = NULL;			//	HSV表色系用IplImage
IplImage	*hueImage = NULL;			//	HSV表色系のHチャンネル用IplImage
IplImage	*maskImage = NULL;			//	マスク画像用IplImage
IplImage	*backprojectImage = NULL;	//	バックプロジェクション画像用IplImage
IplImage	*histImage = NULL;			//	ヒストグラム描画用IplImage
IplImage	*grayImage = NULL;			//	グレースケール画像用IplImage

CvHistogram	*hist = NULL;				//	ヒストグラム処理用構造体

//	処理モード選択用フラグ
int	backprojectMode = HIDDEN_BACKPROJECTION;
int	selectObject = SELECT_OFF;
int	trackObject = TRACKING_STOP;
int showHist = SHOW_HISTOGRAM;

//	CamShiftトラッキング用変数
CvPoint			origin;
CvRect			selection;
CvRect			trackWindow;
CvBox2D			trackRegion;
CvConnectedComp	trackComp;

//	ヒストグラム用変数
int		hdims = H_DIMENSION;		//	ヒストグラムの次元数
float	hRangesArray[] = {H_RANGE_MIN, H_RANGE_MAX};	//ヒストグラムのレンジ
float	*hRanges = hRangesArray;
int		vmin = V_MIN;
int		vmax = V_MAX;

//
//	マウスドラッグによって初期追跡領域を指定する
//
//	引数:
//		event	: マウス左ボタンの状態
//		x		: マウスが現在ポイントしているx座標
//		y		: マウスが現在ポイントしているy座標
//		flags	: 本プログラムでは未使用
//		param	: 本プログラムでは未使用
//
void on_mouse( int event, int x, int y, int flags, void* param ){
	//	画像が取得されていなければ、処理を行わない
	if( resultImage == NULL ){
        return;
	}
	//	原点の位置に応じてyの値を反転（画像の反転ではない）
	if( resultImage->origin == 1 ){
        y = resultImage->height - y;
	}
	//	マウスの左ボタンが押されていれば以下の処理を行う
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
	//	マウスの左ボタンの状態によって処理を分岐
    switch( event ){
		case CV_EVENT_LBUTTONDOWN:
			//	マウスの左ボタンが押されたのであれば、
			//	原点および選択された領域を設定
			origin = cvPoint( x, y );
			selection = cvRect( x, y, 0, 0 );
			selectObject = SELECT_ON;
			break;
		case CV_EVENT_LBUTTONUP:
			//	マウスの左ボタンが離されたとき、widthとheightがどちらも正であれば、
			//	trackObjectフラグをTRACKING_STARTにする
			selectObject = SELECT_OFF;
			if( selection.width > 0 && selection.height > 0 ){
				trackObject = TRACKING_START;
			}
			break;
    }
}

//
//	入力された1つの色相値をRGBに変換する
//
//	引数:
//		hue		: HSV表色系における色相値H
//	戻り値：
//		CvScalar: RGBの色情報がBGRの順で格納されたコンテナ
//
CvScalar hsv2rgb( float hue ){
	IplImage *rgbValue, *hsvValue;
	rgbValue = cvCreateImage( cvSize(1,1), IPL_DEPTH_8U, 3 );
	hsvValue = cvCreateImage( cvSize(1,1), IPL_DEPTH_8U, 3 );

	hsvValue->imageData[0] = hue;	//	色相値H
	hsvValue->imageData[1] = 255;	//	彩度値S
	hsvValue->imageData[2] = 255;	//	明度値V
	
	//	HSV表色系をRGB表色系に変換する
	cvCvtColor( hsvValue, rgbValue, CV_HSV2BGR );

	return cvScalar(	(unsigned char)rgbValue->imageData[0], 
						(unsigned char)rgbValue->imageData[1], 
						(unsigned char)rgbValue->imageData[2], 
						0 );

	//	メモリを解放する
	cvReleaseImage( &rgbValue );
	cvReleaseImage( &hsvValue );
}


//
//	マウス選択された初期追跡領域におけるHSVのH値でヒストグラムを作成し、ヒストグラムの描画までを行う
//
//	引数:
//		hist		: mainで宣言されたヒストグラム用構造体
//		hsvImage	: 入力画像がHSV表色系に変換された後のIplImage
//		maskImage	: マスク画像用IplImage
//		selection	: マウスで選択された矩形領域
//
void CalculateHist( CvHistogram	*hist, IplImage *hsvImage, IplImage *maskImage, CvRect selection ){
	int		i;
	int		binW;	//	ヒストグラムの各ビンの、画像上での幅
	int		val;	//	ヒストグラムの頻度
	float	maxVal;	//	ヒストグラムの最大頻度


	//	hsv画像の各画素が値の範囲内に入っているかチェックし、
	//	マスク画像maskImageを作成する
	cvInRangeS( hsvImage, 
				cvScalar( H_RANGE_MIN, S_MIN, MIN(V_MIN,V_MAX), 0 ),
				cvScalar( H_RANGE_MAX, S_MAX, MAX(V_MIN,V_MAX), 0 ), 
				maskImage );
	//	hsvImageのうち、とくに必要なHチャンネルをhueImageとして分離する
	cvSplit( hsvImage, hueImage, 0, 0, 0 );
	//	trackObjectがTRACKING_START状態なら、以下の処理を行う
	if( trackObject == TRACKING_START ){
		//	追跡領域のヒストグラム計算とhistImageへの描画
		maxVal = 0.0;

		cvSetImageROI( hueImage, selection );
        cvSetImageROI( maskImage, selection );
        //	ヒストグラムを計算し、最大値を求める
		cvCalcHist( &hueImage, hist, 0, maskImage );
		cvGetMinMaxHistValue( hist, 0, &maxVal, 0, 0 );
        //	ヒストグラムの縦軸（頻度）を0-255のダイナミックレンジに正規化
		if( maxVal == 0.0 ){
			cvConvertScale( hist->bins, hist->bins, 0.0, 0 );
		} else{
			cvConvertScale( hist->bins, hist->bins, 255.0 / maxVal, 0 );
		}
		//	hue,mask画像に設定されたROIをリセット
		cvResetImageROI( hueImage );
        cvResetImageROI( maskImage );

        trackWindow = selection;
        //	trackObjectをTRACKING_NOWにする
		trackObject = TRACKING_NOW;

		//	ヒストグラム画像をゼロクリア
        cvSetZero( histImage );
		//	各ビンの幅を決める
        binW = histImage->width / hdims;
		//	ヒストグラムを描画する
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
	char		windowNameHistogram[] = "Histogram";			//	ヒストグラムを表示するウインドウ名
	char		windowNameObjectTracking[] = "ObjectTracking";	//	物体追跡結果を表示するウインドウ名

	IplImage	*frameImage;	//	キャプチャ画像用IplImage
    CvCapture	*capture;		//	キー入力結果を格納する変数
    
    int		i;
	int		j;
	int		key;	//	キー入力結果を格納する変数
	//	Snake用のパラメータ
	float alpha = 1.0;		//	連続エネルギーの重みパラメータ
	float beta = 0.5;		//	曲率の重みパラメータ
	float gamma = 1.5;		//	画像エネルギーの重みパラメータ
	CvPoint pt[SEGMENT];	//	制御点の座標
	CvSize window;			//	最小値を探索する近傍サイズ
	window.width = WINDOW_WIDTH;	
	window.height = WINDOW_HEIGHT;
	CvTermCriteria crit;
	crit.type = CV_TERMCRIT_ITER;		//	終了条件の設定
	crit.max_iter = ITERATION_SNAKE;	//	関数の最大反復数

	//	カメラを初期化する
	if ( ( capture = cvCreateCameraCapture( 0 ) ) == NULL ){
		//	カメラが見つからなかった場合
		printf( "カメラが見つかりません\n" );
		return -1;
	}

    printf( "Hot keys: \n"
        "\tq - quit the program\n"
        "\tc - stop the tracking\n"
        "\tb - switch to/from backprojection view\n"
        "\th - show/hide object histogram\n"
        "To initialize tracking, select the object with mouse\n" );

	//	ウインドウを生成する
	cvNamedWindow( windowNameHistogram, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameObjectTracking, CV_WINDOW_AUTOSIZE );
	
	//	トラックバーを生成する
	cvSetMouseCallback( windowNameObjectTracking, on_mouse, 0 );
	cvCreateTrackbar( "Vmin", windowNameObjectTracking, &vmin, 256, 0 );
	cvCreateTrackbar( "Vmax", windowNameObjectTracking, &vmax, 256, 0 );
	cvCreateTrackbar( "Smin", windowNameObjectTracking, &vmin, 256, 0 );

    while( 1 ){
		//	キャプチャ画像の取得に失敗したら処理を中断
        frameImage = cvQueryFrame( capture );
		if( frameImage == NULL ){
            break;
		}
		//	キャプチャに成功したら処理を継続する
        if( resultImage == NULL ){
            //	各種画像の確保
            resultImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 3 );
			resultImage->origin = frameImage->origin;
            hsvImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 3 );
            hueImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
            maskImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
            backprojectImage = cvCreateImage( cvGetSize(frameImage), IPL_DEPTH_8U, 1 );
			grayImage = cvCreateImage(cvGetSize(frameImage), IPL_DEPTH_8U, 1);
			//	ヒストグラム構造体の使用を宣言
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hRanges, 1 );
			//	ヒストグラム用の画像を確保し、ゼロクリア
            histImage = cvCreateImage( cvSize(HISTIMAGE_WIDTH, HISTIMAGE_HEIGHT), IPL_DEPTH_8U, 3 );
            cvSetZero( histImage );
		}
		//	キャプチャされた画像をresultImageにコピーし、HSV表色系に変換してhsvImageに格納
        cvCopy( frameImage, resultImage, NULL );
        cvCvtColor( resultImage, hsvImage, CV_BGR2HSV );

		//	trackObjectフラグがTRACKING_STOP以外なら、以下の処理を行う
        if( trackObject != TRACKING_STOP ){

			//追跡領域のヒストグラム計算と描画
			CalculateHist(	hist, hsvImage, maskImage, selection );

			//	バックプロジェクションを計算する
            cvCalcBackProject( &hueImage, backprojectImage, hist );
            //	backProjectionのうち、マスクが1であるとされた部分のみ残す
			cvAnd( backprojectImage, maskImage, backprojectImage, 0 );

			//	CamShift法による領域追跡を実行する
			cvCamShift( backprojectImage, 
						trackWindow, 
						cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), 
						&trackComp, 
						&trackRegion );
			
			trackWindow = trackComp.rect;

			//	SnakeImage用のグレースケール画像を作成する
			cvCvtColor( resultImage, grayImage, CV_BGR2GRAY );

			if( backprojectMode == SHOW_BACKPROJECTION ){
                cvCvtColor( backprojectImage, resultImage, CV_GRAY2BGR );
			}
			if( resultImage->origin == 1 ){
                trackRegion.angle = -trackRegion.angle;
			}

			//	CamShiftでの領域追跡結果をSnakeの初期位置に設定する
			for( i=0; i<SEGMENT; i++ ){
				pt[i].x = cvRound(	trackRegion.size.width 
									* cos(i * 6.28 / SEGMENT + trackRegion.angle) 
									/ 2.0 + trackRegion.center.x );
				pt[i].y = cvRound(	trackRegion.size.height 
									* sin(i * 6.28 / SEGMENT + trackRegion.angle) 
									/ 2.0 + trackRegion.center.y );
			}
			//	Snakeによる輪郭抽出を実行する
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
				//	各輪郭点の間に線をひいて輪郭線を描画する
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
		//	マウスで選択中の初期追跡領域の色を反転させる
        if( selectObject == SELECT_ON && selection.width > 0 && selection.height > 0 ){
            cvSetImageROI( resultImage, selection );
            cvXorS( resultImage, cvScalarAll(255), resultImage, 0 );
            cvResetImageROI( resultImage );
        }
		//	backprojectImageの座標原点が左上の場合、上下を反転させる
		//if( backprojectImage->origin == 0 ){
			cvFlip( backprojectImage, backprojectImage, 1 );
		//}
 
		//	画像を表示する
		cvShowImage( windowNameObjectTracking, resultImage );
        cvShowImage( windowNameHistogram, histImage );
		
		//	キー入力を待ち、押されたキーによって処理を分岐させる
        key = cvWaitKey(10);
        if( (char) key == 'q' )
			//	while無限ループから脱出（プログラムを終了）
            break;
        switch( (char) key ){
        case 'b':
			//	表示画像をバックプロジェクション画像に切り替える
			if( backprojectMode == HIDDEN_BACKPROJECTION ){
				backprojectMode = SHOW_BACKPROJECTION;
			}
			else{
				backprojectMode = HIDDEN_BACKPROJECTION;
			}
			break;
        case 'c':
			//	トラッキングを中止する
            trackObject = TRACKING_STOP;
            cvSetZero( histImage );
            break;
        case 'h':
			//	ヒストグラムの表示/非表示を切り替える
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
	//	キャプチャを解放する
	cvReleaseCapture( &capture );
	//	メモリを解放する
	cvReleaseImage( &resultImage );
	cvReleaseImage( &hsvImage );
	cvReleaseImage( &hueImage);
	cvReleaseImage( &maskImage );
	cvReleaseImage( &backprojectImage );
	cvReleaseImage( &histImage );
	cvReleaseImage( &grayImage );
	//	ウインドウを破棄する
	cvDestroyWindow( windowNameObjectTracking );

	return 0;
}
