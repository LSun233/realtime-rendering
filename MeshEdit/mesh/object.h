#pragma once
#include<vector>
#include"Mesh.h"
class Object : public MeshBase
{
public:
    Object()
    {
    }
    Object(std::vector<Mesh*>meshes)
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            m_meshes.push_back(meshes[i]);
        }
        
        
    }
    Object(Mesh*mesh)
    {
        m_meshes.push_back(mesh);
    }

    ~Object()
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            delete m_meshes[i];
        }
    }


    virtual void ShadowDraw(Camera& cam)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->Draw(m_meshes[i]->shadowShader,cam);
        }
    
    }

    virtual void Draw(Camera& cam)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            
            //����mesh,��charater���������mesh��model����ͳһΪcharater�ľ���
            m_meshes[i]->model = GetModelMat();
            m_meshes[i]->Draw(m_meshes[i]->shader,cam);
        }
    }

    virtual void UpdateAnimation(float deltaTime)
    {


    }
public :
    std::vector<Mesh*> m_meshes;
private:




};
