#pragma once
#include<vector>
#include"../mesh/MeshBase.h"
#include"animator.h"

enum class AnimationState { 
	Idle, 
	Walking,
	Running, 
	Jumping 
};


class AnimationStateBase {
public:
	virtual ~AnimationStateBase() = default;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
};


class IdleState : public AnimationStateBase {
public:
    void Enter() override {
        std::cout << "Entering Idle State" << std::endl;
    }
    void Update() override {
        std::cout << "Updating Idle State" << std::endl;
    }
    void Exit() override {
        std::cout << "Exiting Idle State" << std::endl;
    }
};


class WalkingState : public AnimationStateBase {
public:
    void Enter() override {
        std::cout << "Entering Walking State" << std::endl;
    }
    void Update() override {
        std::cout << "Updating Walking State" << std::endl;
    }
    void Exit() override {
        std::cout << "Exiting Walking State" << std::endl;
    }
};



class Charater
{

public:
	Charater(std::vector<MeshBase*>meshes, Animator* panimators)
	{
        m_animators = panimators;
        m_meshes = meshes;

	}
    void performAction(Camera& cam)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
           //´«Èë¹Ç÷À¾ØÕó
            auto transforms = m_animators->GetFinalBoneMatrices();
            m_meshes[i]->shader->use();
            for (int j = 0; j < transforms.size(); ++j)
            {
                m_meshes[i]->shader->setMat4("finalBonesMatrices[" + std::to_string(j) + "]", transforms[j]);

            }
            //»æÖÆmesh
            m_meshes[i]->Draw(cam);
        }
    }

    void Update() {
    }

public:
	std::vector<MeshBase*> m_meshes;
	Animator* m_animators;

};
