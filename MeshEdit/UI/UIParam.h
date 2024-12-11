#include <iostream>
#include <mutex>

#ifndef UIPARAM_H
#define UIPARAM_H

class UIParam {
private:
    static UIParam* instance;
    static std::mutex mutex;


    UIParam() {
        std::cout << "Singleton instance created." << std::endl;

    }

    // 防止复制和赋值
    UIParam(const UIParam&) = delete;
    UIParam& operator=(const UIParam&) = delete;
public:
    static UIParam* getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) {
            instance = new UIParam();
        }
        return instance;
    }

    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }

    float clear_color[4]{ 0.45f, 0.55f, 0.60f, 1.00f };
    bool show_demo_window = false;             //imgui demo 窗口控制
    bool wire_mode = false;                    //线框模式
  
    bool SSA0 = false;
    bool shadow = false;                 //shadowMaping
    bool ARmod = false;                 // AR
    bool shadow_debug = false;
    bool back_ground = true;                  //背景
    bool pick = false;                        //点选面片
    bool pick_drag = false;
    float fps = 0;                           //显示帧率
    float alpha = 1.0f;                      //透明度
    int target_index = 0;
    std::string filePath = "";              //当前加载路径
    int control_type = 0;
    float roughness = 0.5;
    float metallic = 0.5;
    float radius = 0.5;
    float bias = 0.025;
    float albedo[3]{ 0.5f, 0.0f, 0.00f };

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

};





// 初始化静态成员变量
UIParam* UIParam::instance = nullptr;
std::mutex UIParam::mutex;


#endif