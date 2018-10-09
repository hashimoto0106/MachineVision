// Assert.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <opencv2/core/core.hpp>

int
main(int argc, char *argv[])
{
  // 3x3 の行列
  cv::Mat m1 = (cv::Mat_<double>(3,3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

  try {
    // デバッグ時のみ有効
    CV_DbgAssert(m1.type()==CV_8UC1);
  } catch(cv::Exception e) {
    std::cout << "CV_DbgAssert !!" << std::endl;
  }
  std::cout << std::endl;

  try {
    // 常に有効
    CV_Assert(m1.type()==CV_8UC1);
  } catch(cv::Exception e) {
    std::cout << "CV_Assert !!" << std::endl;
  }
}
