
#include "object.h"
Object::Object()
    {
    }
Object::Object(std::vector<Mesh*>meshes)
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            m_meshes.push_back(meshes[i]);
        }


    }
Object::Object(Mesh* mesh)
    {
        m_meshes.push_back(mesh);
    }

Object::~Object()
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            delete m_meshes[i];
        }
    }


void Object::ShadowDraw(Camera& cam)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->Draw(m_meshes[i]->shadowShader, cam);
        }

    }

void Object::Draw(Camera& cam)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {

            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->Draw(m_meshes[i]->shader, cam);
        }
    }

void Object::UpdateAnimation(float deltaTime)
    {


    }

