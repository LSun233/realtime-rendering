#pragma once
#include"type_define.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include"imgui/ImGuiFileDialog.h"

// Helper to wire demo markers located in code to an interactive browser

// Helper to wire demo markers located in code to an interactive browser


void init_imgui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 

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
void interaction(Mesh& target, Camera& cam)
{
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImVec2 Menue_size = ImVec2(screen_size.x / 5, screen_size.y / 3);
    ImVec2 Menue_pos = ImVec2(screen_size.x - Menue_size.x, 0);
    ImGuiIO& io = ImGui::GetIO();

    if (io.MouseDown[0])
    {
        if (io.MousePos.y >0  )
        {
          
            float angle = io.MouseDelta.y*glm::radians(0.1f); // 
            glm::vec4 axis = glm::inverse(target.GetModelMat()) * glm::vec4(cam.Right, 0);
            target.rotation( glm::vec3(axis.x, axis.y, axis.z), angle);
        }
        if (io.MousePos.x > 0)
        { 
            float angle = io.MouseDelta.x * glm::radians(0.1f); // 
            glm::vec4 axis = glm::inverse(target.GetModelMat()) * glm::vec4(cam.Up, 0);
            target.rotation(glm::vec3(axis.x, axis.y, axis.z), angle);
        }
    }
    if (io.MouseDown[1])
    {
        bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
            io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
        if (!on_menue)
        {
            glm::vec3 trans = 0.0001f * io.MouseDelta.x * cam.Right - 0.0001f * io.MouseDelta.y * cam.Up;
            target.translate(trans);
        }
    }

    float wheel = ImGui::GetIO().MouseWheel;
  
    float distance_scale = 0.1f;
    if (io.MouseWheel > 0)
    {
        //靠近相机
        glm::vec3 trans = distance_scale * cam.Front;
        target.translate(trans);
    }
    if (io.MouseWheel < 0)
    {
        //远离相机
        glm::vec3 trans =-1*distance_scale * cam.Front;
        target.translate(trans);
    }
}
void RenderMainImGui(Gui_param& gui_param, Mesh& target, Camera& cam)
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
    ImVec4 clear_color;
    ImGui::ColorEdit4("clear color", (float*)&gui_param.clear_color); // Edit 3 floats representing a color
   // std::cout << clear_color.x << " " << clear_color.y << " " << clear_color.z << " " << clear_color.w << std::endl;

   
    ImGui::SliderFloat("alpha", &gui_param.alpha, 0.0, 1.0f);
    //换行
    ImGui::Checkbox("hard_shadow", &gui_param.hard_shadow);
    ImGui::SameLine();
    ImGui::Checkbox("shadow_debug", &gui_param.shadow_debug);
    ImGui::SameLine();
   
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
    if (ImGui::CollapsingHeader("scene list", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // 节点内容...
        ImGui::Text("model");
        if (ImGui::TreeNode("Child 1"))
        {
            // 子节点内容...
            ImGui::Text("Child 1 contents");
            ImGui::TreePop(); // 结束子节点
        }

        ImGui::TreePop(); // 结束节点
    }

    //UI交互
    interaction(target, cam);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



