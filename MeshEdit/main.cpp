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
#include"UI/ui.h"
#include"mesh/primitive/AABB.h"
#include"mesh/primitive/trianglFace.h"
#include"BVH.h"
#include"mesh/primitive/CUBE.h"
#include"UI/UIParam.h"
#include"render/skybox/skybox.h"
#include"shader/BRDF.h";
#include"shader//BRDFSSAO.h"
#include"render/GI/SSAO.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = ISCR_WIDTH / 2.0f;
float lastY = ISCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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





int main()
{
    vector<MeshBase*> meshList;
    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
    UIParam* ui_param = UIParam::getInstance();
    //读入模型数据
    string path = "../data/model/dragon.ply";
    Mesh*  pmesh =new  Mesh(path);
    pmesh->name = " bunny1";
    camera.OnCenter(pmesh->aabb);
    meshList.push_back(pmesh);
    //meshList.push_back(&mesh);

    std::vector<std::string> faces
    {
     "../data/skybox/sky/right.jpg",
     "../data/skybox/sky/left.jpg",
     "../data/skybox/sky/top.jpg",
      "../data/skybox/sky/bottom.jpg",
      "../data/skybox/sky/front.jpg",
     "../data/skybox/sky/back.jpg"
    };
    SkyBox skybox(faces);
    BRDF* shaderBRDF = new BRDF();
    BPShader* shaderBP = new BPShader();
    BRDFSSAO* shaderBRDFSSA0=new BRDFSSAO();
    SSAO ssao = SSAO(shaderBRDFSSA0);
    SimpleShader* simpleshader = new SimpleShader();
    //初始化光照
    // lighting
    //glm::vec3 lightPos(2.0, 0.7, 1.3);
    glm::vec3 lightColor(300.0f, 300.0f, 300.0f);
    CUBE lightcube = CUBE(glm::vec3(0.02, 0.02, 0.02), glm::vec3(0.1, 0.3, 0.0));
  




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

       
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        ui_param->fps = 1.0 / deltaTime;
        lastFrame = currentFrame;

        // render scene
     
     
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ImVec4 clear_color = ImVec4(ui_param->clear_color[0], ui_param->clear_color[1], ui_param->clear_color[2], ui_param->clear_color[3]);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
       
        
        std::cout << lightcube.GetPostion().x << "   " << lightcube.GetPostion().y << "   " << lightcube.GetPostion().z << std::endl;

        //dram scene
        for (int i = 0; i < meshList.size(); i++)
        {
            if (ui_param->shader_type ==1)
            {
                if (ui_param->SSA0)
                {
                    shaderBRDFSSA0->metallic = ui_param->metallic;
                    shaderBRDFSSA0->roughness = ui_param->roughness;
                    shaderBRDFSSA0->albedo = glm::vec3(ui_param->albedo[0], ui_param->albedo[1], ui_param->albedo[2]);
                    shaderBRDFSSA0->setMaterial();
                    shaderBRDFSSA0->setLight(camera.GetViewMatrix(), lightcube.GetPostion(), lightColor);
                    ssao.render(meshList,camera);
                }
                else
                {
                    shaderBRDF->metallic = ui_param->metallic;
                    shaderBRDF->roughness = ui_param->roughness;
                    shaderBRDF->albedo = glm::vec3(ui_param->albedo[0], ui_param->albedo[1], ui_param->albedo[2]);
                    shaderBRDF->setMaterial();
                    shaderBRDF->setLight(camera.GetViewMatrix(), lightcube.GetPostion(), lightColor);
                    meshList[i]->shader = shaderBRDF;
                    meshList[i]->Draw(camera);
                }
                
            }
            else if(ui_param->shader_type ==0)
            {
                shaderBP->setMaterial();
                shaderBP->setLight(camera.GetViewMatrix(), lightcube.GetPostion(), lightColor);
                meshList[i]->shader = shaderBP;
                meshList[i]->Draw(camera);
            }
        }


        //dram light
        glEnable(GL_MULTISAMPLE);
 
        glEnable(GL_DEPTH_TEST);
    
        skybox.Draw(&camera);
        simpleshader->use();
        simpleshader->setVec4("color",glm::vec4(1,1,1,1));
        lightcube.shader = simpleshader;

        lightcube.Draw(camera);
        // render ui
        RenderMainImGui(meshList,&lightcube, camera);

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

    ISCR_WIDTH = width;
    ISCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}





unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
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



