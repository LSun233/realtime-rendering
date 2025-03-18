#pragma once
#include <vector>
#include <string>
#include <map>
#include"../type_define.h"
#include <glm/glm.hpp>

class Bone;
class Animation
{
public:
    Animation(NodeData rootNode, std::vector<Bone>Bones, std::map<std::string, BoneInfo> BoneInfoMap);
    ~Animation();
     Bone* FindBone(const std::string& name);
     float GetTicksPerSecond();
     float GetDuration();
     const NodeData& GetRootNode();
     const std::map<std::string, BoneInfo>& GetBoneIDMap();
private:
    float m_Duration=1;
    float m_TicksPerSecond=0.5;
    std::vector<Bone> m_Bones;
    NodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};