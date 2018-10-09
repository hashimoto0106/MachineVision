// Mouse.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

//�}�E�X���͗p�̃p�����[�^
struct mouseParam {
    int x;
    int y;
    int event;
    int flags;	//EVENT_FLAG_LBUTTON,EVENT_FLAG_RBUTTON,EVENT_FLAG_MBUTTON,EVENT_FLAG_CTRLKEY,EVENT_FLAG_SHIFTKEY,EVENT_FLAG_ALTKEY
};

//�R�[���o�b�N�֐�
void CallBackFunc(int eventType, int x, int y, int flags, void* userdata)
{
    mouseParam *ptr = static_cast<mouseParam*> (userdata);

    ptr->x = x;
    ptr->y = y;
    ptr->event = eventType;
    ptr->flags = flags;
}

void mouseEvent(mouseParam mouse_info)
{	
	
	if (mouse_info.event == cv::EVENT_MOUSEMOVE) {
		//std::cout << "EVENT_MOUSEMOVE" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_LBUTTONDOWN) {
		std::cout << mouse_info.x << " , " << mouse_info.y << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_RBUTTONDOWN) {
		std::cout << "EVENT_RBUTTONDOWN" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_MBUTTONDOWN) {
		std::cout << "EVENT_MBUTTONDOWN" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_LBUTTONUP) {
		std::cout << "EVENT_LBUTTONUP" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_RBUTTONUP) {
		std::cout << "EVENT_RBUTTONUP" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_MBUTTONUP) {
		std::cout << "EVENT_MBUTTONUP" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_LBUTTONDBLCLK) {
		std::cout << "EVENT_LBUTTONDBLCLK" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_RBUTTONDBLCLK) {
		std::cout << "EVENT_RBUTTONDBLCLK" << std::endl;
	}
	else if (mouse_info.event == cv::EVENT_MBUTTONDBLCLK) {
		std::cout << "EVENT_MBUTTONDBLCLK" << std::endl;
	}
}

int main(void)
{
    mouseParam mouse;

    //���͉摜
    cv::Mat input_img = cv::imread("lenna.bmp");

    //�摜�̕\��
    cv::imshow("Mouse", input_img);

    //�R�[���o�b�N�̐ݒ�
    cv::setMouseCallback("Mouse", CallBackFunc, &mouse);

    while (1) {
        cv::waitKey(20);
		mouseEvent(mouse);
    }
    return 0;
}
