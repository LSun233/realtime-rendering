#pragma once
#include"mesh/MeshBase.h"
class boudingBox : public MeshBase
{
public:
    // constructor
    boudingBox(glm::vec3 max, glm::vec3 min);
    ~boudingBox();
private:

 
};
