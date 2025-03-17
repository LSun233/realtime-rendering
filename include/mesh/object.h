#pragma once

#include<vector>
#include"Mesh.h"
class EXPORTEDCLASSES_API Object : public MeshBase
{
public:
    Object();

    Object(std::vector<Mesh*>meshes);
    Object(Mesh* mesh);

    ~Object();


    virtual void ShadowDraw(Camera& cam);

    virtual void Draw(Camera& cam);

    virtual void UpdateAnimation(float deltaTime);
public :
    std::vector<Mesh*> m_meshes;
private:




};
