#pragma once
#include"type_define.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include"imgui/ImGuiFileDialog.h"
#include"mesh/primitive/line.h"
#include"UIParam.h"


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
ImVec2 dragStartPos;
bool isDragging = false;
void model_control(vector<MeshBase*> meshList, Camera& cam)
{
    if (meshList.size() == 0)return;
    UIParam* ui_param = UIParam::getInstance();
    MeshBase* target = meshList[ui_param->target_index];
  
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImVec2 Menue_size = ImVec2(screen_size.x / 5, screen_size.y / 3);
    ImVec2 Menue_pos = ImVec2(screen_size.x - Menue_size.x, 0);
    ImGuiIO& io = ImGui::GetIO();

    if (io.MouseDown[0])
    {
        if (io.MousePos.y > 0)
        {
            bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
                io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
            if (!on_menue)
            {
                float angle = io.MouseDelta.y * glm::radians(0.1f); // 
                glm::vec4 axis = glm::inverse(target->GetModelMat()) * glm::vec4(cam.Right, 0);
                target->rotation(glm::vec3(axis.x, axis.y, axis.z), angle);
            }

        }
        if (io.MousePos.x > 0)
        {
            bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
                io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
            if (!on_menue)
            {
                float angle = io.MouseDelta.x * glm::radians(0.1f); // 
                glm::vec4 axis = glm::inverse(target->GetModelMat()) * glm::vec4(cam.Up, 0);
                target->rotation(glm::vec3(axis.x, axis.y, axis.z), angle);
            }

        }
    }
    if (io.MouseDown[1])
    {
        bool on_menue = io.MousePos.x > Menue_pos.x && io.MousePos.x <(Menue_pos.x + Menue_size.x) &&
            io.MousePos.y> Menue_pos.y && io.MousePos.y < (Menue_pos.y + Menue_size.y);
        if (!on_menue)
        {
            glm::vec3 trans = 0.0001f * io.MouseDelta.x * cam.Right - 0.0001f * io.MouseDelta.y * cam.Up;
            target->translate(trans);
        }
    }

    float wheel = ImGui::GetIO().MouseWheel;
    float distance_scale = 0.1f;
    if (io.MouseWheel > 0)
    {
        //�������
        glm::vec3 trans = distance_scale * cam.Front;
        target->translate(trans);
    }
    if (io.MouseWheel < 0)
    {
        //Զ�����
        glm::vec3 trans = -1 * distance_scale * cam.Front;
        target->translate(trans);
    }
}
void pick(vector<MeshBase*> meshList, Camera& cam)
{
    UIParam* ui_param = UIParam::getInstance();
    ImGuiIO& io = ImGui::GetIO();
    if (io.MouseClicked[0] != 0)
    {
        if (ui_param->pick)
        {
            // ����Ļ��������ת��Ϊ��һ���豸����
            float ndcX = (io.MouseClickedPos[0].x - 0.5 * ISCR_WIDTH) / (0.5 * ISCR_WIDTH);
            float ndcY = -(io.MouseClickedPos[0].y - 0.5 * ISCR_HEIGHT) / (0.5 * ISCR_HEIGHT);
            float h = tan(glm::radians(cam.Zoom * 0.5f));
            float w = (h * ISCR_WIDTH) / ISCR_HEIGHT;
            glm::vec3 viewSpace = glm::vec3(ndcX * w, ndcY * h, -1);
            glm::vec3 rep = cam.GetPerspectiveMatrix() * glm::vec4(viewSpace, 1.0);
            glm::vec3 worldSpace = glm::inverse(cam.GetViewMatrix()) * glm::vec4(viewSpace, 1.0);


            //����mesh
            float min_t = 99999;
            int target_index = -1;
            for (int i = 0; i < meshList.size(); i++)
            {
              
                glm::vec3 modelSpace = glm::inverse(meshList[i]->GetModelMat()) * glm::vec4(worldSpace, 1.0);
                glm::vec4 reproject = cam.GetPerspectiveMatrix() * cam.GetViewMatrix() * meshList[i]->GetModelMat() * glm::vec4(modelSpace, 1.0);
                glm::vec3 camPos = glm::inverse(meshList[i]->GetModelMat()) * glm::vec4(cam.Position, 1.0);
                glm::vec3 rayDirection = glm::normalize(modelSpace - camPos);
                Ray ray = Ray(camPos, rayDirection);
                float t = meshList[i]->hit(ray);
                if (t > 0 && t < min_t)
                {
                    min_t = t;
                    target_index = i;
                }
            }
            if (target_index != -1)
            {
                ui_param->target_index = target_index;

                
                //��ģ�Ϳռ���
                MeshBase* target = meshList[ui_param->target_index];
                glm::vec3 modelSpace = glm::inverse(target->GetModelMat()) * glm::vec4(worldSpace, 1.0);
                glm::vec4 reproject = cam.GetPerspectiveMatrix() * cam.GetViewMatrix() * target->GetModelMat() * glm::vec4(modelSpace, 1.0);
                glm::vec3 camPos = glm::inverse(target->GetModelMat()) * glm::vec4(cam.Position, 1.0);
                glm::vec3 rayDirection = glm::normalize(modelSpace - camPos);
                Ray ray = Ray(camPos, rayDirection);
                HitResult res = target->bvh->hit(ray);
                if (res.triangle == NULL)
                {
                    return;
                    std::cout << "û�н���" << std::endl;
                }


                target->hitRes.clear();
                target->hitRes.push_back(res.triangle);

                //�ҵ����ڵ�������
                std::cout << "hit face index:" << res.triangle->index << std::endl;
                int target_face = res.triangle->index;

                target->suroundingFace.clear();
                target->halfEdge->findSurroundingFaces(target_face, target->suroundingFace);
            }
        }
    }
}
void  camera_control(Camera& cam)
{
    UIParam* ui_param = UIParam::getInstance();
   
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImVec2 Menue_size = ImVec2(screen_size.x / 5, screen_size.y / 3);
    ImVec2 Menue_pos = ImVec2(screen_size.x - Menue_size.x, 0);
    ImGuiIO& io = ImGui::GetIO();

    if (io.MouseDown[0])
    {

        cam.ProcessMouseMovement(io.MouseDelta.x, io.MouseDelta.y);
    }
    float wheel = ImGui::GetIO().MouseWheel;
    float distance_scale = 0.1f;
    if (io.MouseWheel!=0 )
    {
        cam.ProcessMouseScroll(io.MouseWheel);   
    }

}
void interaction(vector<MeshBase*> meshList, Camera& cam)
{
    UIParam* ui_param = UIParam::getInstance();
    if (ui_param->control_type == 0)
    {
        model_control(meshList, cam);
    }
    else
    {
        camera_control(cam);
    }
   
    pick(meshList, cam);
}
void RenderMainImGui(vector<MeshBase*> meshList,Camera& cam )
{
    UIParam* ui_param = UIParam::getInstance();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (ui_param->show_demo_window)
        ImGui::ShowDemoWindow(&ui_param->show_demo_window);
    bool flag = true;
    if (ui_param->back_ground)
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
    ImGui::Checkbox("Demo Window", &ui_param->show_demo_window);
    ImGui::SameLine();
    ImGui::Checkbox("WireMode", &ui_param->wire_mode);
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::Checkbox("background", &ui_param->back_ground);
    ImGui::SameLine();
    ImGui::Text("FPS: %f", ui_param->fps);
    //����
    //ImVec4 clear_color;
    ImGui::ColorEdit4("clear color", (float*)&ui_param->clear_color); // Edit 3 floats representing a color
   // std::cout << clear_color.x << " " << clear_color.y << " " << clear_color.z << " " << clear_color.w << std::endl;

   
    ImGui::SliderFloat("alpha", &ui_param->alpha, 0.0, 1.0f);
    //���� 
    ImGui::Checkbox("shadow", &ui_param->hard_shadow);
    ImGui::SameLine();
    ImGui::Checkbox("SSAO", &ui_param->SSA0);
    if (ui_param->SSA0)
    {

        ImGui::SliderFloat("radius", &ui_param->radius, 0.0f, 5.0f);
        ImGui::SliderFloat("bias", &ui_param->bias, 0.0f, 2.0f);
    }
    ImGui::SameLine();
    ImGui::Checkbox("pick", &ui_param->pick);

    ImGui::RadioButton("blinn phong", &ui_param->shader_type, 0); ImGui::SameLine();
    ImGui::RadioButton("BRDF", &ui_param->shader_type, 1); ImGui::SameLine();
    ImGui::RadioButton("ray tracing", &ui_param->shader_type, 2);

    if (ui_param->shader_type == 1)
    {
        ImGui::SliderFloat("metallic", &ui_param->metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("roughness", &ui_param->roughness, 0.0f, 1.0f);
        ImGui::ColorEdit4("albedo", (float*)&ui_param->albedo);
    }





    ImGui::RadioButton("model control", &ui_param->control_type, 0); ImGui::SameLine();
    ImGui::RadioButton("camera control", &ui_param->control_type, 1); ImGui::SameLine();

   
    if (ImGui::Button("Import model"))
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ply",  ".");
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            ui_param->filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            while (ui_param->filePath.find('\\') != ui_param->filePath.npos)
            {
                ui_param->filePath = ui_param->filePath.replace(ui_param->filePath.find('\\'), 1, 1, '/');
            }
             
        }
        ImGuiFileDialog::Instance()->Close();
    }
    if (ImGui::CollapsingHeader("scene list", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // �ڵ�����...
        ImGui::Text("model");
        if (ImGui::TreeNode("Child 1"))
        {
            // �ӽڵ�����...
            ImGui::Text("Child 1 contents");
            ImGui::TreePop(); // �����ӽڵ�
        }

        ImGui::TreePop(); // �����ڵ�
    }


    //UI����
    interaction(meshList, cam);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



