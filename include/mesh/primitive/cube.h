#pragma once
#ifndef CUBE_H
#define CUBE_H
#include"../Object.h"
class EXPORTEDCLASSES_API CUBE : public Object {
public:

    // constructor
    CUBE(glm::vec3 scale, glm::vec3 pos);
  

    ~CUBE();
private:


};
#endif