#ifndef TRIANGLFACE_H
#define TRIANGLFACE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"../type_define.h"
#include<string>
#include <glad/glad.h>
#include "../shader_m.h"
#include"../camera.h"
class TriangleFace {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    glm::mat4 Model;
    string name;

    // constructor
    TriangleFace(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3)
    {
        Vertex v1, v2, v3;
        v1.Position = vertex1;
        v2.Position = vertex2;
        v3.Position = vertex3;

        this->vertices = { v1,v2,v3 };
        this->indices = {
            0,1,2
          
        };
        setupMesh();

        Model = glm::mat4(1.0f);
    }

    // render the mesh
    void Draw(Shader& shader, Camera& cam)
    {
        shader.setVec3("viewPos", cam.Position);
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();

        shader.setMat4("model", Model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
    ~TriangleFace()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
private:
    // render data 
    glm::mat4 Rotation;
    glm::vec3 Position;
    unsigned int VAO, VBO, EBO;
    // initializes all the buffer objects/arrays
    void setupMesh()
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


};
#endif