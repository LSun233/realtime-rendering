#pragma once
#ifndef MESHBase_H
#define MESHBase_H
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include"../type_define.h"
#include"../BVH.h"
#include"../halfEdge.h"
#include"../shader/shader_m.h"
class Camera;
class EXPORTEDCLASSES_API MeshBase {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    string name;
    int GL_TYPE;

    Shader* shader=nullptr;
    Shader* shadowShader = nullptr;

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

    bool PlayAnimtion=false;

    // constructor
    MeshBase(glm::vec3 postion = glm::vec3(0, 0, 0));


    virtual void Draw(Shader* shaderpass, Camera& cam);


    virtual void setupMesh();

    void rotation(glm::vec3 axis, float angle);
    void translate(glm::vec3 trans);
    glm::vec3 GetPostion();
    glm::mat4 GetRotation();
    void SetRotation(glm::qua<float> rotation);
    void SetScale(glm::vec3 scale);
    glm::mat4 GetModelMat();
    void SetModelMat(glm::mat4 mat);
    void CreatAABB();

     // TOPO
    void CreatBVH();

    void CreatHalfEdgeMesh();

    float hit(Ray r);
    float hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB);
    ~MeshBase();

private:

};
#endif
