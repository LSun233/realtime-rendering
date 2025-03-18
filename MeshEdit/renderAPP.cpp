#include"renderAPP.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp> 
#include "camera.h"
#include"type_define.h"
#include"UI/ui.h"
#include"render/shadow/shadow.h"
#include"mesh/object.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include"mesh/primitive/cube.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    ISCR_WIDTH = width;
    ISCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

GLFWwindow* RenderApp::creatGLFWwindow()
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); //抗锯齿， 4个样本（可以根据需要调整）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);   //不可改变大小  
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);   //没有边框和标题栏 
    //glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

    int monitorCount;
    GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
    const GLFWvidmode* mode = glfwGetVideoMode(pMonitor[0]);
    GLFWwindow* window = glfwCreateWindow(ISCR_WIDTH, ISCR_HEIGHT, "MyEngine", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // Enable vsync
    //some setting 
   // stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    return window;
}
RenderApp::RenderApp()
{
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    window = creatGLFWwindow();
    init_imgui(window);
    float lastX = ISCR_WIDTH / 2.0f;
    float lastY = ISCR_HEIGHT / 2.0f;
    bool firstMouse = true;
}





void RenderApp::init(void(*start)(RenderApp*))
{
    glm::vec3 lightColor(1.0f, 1.0f, 0.9f);
    light = new CUBE(glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.1, 0.3, 0.0));
    start(this);
    if (objectList.size() > 0&& objectList[0]->m_meshes.size()>0)
    {
        camera->OnCenter(objectList[0]->m_meshes[0]->aabb);
    }
    
    shadow = new Shadow();

}


void RenderApp::run(void(*updata)(RenderApp*))
{
    UIParam* ui_param = UIParam::getInstance();
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        if (ui_param->wire_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        ui_param->deltaTime = currentFrame - ui_param->lastFrame;
        ui_param->fps = 1.0 / ui_param->deltaTime;
        ui_param->lastFrame = currentFrame;

        // render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ImVec4 clear_color = ImVec4(ui_param->clear_color[0], ui_param->clear_color[1], ui_param->clear_color[2], ui_param->clear_color[3]);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

        //具体计算
        updata(this);

        //draw scene
        shadow->render (objectList, *camera, light->GetPostion());
        for (int i = 0; i < objectList.size(); i++)
        {
            Object* OP = objectList[i];
            for (int j = 0; j < OP->m_meshes.size(); j++)
            {
                OP->m_meshes[j]->shader->use();
                OP->m_meshes[j]->shader->setMaterial();
                OP->m_meshes[j]->shader->setLight(camera->GetViewMatrix(), light->GetPostion(), glm::vec3(1.0, 1.0, 1.0));
            }
            OP->Draw(*camera);
        }

        //draw light
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);

        // render ui
        RenderMainImGui(objectList, light, *camera);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
RenderApp::~RenderApp()
{
}
void RenderApp::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}