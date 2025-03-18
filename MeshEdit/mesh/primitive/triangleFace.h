#ifndef TRIANGLFACE_H
#define TRIANGLFACE_H
#include"mesh/MeshBase.h"
class TriangleFace : public MeshBase {
public:

    // constructor
    TriangleFace(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);

    ~TriangleFace();
private:


};
#endif