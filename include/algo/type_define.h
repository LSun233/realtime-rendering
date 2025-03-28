#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glm/gtc/quaternion.hpp>
using namespace std;
#define MAX_BONE_INFLUENCE 4
#define INF 9999999.0

#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};
struct NodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount=0;
    std::vector<NodeData> children;
};

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
    int m_BoneIDs[MAX_BONE_INFLUENCE] = {-1,-1,-1,-1};
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE] = {0,0,0,0};
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

enum class ShaderType {
    BlingPhong,BRDF,Simple
};


struct AABB
{
public:
    AABB() {};
    glm::vec3 max = glm::vec3(-9999999.0f, -9999999.0f, -9999999.0f);
    glm::vec3 min = glm::vec3(9999999.0f, 9999999.0f, 9999999.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);;

};
struct BVHNode {
    BVHNode* left = NULL;       // 左右子树索引
    BVHNode* right = NULL;
    int n, index;               // 叶子节点信息               
    AABB aabb;                // 碰撞盒
};
// 光线
typedef struct Ray {

    glm::vec3 startPoint = glm::vec3(0, 0, 0);    // 起点
    glm::vec3 direction = glm::vec3(0, 0, 0);     // 方向
} Ray;

struct Triangle {
    glm::vec3 p1, p2, p3;   // 三点
    glm::vec3 center;       // 中心
    int index;              //面片的索引

    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        p1 = a, p2 = b, p3 = c;
        center = (p1 + p2 + p3) / glm::vec3(3, 3, 3);
        index = -1;
    }
};

struct HitResult {
    Triangle* triangle = NULL;
    float distance = INF;
    int index = -1;
};

extern  unsigned int ISCR_WIDTH;
extern  unsigned int ISCR_HEIGHT;

#endif

