// Detector.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

int
main(int argc, char *argv[])
{
//  const char *imagename = argc > 1 ? argv[1] : "./lenna.bmp";
  const char *imagename = argc > 1 ? argv[1] : "./man.png";
  cv::Mat img = cv::imread(imagename, 1);
  if(!img.data) return -1; 
  
  double scale = 4.0;
  cv::Mat gray, smallImg(cv::saturate_cast<int>(img.rows/scale), cv::saturate_cast<int>(img.cols/scale), CV_8UC1);
  // �O���[�X�P�[���摜�ɕϊ�
  cv::cvtColor(img, gray, CV_BGR2GRAY);
  // �������ԒZ�k�̂��߂ɉ摜���k��
  cv::resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
  cv::equalizeHist( smallImg, smallImg);

  // ���ފ�̓ǂݍ���
  std::string cascadeName = "./haarcascade_frontalface_alt.xml"; // Haar-like
  //std::string cascadeName = "./lbpcascade_frontalface.xml"; // LBP
  cv::CascadeClassifier cascade;
  if(!cascade.load(cascadeName))
    return -1;

  std::vector<cv::Rect> faces;
  // �}���`�X�P�[���i��j�T��
  // �摜�C�o�͋�`�C�k���X�P�[���C�Œ��`���C�i�t���O�j�C�ŏ���`
  cascade.detectMultiScale(smallImg, faces,
                           1.1, 2,
                           CV_HAAR_SCALE_IMAGE,
                           cv::Size(30, 30));

  // ���ʂ̕`��
  std::vector<cv::Rect>::const_iterator r = faces.begin();
  for(; r != faces.end(); ++r) {
    cv::Point center;
    int radius;
    center.x = cv::saturate_cast<int>((r->x + r->width*0.5)*scale);
    center.y = cv::saturate_cast<int>((r->y + r->height*0.5)*scale);
    radius = cv::saturate_cast<int>((r->width + r->height)*0.25*scale);
    cv::circle( img, center, radius, cv::Scalar(80,80,255), 3, 8, 0 );
  }

  cv::namedWindow("result", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow( "result", img );    
  cv::waitKey(0);
}