#pragma once
#include"../../shader/shader_m.h"
#include <random>
#include"../../mesh/mesh.h"

class SSAO 
{
public:
    SSAO()
    {
        shaderSSAO = new Shader("../data/shader/ssao.vert", "../data/shader/ssao.frag");
        shaderSSAOBlur = new Shader("../data/shader/ssao.vert", "../data/shader/ssao_blur.frag");
        shaderGeometryPass = new Shader("../data/shader/ssao_geometry.vert", "../data/shader/ssao_geometry.frag");
        
        shaderframebuffer = new Shader("../data/shader/framebufferscreen.vert", "../data/shader/framebufferscreen.frag");
        


        CreatGFramebuffer();

        CreatSSAOFrameBuffer();

        CreatSimpleKernel();

        CreatNoiseTexture();

        ShaderConfiguration();

        CreatQuad();


    }


    ~SSAO()
    {
        delete shaderSSAO;
        delete shaderSSAOBlur;
        delete shaderGeometryPass;
        delete shaderLighting;
        delete shaderframebuffer;


        //unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
        //unsigned int ssaoFBO, ssaoBlurFBO, noiseTexture;
        
    
    }

    void render(std::vector<MeshBase*>meshList, Camera& cam, Shader* pRenderShader)
    {
        GeometryPass(meshList, cam);
        SSAOTexturePass(cam);
        blurSSAOTexturePass();
        renderLight(cam, pRenderShader);
    }


    void renderLight(Camera& cam,Shader* pRenderShader)
    {
        pRenderShader->use();
        pRenderShader->SetTexture2D("gPosition", gPosition);
        pRenderShader->SetTexture2D("gNormal", gNormal);
        pRenderShader->SetTexture2D("gAlbedo", gAlbedo);
        pRenderShader->SetTexture2D("ssao", ssaoColorBufferBlur);
   
        glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        renderQuad();
        glEnable(GL_DEPTH_TEST);
    }





private:
    void CreatQuad()
    {
        float quadVertices[] = {
            // positions   // texCoords
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
    void renderQuad()
    {
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    }
  
    void CreatGFramebuffer()
    {
       
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        // position color buffer
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ISCR_WIDTH, ISCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        // normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ISCR_WIDTH, ISCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        // color + specular color buffer
        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ISCR_WIDTH, ISCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);


        // create and attach depth buffer (renderbuffer)
        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ISCR_WIDTH, ISCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

     

    }

    void CreatSSAOFrameBuffer()
    {
       
        glGenFramebuffers(1, &ssaoFBO); 
        glGenFramebuffers(1, &ssaoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        // SSAO color buffer
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ISCR_WIDTH, ISCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Framebuffer not complete!" << std::endl;
        // and blur stage
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ISCR_WIDTH, ISCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void CreatSimpleKernel()
    {
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 64; ++i)
        {
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 64.0f;

            // scale samples s.t. they're more aligned to center of kernel
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }
    }

    void CreatNoiseTexture()
    {
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 16; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
            ssaoNoise.push_back(noise);
        }
       
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // SSAO four pass
    void GeometryPass(std::vector<MeshBase*>meshList, Camera& cam)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        for (int i = 0; i < meshList.size(); i++)
        {
            shaderGeometryPass->use();
            Shader* pshader = meshList[i]->shader;
            shaderGeometryPass->setVec3("albedo", pshader->getMaterial());
            //先将mesh的材质设置为shaderGeometryPass
            meshList[i]->shader = shaderGeometryPass;
            meshList[i]->Draw(meshList[i]->shader,cam);
            //渲染完后再将mesh的材质还原
            meshList[i]->shader = pshader;
        }

        //将深度缓冲拷贝到默认帧缓冲中
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, ISCR_WIDTH, ISCR_HEIGHT, 0, 0, ISCR_WIDTH, ISCR_HEIGHT,GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
 
    void SSAOTexturePass(Camera& cam)
    {
       glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
       glClear(GL_COLOR_BUFFER_BIT );


        shaderSSAO->use();
        UIParam* ui_param = UIParam::getInstance();
        shaderSSAO->setFloat("radius", ui_param->radius);
        shaderSSAO->setFloat("bias", ui_param->bias);
        // Send kernel + rotation 

        for (unsigned int i = 0; i < 64; ++i)
        shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        shaderSSAO->setMat4("projection", cam.GetPerspectiveMatrix());

       
        shaderSSAO->setFloat("screenW",ISCR_WIDTH);
        shaderSSAO->setFloat("screenH", ISCR_HEIGHT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        renderQuad();



    }

    void blurSSAOTexturePass()
    {
       glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
      
       glClear(GL_COLOR_BUFFER_BIT);

        shaderSSAOBlur->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }


    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    void ShaderConfiguration()
    {
        shaderSSAO->use();
        shaderSSAO->setInt("gPosition", 0);
        shaderSSAO->setInt("gNormal", 1);
        shaderSSAO->setInt("texNoise", 2);

        shaderSSAOBlur->use();
        shaderSSAOBlur->setInt("ssaoInput", 0);


        shaderframebuffer->use();
        shaderframebuffer->setInt("gPosition", 0);
        shaderframebuffer->setInt("gNormal", 1);
        shaderframebuffer->setInt("gAlbedo", 2);
    }

    private:
        unsigned int gBuffer;
        unsigned int quadVAO = 0;
        unsigned int quadVBO;
        unsigned int gPosition, gNormal, gAlbedo;
        unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
        unsigned int ssaoFBO, ssaoBlurFBO, noiseTexture;
        std::vector<glm::vec3> ssaoKernel;
        std::vector<glm::vec3> ssaoNoise;
        Shader* shaderSSAO;
        Shader* shaderSSAOBlur;
        Shader* shaderGeometryPass;
        Shader* shaderLighting;
        Shader* shaderframebuffer;
};


