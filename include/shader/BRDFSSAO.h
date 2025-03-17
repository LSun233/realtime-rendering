#pragma once
#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"

class BRDFSSAO : public Shader {
public:

    float roughness = 0.5f;
    float metallic = 0.5f;
   
    BRDFSSAO(const char* vertexPath = "../data/shader/ssao.vert", const char* fragmentPath = "../data/shader/ssao_brdf_lighting.frag") :Shader(vertexPath, fragmentPath)
    {
        shader_type = ShaderType::BRDF;
    }

    void setMaterial()override
    {
        use();
        setFloat("metallic", metallic);
        setFloat("roughness", roughness);
    }


    void setLight(glm::mat4 view, glm::vec3 lightPos, glm::vec3 lightColor)override
    {
        use();
        setVec3("lightColors", lightColor);
        setVec3("lightPositionsInView", glm::vec3(view * glm::vec4(lightPos, 1.0)));
    }
};
