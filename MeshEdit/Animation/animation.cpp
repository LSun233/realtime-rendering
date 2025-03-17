#pragma once
#include"animation.h"
#include"Bone.h"
Animation::Animation(NodeData rootNode, std::vector<Bone>Bones, std::map<std::string, BoneInfo> BoneInfoMap)
{
    m_RootNode = rootNode;
    m_Bones = Bones;
    m_BoneInfoMap = BoneInfoMap;
}

Animation:: ~Animation()
{
}

Bone* Animation::FindBone(const std::string& name)
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


 float Animation::GetTicksPerSecond() { return m_TicksPerSecond; }

 float Animation::GetDuration() { return m_Duration; }

 const NodeData& Animation::GetRootNode() { return m_RootNode; }

const std::map<std::string, BoneInfo>& Animation::GetBoneIDMap()
{
    return m_BoneInfoMap;
}

