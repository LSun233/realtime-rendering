#pragma once
#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif

class EXPORTEDCLASSES_API BRDF : public Shader {
public:
    
    BRDF(glm::vec3 color, const char* vertexPath = "../data/shader/BRDF.vert", const char* fragmentPath = "../data/shader/BRDF.frag") ;

    virtual glm::vec3 getMaterial()override;

    virtual void setLight(glm::mat4 view, glm::vec3 lightPos, glm::vec3 lightColor);

    void setMaterial();
};
