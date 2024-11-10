#pragma once
#ifndef MESHBase_H
#define MESHBase_H
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../shader/shader_m.h"
#include <string>
#include <vector>
#include"../camera.h"
#include"../type_define.h"

class MeshBase {


public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    string name;
    int GL_TYPE;
    Shader* shader=nullptr;

    // render data 
    unsigned int VAO, VBO, EBO;

    // posture
    glm::vec3 Scale;
    glm::mat4 model;
    glm::mat4 Rotation;
    glm::vec3 Position;
    glm::qua<float> Rotation_qua;

    vector<Triangle*> hitRes;
    vector<int> suroundingFace;


    AABB aabb;
    BVH::BVH* bvh;
    HalfEdge::HalfEdgeMesh* halfEdge;

    // constructor
    MeshBase()
    {
        model = glm::mat4(1.0f);
    }


    virtual void Draw(Camera& cam)
    {
        if (shader == nullptr)
        {
            shader = new SimpleShader;
        }
        
        shader->activate();
        shader->setVec3("viewPos", cam.Position);
        shader->setMVPmatrix(GetModelMat(), cam.GetViewMatrix(), cam.GetPerspectiveMatrix());

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TYPE, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }


    virtual void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    }


     void rotation(glm::vec3 axis, float angle)
    {
        
        glm::mat4 transM = glm::mat4(1.0f);
        transM = glm::rotate(transM, angle, axis);
        model = model * transM;

    }
     void translate(glm::vec3 trans)
    {
        glm::mat4 transM = glm::mat4(1.0f);
        transM = glm::translate(transM, trans);
        model = transM * model;
    }
     glm::mat4 GetRotation()
    {
        return Rotation;
    }
     void SetRotation(glm::qua<float> rotation)
    {
        Rotation_qua = rotation;
        Rotation = glm::mat4_cast(Rotation_qua);

        // model = glm::mat4(1.0f);
        model = Rotation * model;
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);
    }
     void SetScale(glm::vec3 scale)
    {
        Scale = scale;

        model = glm::mat4(1.0f);
        model = Rotation * model;
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);
    }
     glm::mat4 GetModelMat()
    {
        return model;
    }
     void SetModelMat(glm::mat4 mat)
    {
        model = mat;
    }

     void CreatAABB()
     {
         float xmax = -9999999;
         float ymax = -9999999;
         float zmax = -9999999;
         float xmin = +9999999;
         float ymin = +9999999;
         float zmin = +9999999;


         for (int j = 0; j < vertices.size(); j++)
         {
             // ¼ÆËãpMax
             if (xmax < vertices[j].Position.x)
                 xmax = vertices[j].Position.x;
             if (ymax < vertices[j].Position.y)
                 ymax = vertices[j].Position.y;
             if (zmax < vertices[j].Position.z)
                 zmax = vertices[j].Position.z;

             // ¼ÆËãpMin
             if (xmin > vertices[j].Position.x)
                 xmin = vertices[j].Position.x;
             if (ymin > vertices[j].Position.y)
                 ymin = vertices[j].Position.y;
             if (zmin > vertices[j].Position.z)
                 zmin = vertices[j].Position.z;
         }
         aabb.max = glm::vec3(xmax, ymax, zmax);
         aabb.min = glm::vec3(xmin, ymin, zmin);
         aabb.center = 0.5f * (aabb.max + aabb.min);

         //aabb.max -= aabb.center;
         //aabb.min -= aabb.center;
     }

     // TOPO
     void CreatBVH()
     {
         bvh = new BVH::BVH(vertices, indices);
     }
     void CreatHalfEdgeMesh()
     {
         halfEdge = new HalfEdge::HalfEdgeMesh(vertices.size(), indices);
         std::cout << "face num: " << halfEdge->m_faces.size() << std::endl;
     }
     float hit(Ray r)
     {
         return hitAABB(r, aabb.min, aabb.max);
     
     }
     float hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB)
     {
         // 1.0 / direction
         glm::vec3 invdir = glm::vec3(1.0 / r.direction.x, 1.0 / r.direction.y, 1.0 / r.direction.z);

         glm::vec3 in = (BB - r.startPoint) * invdir;
         glm::vec3 out = (AA - r.startPoint) * invdir;

         glm::vec3 tmax = max(in, out);
         glm::vec3 tmin = min(in, out);

         float t1 = glm::min(tmax.x, glm::min(tmax.y, tmax.z));
         float t0 = glm::max(tmin.x, glm::max(tmin.y, tmin.z));

         return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
     }
    ~MeshBase()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }


   
private:


};
#endif
