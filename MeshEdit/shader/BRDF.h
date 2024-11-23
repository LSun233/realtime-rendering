#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"


class BRDF : public Shader {
public:
    
    BRDF(glm::vec3 color,const char* vertexPath = "../data/shader/BRDF.vert", const char* fragmentPath = "../data/shader/BRDF.frag") :Shader(vertexPath, fragmentPath)
    {
        albedo = color;
        shader_type = ShaderType::BRDF;
    }

    virtual glm::vec3 getMaterial()override
    {
        return albedo;
    }

    virtual void setLight(glm::mat4 view, glm::vec3 lightPos, glm::vec3 lightColor)
    {
        use();
        setVec3("lightColors", lightColor);
        setVec3("lightPositionsInView", glm::vec3(view * glm::vec4(lightPos, 1.0)));
    }

    void setMaterial()
    {
        use();
        setVec3("albedo", albedo);
        setFloat("metallic", metallic);
        setFloat("roughness", roughness);
    }
};
