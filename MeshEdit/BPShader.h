#include"shader_m.h"
#include"type_define.h"
#include"camera.h"
class BPShader : public Shader {
public:
    BPShader(const char* vertexPath, const char* fragmentPath) :Shader(vertexPath, fragmentPath)
    {
    
    }
    void activate(BPMaterial& mat, BPLight& light) {
        use();
       
        setVec3("light.ambient", light.ambient);
        setVec3("light.diffuse", light.diffuse);
        setVec3("light.specular", light.specular);
        setVec3("light.position", light.position);

        setVec3("material.ambient", mat.ambient);
        setVec3("material.diffuse", mat.diffuse);
        setVec3("material.specular", mat.specular);
        setFloat("material.shininess", mat.shininess);

    }
};
