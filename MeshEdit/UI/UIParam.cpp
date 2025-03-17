// UIParam.cpp
#include "UIParam.h"

// 初始化静态成员变量
UIParam* UIParam::instance = nullptr;
std::mutex UIParam::mutex;

// 构造函数的定义
UIParam::UIParam() {
    std::cout << "Singleton instance created." << std::endl;
}

// getInstance 函数的定义
UIParam* UIParam::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new UIParam();
    }
    return instance;
}

// doSomething 函数的定义
void UIParam::doSomething() {
    std::cout << "Doing something..." << std::endl;
}