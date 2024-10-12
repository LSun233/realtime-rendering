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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include"imgui/ImGuiFileDialog.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window,  Mesh& target, Camera& cam);

// settings


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
void init_imgui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    const char* glsl_version = "#version 130";
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
struct Gui_param
{
    Gui_param() {};
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = false;         //imgui demo 窗口控制
    bool wire_mode = false;                //线框模式
    bool SSA0 = false;
    bool hard_shadow = false;
    bool shadow_debug = false;
    bool back_ground = true;
    float fps = 0;                           //显示帧率
    float alpha = 1.0f;
    string filePath = "";                    //当前加载路径
};
void RenderMainImGui(Gui_param& gui_param)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (gui_param.show_demo_window)
        ImGui::ShowDemoWindow(&gui_param.show_demo_window);
    bool flag = true;
    if (gui_param.back_ground)
    {
        ImGui::Begin("Menu", &flag, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    }
    else
    {
        ImGui::Begin("Menu", &flag, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    }
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImVec2 Menue_size = ImVec2(screen_size.x / 5, screen_size.y / 3);
    ImVec2 Menue_pos = ImVec2(screen_size.x - Menue_size.x, 0);
    ImGui::SetWindowSize("Menu", Menue_size);
    ImGui::SetWindowPos("Menu", Menue_pos);
    //ImGui::Text("Use 'Left Alter' to focus on window");
    ImGui::Checkbox("Demo Window", &gui_param.show_demo_window);
    ImGui::SameLine();
    ImGui::Checkbox("WireMode", &gui_param.wire_mode);
    ImGui::SameLine();
    
    ImGui::SameLine();
    ImGui::Checkbox("background", &gui_param.back_ground);
    ImGui::SameLine();
    ImGui::Text("FPS: %f", gui_param.fps);
    //换行
    ImGui::ColorEdit4("clear color", (float*)&gui_param.clear_color); // Edit 3 floats representing a color
    ImGui::SliderFloat("alpha", &gui_param.alpha, 0.0, 1.0f);
    //换行
    ImGui::Checkbox("hard_shadow", &gui_param.hard_shadow);
    ImGui::SameLine();
    ImGui::Checkbox("shadow_debug", &gui_param.shadow_debug);
    ImGui::SameLine();

    //ImGui::DragFloat("alpha", &gui_param.alpha,0.01f,0.0,1.0f);

    if (ImGui::Button("Import model"))
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            gui_param.filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            while (gui_param.filePath.find('\\') != gui_param.filePath.npos)
            {
                gui_param.filePath = gui_param.filePath.replace(gui_param.filePath.find('\\'), 1, 1, '/');
            }
            // gui_param.filePath.replace("","");
        }

        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
        // -----
        processInput(window,mesh,camera);

        // render
        // ------
        if (gui_param.wire_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ImVec4 clear_color = gui_param.clear_color;
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

       

        bpShader.activate(mat, light);
        mesh.Draw(bpShader,mat, light, camera);


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

void processInput(GLFWwindow* window,Mesh& target,Camera& cam)
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
    std::cout << "up:" << io.MouseWheel<< std::endl;
    if (io.MouseWheel > 0)
    {
        std::cout << "up" << std::endl;
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
    glViewport(0, 0, width, height);
}



