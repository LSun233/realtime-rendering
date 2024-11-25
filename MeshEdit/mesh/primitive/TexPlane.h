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
        // ����һ��������󣬲�����һ����һ�޶��ı�ʶ������textureID��
        glGenTextures(1, &textureID);
        // ��textureID��ʶ������֮������в�����������ڸ������
        glBindTexture(GL_TEXTURE_2D, textureID);
        // ���������Ʒ�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // ����������˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // ���
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }
    void  CreatTexPlane()
    {
        //������������壬����ʾ����ͷ������
        float vertices[] = {
            // positions           // texture coords OPENCV��opengl�Ƿ��ŵ�
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