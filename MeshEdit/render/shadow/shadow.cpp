#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"shadow.h"
#include"UI/UIParam.h"
#include<iostream>
#include"type_define.h"
#include"shader/shader_m.h"
#include"mesh/mesh.h"
#include"mesh/Object.h"
#include"Camera.h"
Shadow::Shadow()
{
    // configure depth map FBO
  // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    simpleDepthShader = new Shader("../data/shader/point_shadows_soft_depth.vert", "../data/shader/point_shadows_soft_depth.frag", "../data/shader/point_shadows_soft_depth.geom");
}

void Shadow::render(vector<Object*> objectList, Camera& cam, glm::vec3 lightPos)
{
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    simpleDepthShader->use();
    for (unsigned int i = 0; i < 6; ++i)
    {
        simpleDepthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }

    simpleDepthShader->setFloat("far_plane", far_plane);
    simpleDepthShader->setVec3("lightPos", lightPos);



    //设置每个object的每个mesh的shadowshader
    for (int i = 0; i < objectList.size(); i++)
    {

        Object* OP = objectList[i];
        for (int j = 0; j < OP->m_meshes.size(); j++)
        {

            OP->m_meshes[j]->shadowShader = simpleDepthShader;

        }
    }

    //依此渲染每个物体
    for (int i = 0; i < objectList.size(); i++)
    {
        Object* OP = objectList[i];
        OP->ShadowDraw(cam);
    }

    //将深度图传入每个object的每个mesh的渲染shader中，并设置相关参数
    UIParam* ui_param = UIParam::getInstance();
    for (int i = 0; i < objectList.size(); i++)
    {

        Object* OP = objectList[i];
        for (int j = 0; j < OP->m_meshes.size(); j++)
        {


            OP->m_meshes[j]->shader->use();
            OP->m_meshes[j]->shader->setMat4("view", cam.GetViewMatrix());
            OP->m_meshes[j]->shader->setBool("shadowsFlag", ui_param->shadow);
            OP->m_meshes[j]->shader->setFloat("far_plane", far_plane);
            OP->m_meshes[j]->shader->setFloat("near_plane", near_plane);
            OP->m_meshes[j]->shader->SetTexture3D("depthMap", depthCubemap);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ISCR_WIDTH, ISCR_HEIGHT);
}




Shadow::~Shadow()
{

}
