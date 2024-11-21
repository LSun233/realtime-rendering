#pragma once
#ifndef PLANE_H
#define PLANE_H
#include"../MeshBase.h"
class plane : public MeshBase {
public:

    // constructor
    plane(glm::vec3 scale)
    {
        Vertex v1, v2, v3, v4;

        v1.Position = scale * glm::vec3(1, 0, -1);
        v2.Position = scale * glm::vec3(-1, 0, -1);
        v3.Position = scale * glm::vec3(-1, 0, 1);
        v4.Position = scale * glm::vec3(1, 0, 1);

        v1.Normal= glm::vec3(0, 1, 0);
        v2.Normal = glm::vec3(0, 1, 0);
        v3.Normal = glm::vec3(0, 1, 0);
        v4.Normal = glm::vec3(0, 1, 0);

        this->vertices = { v1,v2,v3,v4 };
        this->indices = { 0,1,2, 2,3,0 };

        setupMesh();
        GL_TYPE = GL_TRIANGLES;
    }
    ~plane()
    {
    }
private:


};
#endif