#pragma once
#include"shader_m.h"
#include"../type_define.h"
#include"../camera.h"
class SimpleShader : public Shader {
public:
    SimpleShader(const char* vertexPath= "../data/shader/simple.vert", const char* fragmentPath= "../data/shader/simple.frag") :Shader(vertexPath, fragmentPath)
    {
        shader_type = ShaderType::Simple;

    }
};
