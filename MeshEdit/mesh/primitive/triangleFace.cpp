#include"triangleFace.h"

    // constructor
TriangleFace::TriangleFace(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3)
    {
        Vertex v1, v2, v3;
        v1.Position = vertex1;
        v2.Position = vertex2;
        v3.Position = vertex3;

        this->vertices = { v1,v2,v3 };
        this->indices = { 0,1,2 };

        setupMesh();
        GL_TYPE = GL_TRIANGLES;
    }

TriangleFace::~TriangleFace()
    {
    }
