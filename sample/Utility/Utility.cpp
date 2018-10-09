// Utility.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#include <iostream>

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)

#ifndef CV_CPU_POPCNT
#define CV_CPU_POPCNT CV_CPU_SSE4_2+1
#endif

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)


//OpenCVビルド情報
void showOpenCV_BuildInfo(void)
{
#if OPENCV_VERSION_CODE<OPENCV_VERSION(2,4,0)
    std::cout << "cannot support this opencv version:"
              <<  CV_VERSION << std::endl;
    return -1;
#else
  std::cout << cv::getBuildInformation() << std::endl;
#endif  std::cout << "OpenCV >= 2.0.0: " << (OPENCV_VERSION_CODE>=OPENCV_VERSION(2,0,0)?"true":"false") << std::endl;
}

//OpenCVバージョン
void showOpenCV_Version(void)
{
  std::cout << "version: " << CV_VERSION << std::endl;
  std::cout << "  major: " << CV_MAJOR_VERSION << std::endl;
  std::cout << "  minor: " << CV_MINOR_VERSION << std::endl;
  std::cout << "  subminor: " << CV_SUBMINOR_VERSION << std::endl;
  std::cout << "OpenCV >= 2.0.0: " << (OPENCV_VERSION_CODE>=OPENCV_VERSION(2,0,0)?"true":"false") << std::endl;
}

//CPUがサポートする機能チェック
void checkSupportHardware(void)
{
	using std::string;
	using std::pair;

  // 判定するCPU機能
  std::vector< pair<int, string> >flags;
  flags.push_back(pair<int,string>(CV_CPU_MMX, "MMX"));
  flags.push_back(pair<int,string>(CV_CPU_SSE, "SSE"));
  flags.push_back(pair<int,string>(CV_CPU_SSE2, "SSE 2"));
  flags.push_back(pair<int,string>(CV_CPU_SSE3, "SSE 3"));
  flags.push_back(pair<int,string>(CV_CPU_SSSE3, "SSSE 3"));
  flags.push_back(pair<int,string>(CV_CPU_SSE4_1, "SSE 4.1"));
  flags.push_back(pair<int,string>(CV_CPU_SSE4_2, "SSE 4.2"));
  flags.push_back(pair<int,string>(CV_CPU_AVX, "AVX"));
  // only for 2.3 or later
  flags.push_back(pair<int,string>(CV_CPU_POPCNT, "POPCOUNT"));

  std::vector< pair<int, string> >::const_iterator it = flags.begin();
  for(; it!=flags.end(); ++it) {
    std::cout << it->second << " is ";
    if(cv::checkHardwareSupport(it->first))
      std::cout << "supported." << std::endl;
    else
      std::cout << "NOT supported." << std::endl;
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
	showOpenCV_Version();
	checkSupportHardware();
	showOpenCV_BuildInfo();

	return 0;
}

