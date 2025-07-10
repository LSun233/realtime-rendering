#pragma once

#include<vector>
#include"Mesh.h"
class EXPORTEDCLASSES_API Object : public MeshBase
{
public:
    Object(string _name = "");
    Object(std::vector<Mesh*>meshes,string _name="");
    Object(Mesh* mesh , string _name = "");
    ~Object();


    virtual void ShadowDraw(Camera& cam);

    virtual void Draw(Camera& cam);

    virtual void UpdateAnimation(float deltaTime);
public :
    std::vector<Mesh*> m_meshes;
    bool active;

    std::string name;
private:




};
