#pragma once
#ifndef CANVAS_H
#define CANVAS_H
#include"../MeshBase.h"
class TexPlane : public MeshBase {
public:
    GLuint textureID;
    // constructor
    TexPlane()
    {
    
        CreatTexPlane();

        textureID = createTexture();

        shader = new Shader("../data/shader/quad.vert", "../data/shader/quad.frag");
    }

    ~TexPlane()
    {
    }
    void draw()
    {
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, textureID);
        //shader->use();
        //shader->setInt("screenTexture", 0);

        shader->SetTexture2D("screenTexture", textureID);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
private:
    unsigned int  VAO;
   
    Shader* shader;
    unsigned int createTexture()
    {
        GLuint textureID;
        // 创建一个纹理对象，并返回一个独一无二的标识保存在textureID中
        glGenTextures(1, &textureID);
        // 绑定textureID标识的纹理，之后的所有操作都是相对于该纹理的
        glBindTexture(GL_TEXTURE_2D, textureID);
        // 设置纹理环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // 设置纹理过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 解绑
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }
    void  CreatTexPlane()
    {
        //创建纹理的载体，以显示摄像头的内容
        float vertices[] = {
            // positions           // texture coords OPENCV与opengl是反着的
            -1.0f, -1.0f, 0.0f,   0.0f, 1.0f,
             1.0f, -1.0f, 0.0f,   1.0f, 1.0f,
             1.0f,  1.0f, 0.0f,   1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,   0.0f, 0.0f
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
       
        unsigned int VBO,EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

      
    }
};
#endif