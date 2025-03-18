#pragma once
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Object;
class Camera;
class Shader;
class EXPORTEDCLASSES_API Shadow
{
public:
	Shadow();
    void render(std::vector<Object*>objectList, Camera& cam, glm::vec3 lightPos);
	~Shadow();
    unsigned int depthMapFBO;
    unsigned int depthCubemap;
    Shader* simpleDepthShader;

private:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
   
    float near_plane = 0.001f;
    float far_plane = 100.0f;
};

