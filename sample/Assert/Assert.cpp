// Assert.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <iostream>
#include <opencv2/core/core.hpp>

int
main(int argc, char *argv[])
{
  // 3x3 �̍s��
  cv::Mat m1 = (cv::Mat_<double>(3,3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

  try {
    // �f�o�b�O���̂ݗL��
    CV_DbgAssert(m1.type()==CV_8UC1);
  } catch(cv::Exception e) {
    std::cout << "CV_DbgAssert !!" << std::endl;
  }
  std::cout << std::endl;

  try {
    // ��ɗL��
    CV_Assert(m1.type()==CV_8UC1);
  } catch(cv::Exception e) {
    std::cout << "CV_Assert !!" << std::endl;
  }
}
