#pragma once
#include <vector>

#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif

class Camera;
class Object;
class Shadow;
struct GLFWwindow;

class EXPORTEDCLASSES_API RenderApp
{
public:

	Camera* camera;
	std::vector<Object*> objectList;
	Shadow* shadow;
	Object* light;
public:

	RenderApp();
	void init(void (*start)(RenderApp*));
	void run(void(*updata)(RenderApp*));
	~RenderApp();
private:

	GLFWwindow* window;
	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;

	float lastX;
	float lastY;
	bool firstMouse;


private:
	void renderQuad();
	GLFWwindow* creatGLFWwindow();
};







 
