#pragma once
#include"../MeshBase.h"
class boudingBox : public MeshBase
{
public:
    // constructor
    boudingBox(glm::vec3 max, glm::vec3 min);
    ~boudingBox();
private:

 
};
