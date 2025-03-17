#pragma once
#ifndef PLANE_H
#define PLANE_H
#include"../object.h"

class EXPORTEDCLASSES_API plane : public Object {



public:

    // constructor
    plane(glm::vec3 scale);
    ~plane();
private:


};
#endif