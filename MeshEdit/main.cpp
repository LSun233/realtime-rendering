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
#include"mesh/primitive/CUBE.h"
#include"mesh/primitive/plane.h"
#include"UI/UIParam.h"
#include"render/skybox/skybox.h"
#include"shader/BRDF.h";
#include"shader//BRDFSSAO.h"
#include"render/shadow/shadow.h"
#include"render/GI/SSAO.h"
#include"Animation/Bone.h"
#include"Animation/animation.h"
#include"Animation/animator.h"
#include"FileIO/daeRead.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = ISCR_WIDTH / 2.0f;
float lastY = ISCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


/// <summary>

/// </summary>



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

    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
    UIParam* ui_param = UIParam::getInstance();


    string xmlpath = "../data/model/Walking.dae";

    //std::vector<Vertex> vertices;
    //std::vector<unsigned int> vertexIndices;

    vector<Mesh*> meshes;
    vector<Animation*>animations;

    daeRead(xmlpath, meshes, animations);






    Animator animator1(animations[0]);
    Animator animator2(animations[1]);


    vector<MeshBase*> meshList;

    
    
    //读入模型数据

    Mesh* pmesh1 = meshes[0];
    Mesh* pmesh2 = meshes[1];




    //Mesh*  pmesh =new  Mesh(path);
    BRDF* shaderBRDF_dragon = new BRDF(glm::vec3(0.5, 0.0, 0.0));
    pmesh1->shader = shaderBRDF_dragon;
    pmesh1->name = " dragon";
    camera.OnCenter(pmesh1->aabb);
    meshList.push_back(pmesh1);

    BRDF* shaderBRDF_dragon1 = new BRDF(glm::vec3(0.5, 0.2, 0.3));
    pmesh2->shader = shaderBRDF_dragon1;
    pmesh2->name = " dragon";
    camera.OnCenter(pmesh2->aabb);
    meshList.push_back(pmesh2);






    plane* floor =new  plane(glm::vec3(10.0, 10.0, 10.0));
    floor->name = "floor";
    BRDF* shaderBRDF_floor = new BRDF(glm::vec3(1.0, 1.0, 1.0));
    floor->shader = shaderBRDF_floor;
    meshList.push_back(floor);

    plane* wall = new  plane(glm::vec3(10.0, 10.0, 10.0));
    floor->name = "wall";
    BRDF* shaderBRDF_wall = new BRDF(glm::vec3(1.0, 1.0, 1.0));
    wall->shader = shaderBRDF_wall;
    //meshList.push_back(wall);


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
    
   
    BRDFSSAO* shaderBRDFSSA0=new BRDFSSAO();
    SSAO ssao = SSAO();
    SimpleShader* lightshader = new SimpleShader();
 
    //初始化光照
    // lighting
  
    glm::vec3 lightColor(1.0f, 1.0f, 0.9f);
    CUBE lightcube = CUBE(glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.1, 0.3, 0.0));

    //阴影
    Shadow  shadow= Shadow();

 



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

        animator1.UpdateAnimation(deltaTime);
        animator2.UpdateAnimation(deltaTime);

        // render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ImVec4 clear_color = ImVec4(ui_param->clear_color[0], ui_param->clear_color[1], ui_param->clear_color[2], ui_param->clear_color[3]);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
       


 
        //draw scene
        if (ui_param->SSA0)
        {
                shaderBRDFSSA0->metallic = ui_param->metallic;
                shaderBRDFSSA0->roughness = ui_param->roughness;
                shaderBRDFSSA0->use();
                shaderBRDFSSA0->setMaterial();
                shaderBRDFSSA0->setLight(camera.GetViewMatrix(), lightcube.GetPostion(), lightColor);
                vector<Shader*>MeshShader(meshList.size());
                for (int i = 0; i < meshList.size(); i++)
                {
                    MeshShader[i] = meshList[i]->shader;
                    meshList[i]->shader = shaderBRDFSSA0;
                }
                shadow.render(meshList, camera, lightcube.GetPostion());
                for (int i = 0; i < meshList.size(); i++)
                {
                    meshList[i]->shader = MeshShader[i];
                }
                ssao.render(meshList, camera, shaderBRDFSSA0);
        }
        else
        {
            shadow.render(meshList, camera, lightcube.GetPostion());
            for (int i = 0; i < meshList.size(); i++)
            {
                    meshList[i]->shader->use();
                    meshList[i]->shader->metallic = ui_param->metallic;
                    meshList[i]->shader->roughness = ui_param->roughness;
                    meshList[i]->shader->setMaterial();
                    meshList[i]->shader->setLight(camera.GetViewMatrix(), lightcube.GetPostion(), lightColor);
                    meshList[i]->Draw(camera);
            }

            //传入骨骼变换矩阵
            auto transforms1 = animator1.GetFinalBoneMatrices();
            meshList[0]->shader->use();
            for (int i = 0; i < transforms1.size(); ++i)
            {
                    meshList[0]->shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms1[i]);
            
            }

            auto transforms2 = animator2.GetFinalBoneMatrices();
            meshList[1]->shader->use();
            for (int i = 0; i < transforms2.size(); ++i)
            {
                meshList[1]->shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms2[i]);

            }

        }

        //draw light
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);

        skybox.Draw(&camera);

        lightshader->use();
        lightshader->setVec4("color", glm::vec4(lightColor,1.0));

        lightcube.shader = lightshader;
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



