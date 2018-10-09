// Trackbar.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>

void on_trackbar1(int val); 
void on_trackbar2(int val); 
 
int main (int argc, char **argv) { 
  // �E�B���h�E�C����уg���b�N�o�[���쐬���� 
  cvNamedWindow("Window", CV_WINDOW_AUTOSIZE); 
  cvCreateTrackbar("Trackbar1", "Window", 0, 100, on_trackbar1); 
  cvCreateTrackbar("Trackbar2", "Window", 0, 100, on_trackbar2); 
  cvShowImage("Window", NULL); 
  cvWaitKey(0); 
  cvDestroyWindow ("Image"); 
  return 0; 
} 
 
/* �R�[���o�b�N�֐� */ 
void on_trackbar1(int val) { 
  // �g���b�N�o�[2���C�g���b�N�o�[1�ɓ��������� 
  cvSetTrackbarPos ("Trackbar2", "Window", val); 
} 
 
void on_trackbar2(int val) { 
  int pos1, pos2; 
  // �g���b�N�o�[2�̈ړ��͈͂��C�g���b�N�o�[1�̒l�}20�Ɍ��肷�� 
  pos1 = cvGetTrackbarPos ("Trackbar1", "Window"); 
  if (pos1 > val) pos2 = pos1 - 20 < val ? val : pos1 - 20; 
  else pos2 = pos1 + 20 > val ? val : pos1 + 20; 
  cvSetTrackbarPos("Trackbar2", "Window", pos2); 
} 
