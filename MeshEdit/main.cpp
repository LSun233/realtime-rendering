#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/vec1.hpp> 

#include "shader_m.h"
#include"BPShader.h"
#include "camera.h"
#include"mesh.h"
#include"type_define.h"
#include <iostream>

#include"ui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(Mesh& target, Camera& cam);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
    Gui_param gui_param= Gui_param();
    //读入模型数据
    string path = "C:/Users/孙龙/Desktop/bunny1.ply";
    Mesh  mesh = Mesh(path);

    //设置模型材质
    BPMaterial mat = { 
        glm::vec3(1.0f, 0.5f, 0.31f),   //ambient
        glm::vec3(1.0f, 0.5f, 0.31f),   //diffuse
        glm::vec3(0.5f, 0.5f, 0.5f),    //specular
        32.0f };                         //shininess


    BPLight light = {
        glm::vec3(2.0,0.7,1.3),                 //postion
        glm::vec3(2.0,0.7,1.3) * glm::vec3(0.1f),//ambient
        glm::vec3(2.0,0.7,1.3) * glm::vec3(0.5f),//diffuse
        glm::vec3(1.0,1.0,1.0)                     //specular

    };

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    BPShader bpShader("../data/shader/blinn_phong.vert", "../data/shader/blinn_phong.frag");
    Shader lightCubeShader("../data/shader/light.vert", "../data/shader/light.frag");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(mesh,camera);

        // render
        // ------
        if (gui_param.wire_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ImVec4 clear_color = ImVec4(gui_param.clear_color[0], gui_param.clear_color[1], gui_param.clear_color[2], gui_param.clear_color[3]);
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

       

        bpShader.activate(mat, light);
        mesh.Draw(bpShader,mat, light, camera);

        // render ui
        RenderMainImGui(gui_param);

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------


void processInput(Mesh& target,Camera& cam)
{
    //if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //    glfwSetWindowShouldClose(window, true);
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    camera.ProcessKeyboard(FORWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.ProcessKeyboard(BACKWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera.ProcessKeyboard(LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera.ProcessKeyboard(RIGHT, deltaTime);

    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImVec2 Menue_size = ImVec2(screen_size.x / 5, screen_size.y / 3);
    ImVec2 Menue_pos = ImVec2(screen_size.x - Menue_size.x, 0);
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseDown[0])
    {
        bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
            io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
        if (!on_menue)
        {
            cam.ProcessMouseMovement(io.MouseDelta.x, io.MouseDelta.y);
        }
    }
    if (io.MouseDown[1])
    {
        bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
            io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
        if (!on_menue)
        {
            glm::vec3 newPos = target.Position;
            newPos.x = newPos.x + 0.01f*io.MouseDelta.x;
            newPos.y = newPos.y + 0.01f * io.MouseDelta.y;
            target.SetPostion(newPos);
        }
    }
    
    if (io.MouseWheel > 0)
    {
        //靠近相机
        glm::vec3 newPos = target.Position;
        glm::vec3 camPos = cam.Position;
        glm::vec3 direct = glm::normalize(camPos - newPos);
        float scale = 0.5f;
        newPos = newPos + scale*direct;
        target.SetPostion(newPos);
    }
    if (io.MouseWheel < 0)
    {
        //远离相机
        glm::vec3 newPos = target.Position;
        glm::vec3 camPos = cam.Position;
        glm::vec3 direct = glm::normalize(camPos - newPos);
        float scale = 0.5f;
        newPos = newPos - scale * direct;
        target.SetPostion(newPos);
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}



