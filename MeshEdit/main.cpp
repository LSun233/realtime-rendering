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
#include"Utilis/tinyXML/tinyxml.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = ISCR_WIDTH / 2.0f;
float lastY = ISCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 帮助函数：分割字符串并返回浮点数向量 
std::vector<float> parseFloats(const std::string& text) {
    std::vector<float> values; std::istringstream iss(text); 
    float value;
    while (iss >> value) { 
        values.push_back(value);
    } 
    return values; }

// 帮助函数：分割字符串并返回整数向量 
std::vector<int> parseInts(const std::string& text) {
    std::vector<int> values; std::istringstream iss(text); 
    int value; while (iss >> value) { 
        values.push_back(value); 
    }
    return values; 
}
/// <summary>

/// </summary>
void XMLtest(string xmlpath)
{
    TiXmlDocument doc(xmlpath.data());
    bool loadOkay = doc.LoadFile();
    if (!loadOkay)
    {
        std::cout << "加载xml文件失败" << std::endl;
    }
    else
    {
        std::cout << "加载xml文件成功" << std::endl;
    }
    if (!doc.LoadFile()) { 
        std::cerr << "Failed to load file 'example.dae'" << std::endl; 
        return ; 
    } 
    TiXmlElement* root = doc.RootElement(); 
    if (root == nullptr) {
        std::cerr << "Failed to get root element" << std::endl; return ;
    }

    // 找到geometry元素
    TiXmlElement* geometry = root->FirstChildElement("library_geometries")->FirstChildElement("geometry"); 
    if (geometry == nullptr) { 
        std::cerr << "Failed to find geometry element" << std::endl; 
        return ;
    } 

    // 找到mesh元素并解析顶点数据 
    std::vector<Vertex> vertices;
    TiXmlElement* mesh = geometry->FirstChildElement("mesh");

    // 获取所有source元素 
    std::vector<TiXmlElement*> sources; 
    for (TiXmlElement* source = mesh->FirstChildElement("source"); source != nullptr; source = source->NextSiblingElement("source")) { 
        sources.push_back(source);
    }


    // 解析Position数据 
    std::vector<float> positionValues = parseFloats(sources[0]->FirstChildElement("float_array")->GetText());
    for (size_t i = 0; i < positionValues.size(); i += 3) { 
        Vertex vertex;
        vertex.Position = glm::vec3(positionValues[i], positionValues[i + 1], positionValues[i + 2]);
        vertices.push_back(vertex); 
    } 
    // 解析Normal数据 
    std::vector<float> normalValues = parseFloats(sources[1]->FirstChildElement("float_array")->GetText()); 
    // 解析TexCoords数据
     std::vector<float> texCoordValues = parseFloats(sources[2]->FirstChildElement("float_array")->GetText());
     // 解析polylist元素并匹配顶点、法线和纹理坐标
      TiXmlElement* polylist = mesh->FirstChildElement("polylist");
      TiXmlElement* p = polylist->FirstChildElement("p"); 
      std::vector<int> indices = parseInts(p->GetText()); 
      // 存储面片的顶点索引到vector<int>中
      std::vector<int> vertexIndices;


      // 获取每个面有多少个顶点 
      TiXmlElement* vcount = polylist->FirstChildElement("vcount"); 
      std::vector<int> vcounts = parseInts(vcount->GetText()); 
      size_t index = 0; for (size_t i = 0; i < vcounts.size(); ++i) { 
          for (int j = 0; j < vcounts[i]; ++j) { 
              int vertexIndex = indices[index * 3]; 
              int texCoordIndex = indices[index * 3 + 2]; 
              // 索引数组的结构通常是 顶点索引/纹理坐标索引/法线索引 
              int normalIndex = indices[index * 3 + 1]; 
              vertices[vertexIndex].TexCoords = glm::vec2(texCoordValues[texCoordIndex * 2], texCoordValues[texCoordIndex * 2 + 1]); 
              vertices[vertexIndex].Normal = glm::vec3(normalValues[normalIndex * 3], normalValues[normalIndex * 3 + 1], normalValues[normalIndex * 3 + 2]); ++index; 
              vertexIndices.push_back(vertexIndex);
          } 
      }
    // 解析骨骼权重数据
    // 解析library_controllers中的骨骼权重数据 
    TiXmlElement* controller = root->FirstChildElement("library_controllers")->FirstChildElement("controller"); 
    TiXmlElement* skin = controller->FirstChildElement("skin");
    TiXmlElement* vertexWeights = skin->FirstChildElement("vertex_weights");
    TiXmlElement* vcountElement = vertexWeights->FirstChildElement("vcount"); 
    TiXmlElement* vElement = vertexWeights->FirstChildElement("v"); 
    
    std::vector<int> vCounts = parseInts(vcountElement->GetText()); 
    std::vector<int> vValues = parseInts(vElement->GetText()); 
    // 解析权重数组 
    TiXmlElement* weightsSource = skin->FirstChildElement("source")->NextSiblingElement("source")->NextSiblingElement("source");
    std::vector<float> weights = parseFloats(weightsSource->FirstChildElement("float_array")->GetText()); 
    // 填充骨骼ID和权重 
    size_t vIndex = 0; for (size_t i = 0; i < vertices.size(); ++i) { 
        int influenceCount = vCounts[i];
        for (int j = 0; j < influenceCount; ++j) {
            int jointId = vValues[vIndex++]; 
            int weightIndex = vValues[vIndex++];
            if (j < MAX_BONE_INFLUENCE) {
                vertices[i].m_BoneIDs[j] = jointId;
                vertices[i].m_Weights[j] = weights[weightIndex]; 
            } 
        } 
    }
    
    //// 解析其他数据（Color, TexCoords等）的方法类似 // ... // 输出解析后的顶点数据 
    //for (const auto& vertex : vertices) 
    //{ 
    //    std::cout << "Position: " << vertex.Position.x << " " << vertex.Position.y << " " << vertex.Position.z << std::endl;
    //    std::cout << "Normal: " << vertex.Normal.x << " " << vertex.Normal.y << " " << vertex.Normal.z << std::endl;
    //    std::cout << "BoneID: " << vertex.m_BoneIDs[0] << "  " << vertex.m_BoneIDs[1] << "  " << vertex.m_BoneIDs[2] << "  " << vertex.m_BoneIDs[3] << "  " << std::endl;
    //    std::cout<<"BoneWeight: "<< vertex.m_Weights[0] << "  " << vertex.m_Weights[1] << "  " << vertex.m_Weights[2] << "  " << vertex.m_Weights[3] << "  " << std::endl;
    //}




   
    

}


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
    string xmlpath = "../data/model/Walking.dae";
    XMLtest(xmlpath);
    vector<MeshBase*> meshList;
    GLFWwindow* window = creatGLFWwindow();
    init_imgui(window);
    UIParam* ui_param = UIParam::getInstance();
    
    
    //读入模型数据
    string path = "../data/model/dragon.ply";
    Mesh*  pmesh =new  Mesh(path);
    BRDF* shaderBRDF_dragon = new BRDF(glm::vec3(0.5, 0.0, 0.0));
    pmesh->shader = shaderBRDF_dragon;
    pmesh->name = " dragon";
    camera.OnCenter(pmesh->aabb);
    meshList.push_back(pmesh);


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



