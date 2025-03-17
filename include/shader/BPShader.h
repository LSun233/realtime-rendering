#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"
class BPShader : public Shader {
public:
    BPMaterial mat;
    BPLight light;
    BPShader(const char* vertexPath= "../data/shader/blinn_phong.vert", const char* fragmentPath= "../data/shader/blinn_phong.frag") :Shader(vertexPath, fragmentPath)
    {

        //设置模型材质
        mat = {
            glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
            glm::vec3(1.0f, 0.5f, 0.31f),   //diffuse
            glm::vec3(0.5f, 0.5f, 0.5f),    //specular
            32.0f };                        //shininess



        light = {
            glm::vec3(2.0,0.7,1.3),                 //postion
            glm::vec3(2.0,0.7,1.3) * glm::vec3(0.1f),//ambient
            glm::vec3(2.0,0.7,1.3) * glm::vec3(0.5f),//diffuse
            glm::vec3(1.0,1.0,1.0)                     //specular

        };

        shader_type = ShaderType::BlingPhong;
    }


    virtual void setMaterial()override
    {
        use();
        setVec3("material.ambient", mat.ambient);
        setVec3("material.diffuse", mat.diffuse);
        setVec3("material.specular", mat.specular);
        setFloat("material.shininess", mat.shininess);
    }

    virtual glm::vec3 getMaterial()override
    {
        return mat.ambient;
    }

    virtual void setLight(glm::mat4 view, glm::vec3 lightPos, glm::vec3 lightColor) override
    {

        use();
        setVec3("light.ambient", light.ambient);
        setVec3("light.diffuse", light.diffuse);
        setVec3("light.specular", light.specular);
        setVec3("light.position", lightPos);
    }
};
