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

    // ��ֹ���ƺ͸�ֵ
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
    bool show_demo_window = false;             //imgui demo ���ڿ���
    bool wire_mode = false;                    //�߿�ģʽ
    int  shader_type = 0;                       //shader����
    bool SSA0 = false;
    bool hard_shadow = false;                 //shadowMaping
    bool shadow_debug = false;
    bool back_ground = true;                  //����
    bool pick = false;                        //��ѡ��Ƭ
    bool pick_drag = false;
    float fps = 0;                           //��ʾ֡��
    float alpha = 1.0f;                      //͸����
    int target_index = 0;
    std::string filePath = "";              //��ǰ����·��
    int control_type = 0;
};

// ��ʼ����̬��Ա����
UIParam* UIParam::instance = nullptr;
std::mutex UIParam::mutex;


#endif