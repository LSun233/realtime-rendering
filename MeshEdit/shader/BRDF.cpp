#include"BRDF.h"
BRDF::BRDF(glm::vec3 color, const char* vertexPath , const char* fragmentPath ) :Shader(vertexPath, fragmentPath)
    {
        albedo = color;
        shader_type = ShaderType::BRDF;
    }

 glm::vec3 BRDF::getMaterial()
 {
        return albedo;
 }

 void BRDF::setLight(glm::mat4 view, glm::vec3 lightPos, glm::vec3 lightColor)
 {
        use();
        setVec3("lightColors", lightColor);
        setVec3("lightPositionsInView", glm::vec3(view * glm::vec4(lightPos, 1.0)));
 }

void BRDF::setMaterial()
{
        use();
        setVec3("albedo", albedo);
        setFloat("metallic", metallic);
        setFloat("roughness", roughness);
}

