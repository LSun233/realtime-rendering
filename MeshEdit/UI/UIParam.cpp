// UIParam.cpp
#include "UIParam.h"

// ��ʼ����̬��Ա����
UIParam* UIParam::instance = nullptr;
std::mutex UIParam::mutex;

// ���캯���Ķ���
UIParam::UIParam() {
    std::cout << "Singleton instance created." << std::endl;
}

// getInstance �����Ķ���
UIParam* UIParam::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new UIParam();
    }
    return instance;
}

// doSomething �����Ķ���
void UIParam::doSomething() {
    std::cout << "Doing something..." << std::endl;
}