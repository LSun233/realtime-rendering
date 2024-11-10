#pragma once
#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"
class SimpleShader : public Shader {
public:
    BPMaterial mat;
    BPLight light;
    SimpleShader(const char* vertexPath= "../data/shader/simple.vert", const char* fragmentPath= "../data/shader/simple.frag") :Shader(vertexPath, fragmentPath)
    {
        shader_type = ShaderType::Simple;
    }
    void activate() override {
        use();
       setVec4("color", 1.0, 1.0, 1.0, 1.0);
    }
};
