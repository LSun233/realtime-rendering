#pragma once

#include <glm/glm.hpp>
#include <vector>
using namespace std;
#define MAX_BONE_INFLUENCE 4
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    //Normal
    glm::vec3 Color;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct BPMaterial {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
struct BPLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct AABB
{
    glm::vec3 max = glm::vec3(-9999999.0f, -9999999.0f, -9999999.0f);
    glm::vec3 min = glm::vec3(9999999.0f, 9999999.0f, 9999999.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);;

};
struct Gui_param
{
    Gui_param() {};
    float clear_color[4]{ 0.45f, 0.55f, 0.60f, 1.00f };
    bool show_demo_window = false;         //imgui demo 窗口控制
    bool wire_mode = false;                //线框模式
    bool SSA0 = false;
    bool hard_shadow = false;
    bool shadow_debug = false;
    bool back_ground = true;
    float fps = 0;                           //显示帧率
    float alpha = 1.0f;
    string filePath = "";                    //当前加载路径
};
 unsigned int SCR_WIDTH = 800;
 unsigned int SCR_HEIGHT = 600;

