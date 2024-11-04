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
#include"primitive/AABB.h"
#include"primitive/trianglFace.h"
#include"BVH.h"
#include"primitive/line.h"

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
    vector<Mesh> meshList;
    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
    Gui_param gui_param= Gui_param();
    //读入模型数据
    string path = "C:/Users/孙龙/Desktop/bunny1.ply";
    Mesh  mesh = Mesh(path);
    mesh.name = " bunny1";
    mesh.OnCenter(camera.Position,camera.Front);

  
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
   

    // build and compile our shader zprogram
    // ------------------------------------
    BPShader bpShader("../data/shader/blinn_phong.vert", "../data/shader/blinn_phong.frag");
    Shader lightCubeShader("../data/shader/light.vert", "../data/shader/light.frag");
    Shader simpleShader("../data/shader/simple.vert", "../data/shader/simple.frag");

    boudingBox aabb(mesh.aabb.max, mesh.aabb.min);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
     

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
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

     
        bpShader.activate(mat, light);
        mesh.Draw(bpShader,mat, light, camera);
       // mesh.DrawBVH(simpleShader, camera);

        //aabb.Model = mesh.GetModelMat();
        //aabb.Draw(simpleShader, camera);



        ////render hit triangles
        glDisable(GL_DEPTH_TEST);
      
        for (int i = 0; i < mesh.hitRes.size(); i++)
        {
            
            simpleShader.use();
            simpleShader.setVec4("color", 1.0, 1.0, 1.0, 1.0);
            TriangleFace  tri(mesh.hitRes[i]->p1, mesh.hitRes[i]->p2, mesh.hitRes[i]->p3);
            tri.Model= mesh.GetModelMat();
            tri.Draw(simpleShader, camera);
        }

        std::cout <<"size():"<< mesh.suroundingFace.size() << std::endl;
        for (int i = 0; i < mesh.suroundingFace.size(); i++)
        {
            int tri_index = mesh.suroundingFace[i];
            int i1 = mesh.indices[3 * tri_index];
            int i2 = mesh.indices[3 * tri_index + 1];
            int i3 = mesh.indices[3 * tri_index + 2];

            glm::vec3 v1 = mesh.vertices[i1].Position;
            glm::vec3 v2 = mesh.vertices[i2].Position;
            glm::vec3 v3 = mesh.vertices[i3].Position;

            simpleShader.use();
            simpleShader.setVec4("color",1.0,1.0,0,1.0);
            TriangleFace  tri(v1, v2, v3);
            tri.Model = mesh.GetModelMat();
            tri.Draw(simpleShader, camera);

        }
        //std::cout << mesh.debug_line.size() << std::endl;
        //for (int i = 0; i < mesh.debug_line.size(); i++)
        //{
        //    mesh.debug_line[i].Model= mesh.GetModelMat();
        //    mesh.debug_line[i].Draw(simpleShader, camera);
        //}

        // render ui
        RenderMainImGui(gui_param, mesh, camera);

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






