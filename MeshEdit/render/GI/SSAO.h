#pragma once
#include"shader/shader_m.h"
#include <random>
#include"mesh/mesh.h"
#include"type_define.h"
class SSAO 
{
public:
    SSAO();


    ~SSAO();

    void render(std::vector<MeshBase*>meshList, Camera& cam, Shader* pRenderShader);



    void renderLight(Camera& cam, Shader* pRenderShader);





private:
    void CreatQuad();
    void renderQuad();
  
    void CreatGFramebuffer();
    void CreatSSAOFrameBuffer();

    void CreatSimpleKernel();

    void CreatNoiseTexture();

    // SSAO four pass
    void GeometryPass(std::vector<MeshBase*>meshList, Camera& cam);
 
    void SSAOTexturePass(Camera& cam);

    void blurSSAOTexturePass();


    float lerp(float a, float b, float f);

    void ShaderConfiguration();

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


