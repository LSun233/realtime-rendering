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

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;