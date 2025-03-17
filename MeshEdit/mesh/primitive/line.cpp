#include"line.h"

Line:: Line(glm::vec3 vertex1, glm::vec3 vertex2)
    {
        Vertex v1, v2;
        v1.Position = vertex1;
        v2.Position = vertex2;

        this->vertices = { v1,v2 };
        this->indices = {
            0,1
        };
        setupMesh();
        GL_TYPE = GL_LINES;
    }

Line::~Line()
    {
    }

