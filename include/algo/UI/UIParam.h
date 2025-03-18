// UIParam.h
#ifndef UIPARAM_H
#define UIPARAM_H

#include <iostream>
#include <mutex>
#include <string>

class UIParam {
private:
    static UIParam* instance;
    static std::mutex mutex;

    UIParam();

    // ��ֹ���ƺ͸�ֵ
    UIParam(const UIParam&) = delete;
    UIParam& operator=(const UIParam&) = delete;

public:
    static UIParam* getInstance();

    void doSomething();

    float clear_color[4]{ 0.45f, 0.55f, 0.60f, 1.00f };
    bool show_demo_window = false;             //imgui demo ���ڿ���
    bool wire_mode = false;                    //�߿�ģʽ
    bool SSA0 = false;
    bool shadow = false;                 //shadowMaping
    bool ARmod = false;                 // AR
    bool shadow_debug = false;
    bool back_ground = true;                  //����
    bool pick = false;                        //��ѡ��Ƭ
    bool pick_drag = false;
    float fps = 0;                           //��ʾ֡��
    float alpha = 1.0f;                      //͸����
    int target_index = 0;
    std::string filePath = "";              //��ǰ����·��
    int control_type = 0;
    float roughness = 0.5;
    float metallic = 0.5;
    float radius = 0.5;
    float bias = 0.025;
    float albedo[3]{ 0.5f, 0.0f, 0.00f };

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    unsigned int ISCR_WIDTH = 2560;
    unsigned int ISCR_HEIGHT = 1351;
};

#endif