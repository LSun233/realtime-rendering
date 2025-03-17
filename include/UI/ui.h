#pragma once
#include"../type_define.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include"../imgui/ImGuiFileDialog.h"
#include"UIParam.h"
#include"../mesh/object.h"

void init_imgui(GLFWwindow* window);
void model_control(vector<Object*> meshList, Camera& cam);
void pick(vector<Object*> meshList, Camera& cam);
void light_control(MeshBase* lightMesh, Camera& cam);
void  camera_control(Camera& cam);
void interaction(vector<Object*>meshList, MeshBase* light, Camera& cam);
void RenderMainImGui(vector<Object*> meshList, Object* light, Camera& cam);



