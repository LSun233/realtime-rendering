#pragma once
#include<vector>
#include"animator.h"
#include"Charater.h"
#include"../UI/UIParam.h"
#include"animator.h"
#include"../mesh/object.h"
    Charater::Charater(std::vector<Mesh*>meshes, std::vector <Animation*> panimators) :Object(meshes)
    {
        m_animations = panimators;
        for (int i = 0; i < m_animations.size(); i++)
        {
            Animator* animator = new Animator(m_animations[i]);
            m_animators.push_back(animator);
        }
        PlayAnimtion = true;
    }
    Charater:: ~Charater()
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

    void Charater::ShadowDraw(Camera& cam)
    {
        UIParam* ui_param = UIParam::getInstance();
        UpdateAnimation(ui_param->deltaTime);
        for (int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i]->shadowShader->use();
            //�����������
            auto transforms = m_animators[i]->GetFinalBoneMatrices();
            for (int j = 0; j < transforms.size(); ++j)
            {
                m_meshes[i]->shadowShader->setMat4("finalBonesMatrices[" + std::to_string(j) + "]", transforms[j]);
            }

            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->shadowShader->setBool("animationFlag", true);
            m_meshes[i]->Draw(m_meshes[i]->shadowShader, cam);
        }
    }

    void Charater::Draw(Camera& cam)
    {
        ////UIParam* ui_param = UIParam::getInstance();
        ////UpdateAnimation(ui_param->deltaTime);
        for (int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i]->shader->use();
            //�����������
            auto transforms = m_animators[i]->GetFinalBoneMatrices();
            for (int j = 0; j < transforms.size(); ++j)
            {
                m_meshes[i]->shader->setMat4("finalBonesMatrices[" + std::to_string(j) + "]", transforms[j]);
            }


            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->shader->setBool("animationFlag", true);
            m_meshes[i]->Draw(m_meshes[i]->shader, cam);
        }
    }


    void Charater::UpdateAnimation(float deltaTime) {
        for (int i = 0; i < m_animators.size(); i++)
        {
            m_animators[i]->UpdateAnimation(deltaTime);
        }
    }


