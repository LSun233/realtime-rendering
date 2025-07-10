#include<vector>
#include "mesh/object.h"
Object::Object(string _name )
    {
         name = _name;
         active= true;
    }
Object::Object(std::vector<Mesh*>meshes,  string _name )
{
        for (int i = 0; i < meshes.size(); i++)
        {
            m_meshes.push_back(meshes[i]);
            
        }
        name = _name;
        active = true;
 }
Object::Object(Mesh* mesh, string _name )
{
        m_meshes.push_back(mesh);
        name = _name;
        active = true;
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
        //绘制mesh,将charater下面的所有mesh的model矩阵统一为charater的矩阵
        m_meshes[i]->model = GetModelMat();
        m_meshes[i]->Draw(m_meshes[i]->shadowShader, cam);
    }

}

void Object::Draw(Camera& cam)
{
    for (int i = 0; i < m_meshes.size(); i++)
    {

        //绘制mesh,将charater下面的所有mesh的model矩阵统一为charater的矩阵
        m_meshes[i]->model = GetModelMat();
        m_meshes[i]->Draw(m_meshes[i]->shader, cam);
    }
}

void Object::UpdateAnimation(float deltaTime)
    {


    }

