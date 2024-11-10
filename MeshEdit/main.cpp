#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp> 
#include "shader/shader_m.h"
#include"shader/BPShader.h"
#include"shader/SimpleShader.h"
#include "camera.h"
#include"mesh/mesh.h"
#include"type_define.h"
#include <iostream>
#include"UI/ui.h"
#include"mesh/primitive/AABB.h"
#include"mesh/primitive/trianglFace.h"
#include"BVH.h"
#include"mesh/primitive/line.h"
#include"UI/UIParam.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
GLFWwindow* creatGLFWwindow()
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); //����ݣ� 4�����������Ը�����Ҫ������
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);   //���ɸı��С  
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);   //û�б߿�ͱ����� 
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

    int monitorCount;
    GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
    const GLFWvidmode* mode = glfwGetVideoMode(pMonitor[0]);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyEngine", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // Enable vsync
    //some setting 
   // stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    return window;
}
int main()
{
    vector<MeshBase*> meshList;
    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
   
    UIParam* ui_param = UIParam::getInstance();

    //����ģ������
    string path = "../data/model/bunny1.ply";

    Mesh  mesh = Mesh(path);
    mesh.name = " bunny1";
   // mesh.OnCenter(camera.Position,camera.Front);
    //meshList.push_back(mesh);

    //BPShader bpShader;
    //SimpleShader simpleShader;

    //meshList.push_back(&mesh);
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        if (ui_param->wire_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (ui_param->filePath != "")
        {
            Mesh*  mesh1 =new  Mesh(ui_param->filePath);
            meshList.push_back(mesh1);
            ui_param->filePath = "";
        }

        ImVec4 clear_color = ImVec4(ui_param->clear_color[0], ui_param->clear_color[1], ui_param->clear_color[2], ui_param->clear_color[3]);
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        ui_param->fps = 1.0 / deltaTime;
        lastFrame = currentFrame;

        // render scene
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        for (int i = 0; i < meshList.size(); i++)
        {
            meshList[i]->Draw(camera);
        }
     
        //render hit triangles
        //glDisable(GL_DEPTH_TEST);
        //for (int i = 0; i < mesh.hitRes.size(); i++)
        //{
        //    TriangleFace  tri(mesh.hitRes[i]->p1, mesh.hitRes[i]->p2, mesh.hitRes[i]->p3);
        //    tri.model= mesh.GetModelMat();
        //    glDisable(GL_DEPTH_TEST);
        //    tri.Draw(camera);
        //}

        // render ui
        RenderMainImGui(meshList, camera);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}






