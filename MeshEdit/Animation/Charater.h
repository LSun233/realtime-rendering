#pragma once
#include<vector>
#include"animator.h"
#include"../mesh/object.h"
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



class Charater: public Object
{

public:

	Charater(std::vector<Mesh*>meshes, std::vector <Animation*> panimators):Object(meshes)
	{
        m_animations = panimators;
        for (int i = 0; i < m_animations.size(); i++)
        {
            Animator* animator=new Animator(m_animations[i]);
            m_animators.push_back(animator);
        }
        PlayAnimtion = true;
	}
    ~Charater()
    {
        for (int i = 0; i < m_animators.size(); i++)
        {
            delete m_animators[i];
        }

        for (int i = 0; i < m_animations.size(); i++)
        {
            delete m_animations[i];
        }


    }


    void ShadowDraw(Camera& cam)
    {
        UIParam* ui_param = UIParam::getInstance();
        UpdateAnimation(ui_param->deltaTime);
        for (int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i]->shadowShader->use();
            //传入骨骼矩阵
            auto transforms = m_animators[i]->GetFinalBoneMatrices();
            for (int j = 0; j < transforms.size(); ++j)
            {
                m_meshes[i]->shadowShader->setMat4("finalBonesMatrices[" + std::to_string(j) + "]", transforms[j]);
            }
           
            //绘制mesh,将charater下面的所有mesh的model矩阵统一为charater的矩阵
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->shadowShader->setBool("animationFlag", true);
            m_meshes[i]->Draw(m_meshes[i]->shadowShader, cam);
        }
    }

    void Draw(Camera& cam)
    {
        ////UIParam* ui_param = UIParam::getInstance();
        ////UpdateAnimation(ui_param->deltaTime);
        for (int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i]->shader->use();
            //传入骨骼矩阵
            auto transforms = m_animators[i]->GetFinalBoneMatrices();
            for (int j = 0; j < transforms.size(); ++j)
            {
                m_meshes[i]->shader->setMat4("finalBonesMatrices[" + std::to_string(j) + "]", transforms[j]);
            }
           

            //绘制mesh,将charater下面的所有mesh的model矩阵统一为charater的矩阵
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->shader->setBool("animationFlag",true);
            m_meshes[i]->Draw(m_meshes[i]->shader,cam);
        }
    }


    void UpdateAnimation(float deltaTime) {
        for (int i = 0; i < m_animators.size(); i++)
        {
            m_animators[i]->UpdateAnimation(deltaTime);
        }
    }

private:
    std::vector <Animation*> m_animations;
    std::vector <Animator*> m_animators;

private:




};
