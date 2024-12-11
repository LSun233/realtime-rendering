#pragma once
#ifndef PLANE_H
#define PLANE_H
#include"../object.h"
#include"../../shader/BRDF.h"
#include"../../shader/SimpleShader.h"
class plane : public Object {
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

        vector<Vertex> vertices = { v1,v2,v3,v4 };
        vector<unsigned int>indices = { 0,1,2, 2,3,0 };
        Mesh* mesh = new Mesh(vertices, indices);
        BRDF* shader_floor = new BRDF(glm::vec3(0.953, 0.943, 0.95));
        mesh->shader = shader_floor;
        mesh->shader->use();
        

        m_meshes.push_back(mesh);
        GL_TYPE = GL_TRIANGLES;

    }
    ~plane()
    {
    }
private:


};
#endif