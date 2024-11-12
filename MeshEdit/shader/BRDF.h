#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"
class BRDF : public Shader {
public:
    BPMaterial mat;
    BPLight light;
    float roughness = 0.5f;
    float metallic = 0.5f;
    glm::vec3 albedo = glm::vec3(0.5f, 0.0f, 0.0f);
    BRDF(const char* vertexPath = "../data/shader/BRDF.vert", const char* fragmentPath = "../data/shader/BRDF.frag") :Shader(vertexPath, fragmentPath)
    {

        //设置模型材质
        //mat = {
        //    glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
        //    glm::vec3(1.0f, 0.5f, 0.31f),   //diffuse
        //    glm::vec3(0.5f, 0.5f, 0.5f),    //specular
        //    32.0f };                        //shininess

        light = {
            glm::vec3(2.0,0.7,1.3),                 //postion
            glm::vec3(2.0,0.7,1.3) * glm::vec3(0.3f),//ambient
            glm::vec3(300.0f, 300.0f, 300.0f) * glm::vec3(0.5f),//diffuse
            glm::vec3(1.0,1.0,1.0)                     //specular

        };

        shader_type = ShaderType::BRDF;
    }

    void activate() override {
        use();
        setVec3("lightColors",    light.diffuse);
        setVec3("lightPositions", light.position);
        setVec3("albedo", albedo);
        setFloat("ao", 1.0f);
        setFloat("metallic", metallic);
        setFloat("roughness", roughness);
    }


    void setmatrix(glm::mat4 model, glm::vec3 Position)
    {
        use();
        setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
        setVec3("camPos", Position);
    }
};
