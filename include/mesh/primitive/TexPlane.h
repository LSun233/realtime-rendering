#pragma once
#ifndef CANVAS_H
#define CANVAS_H
#include"../MeshBase.h"
class TexPlane : public MeshBase {
public:
    GLuint textureID;
    // constructor
    TexPlane();

    ~TexPlane();
    void draw();
private:
    unsigned int  VAO;
   
    Shader* shader;
    unsigned int createTexture();
    void  CreatTexPlane();
};
#endif