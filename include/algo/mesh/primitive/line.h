
#ifndef LINE_H
#define LINE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include"../../camera.h"
#include"../MeshBase.h"
class  Line : public MeshBase
{
public:
    Line(glm::vec3 vertex1, glm::vec3 vertex2);

    ~Line();
private:

 


};
#endif
