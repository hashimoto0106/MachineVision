// Trackbar.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>

void on_trackbar1(int val); 
void on_trackbar2(int val); 
 
int main (int argc, char **argv) { 
  // ウィンドウ，およびトラックバーを作成する 
  cvNamedWindow("Window", CV_WINDOW_AUTOSIZE); 
  cvCreateTrackbar("Trackbar1", "Window", 0, 100, on_trackbar1); 
  cvCreateTrackbar("Trackbar2", "Window", 0, 100, on_trackbar2); 
  cvShowImage("Window", NULL); 
  cvWaitKey(0); 
  cvDestroyWindow ("Image"); 
  return 0; 
} 
 
/* コールバック関数 */ 
void on_trackbar1(int val) { 
  // トラックバー2を，トラックバー1に同期させる 
  cvSetTrackbarPos ("Trackbar2", "Window", val); 
} 
 
void on_trackbar2(int val) { 
  int pos1, pos2; 
  // トラックバー2の移動範囲を，トラックバー1の値±20に限定する 
  pos1 = cvGetTrackbarPos ("Trackbar1", "Window"); 
  if (pos1 > val) pos2 = pos1 - 20 < val ? val : pos1 - 20; 
  else pos2 = pos1 + 20 > val ? val : pos1 + 20; 
  cvSetTrackbarPos("Trackbar2", "Window", pos2); 
} 
