#ifndef MESH_H
#define MESH_H
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_m.h"
#include <string>
#include <vector>
#include"PLY.h"
#include"type_define.h"



class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::mat4 Rotation;
    glm::qua<float> Rotation_qua;
    AABB aabb;
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
        setupMesh();
 

    }
    Mesh(string pathPly)
    {
        plyRead(pathPly, this->vertices, this->indices);
        setupMesh();
    }

    void loadTex(vector<Texture> textures)
    {
        this->textures = textures;
    }

    // render the mesh
    void Draw(Shader& shader, BPMaterial& mat, BPLight& light, Camera& cam)
    {
        shader.setVec3("viewPos", cam.Position);
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = cam.GetViewMatrix();

        shader.setMat4("model", GetModelMat());
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);   
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    void SetPostion(glm::vec3 position)
    {
        Position = position;
    }
    void SetRotation(glm::qua<float> rotation)
    {
        Rotation_qua = rotation;
        Rotation=glm::mat4_cast(Rotation_qua);
    }
    void SetScale(glm::vec3 scale)
    {
        Scale = scale;
    }
    glm::mat4 GetModelMat()
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = Rotation * model;
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);

        return model;
    }
    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
private:
    // render data 
     
    unsigned int VAO,VBO, EBO;
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
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); //offsetof ��һ����Ա�ڽṹ���е�ƫ����
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);

        Position = glm::vec3(0, 0, 0);
        Scale = glm::vec3(1.0, 1.0, 1.0);
        Rotation_qua = glm::qua<float>(1.0,0.0,0.0,0.0);
        Rotation=glm::mat4_cast(Rotation_qua);

    }
    void GetAABB()
    {
        float xmax = -9999999;
        float ymax = -9999999;
        float zmax = -9999999;
        float xmin = +9999999;
        float ymin = +9999999;
        float zmin = +9999999;

     
            for (int j = 0; j < vertices.size(); j++)
            {
                // 计算pMax
                if (xmax < vertices[j].Position.x)
                    xmax = vertices[j].Position.x;
                if (ymax <vertices[j].Position.y)
                    ymax = vertices[j].Position.y;
                if (zmax <vertices[j].Position.z)
                    zmax = vertices[j].Position.z;

                // 计算pMin
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
        aabb.max -= aabb.center;
        aabb.min -= aabb.center;
    }


   

};
#endif
