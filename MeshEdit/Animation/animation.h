#pragma once
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include"Bone.h"
struct NodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<NodeData> children;
};

class Animation
{
public:
    Animation() = default;

    Animation()
    {
       
    }

    ~Animation()
    {
    }

    Bone* FindBone(const std::string& name)
    {
        auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
            [&](const Bone& Bone)
            {
                return Bone.GetBoneName() == name;
            }
        );
        if (iter == m_Bones.end()) return nullptr;
        else return &(*iter);
    }


    inline float GetTicksPerSecond() { return m_TicksPerSecond; }

    inline float GetDuration() { return m_Duration; }

    inline const NodeData& GetRootNode() { return m_RootNode; }

    inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
    {
        return m_BoneInfoMap;
    }

private:

    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    NodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};