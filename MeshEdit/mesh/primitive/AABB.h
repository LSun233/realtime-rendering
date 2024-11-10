#pragma once
#include"../MeshBase.h"
class boudingBox : public MeshBase
{
public:
    // constructor
    boudingBox(glm::vec3 max, glm::vec3 min)
    {
        Vertex v1,v2,v3,v4,v5,v6,v7,v8;
        v1.Position = max;
        v2.Position = glm::vec3(max.x,min.y,max.z);
        v3.Position = glm::vec3(min.x, min.y, max.z);
        v4.Position = glm::vec3(min.x, max.y, max.z);

        v5.Position = glm::vec3(max.x, max.y, min.z);
        v6.Position = glm::vec3(max.x, min.y, min.z);
        v7.Position = glm::vec3(min.x, min.y, min.z);
        v8.Position = glm::vec3(min.x, max.y, min.z);

        this->vertices = {v1,v2,v3,v4,v5,v6,v7,v8};
        this->indices = {
            0,1, 1,2, 2,3, 3,0, 
            4,5, 5,6, 6,7, 7,4,
            0,4,1,5,2,6,3,7
        };
        setupMesh();
        GL_TYPE = GL_LINES;
    }

    ~boudingBox()
    {

    }
private:

 
};
