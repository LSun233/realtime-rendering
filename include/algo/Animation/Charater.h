#pragma once
#ifdef EXPORTEDCLASSES_EXPORTS
#define EXPORTEDCLASSES_API __declspec(dllexport)
#else
#define EXPORTEDCLASSES_API __declspec(dllimport)
#endif
#include<vector>
#include"../mesh/object.h"

class  Mesh;
class  Camera;
class Animation;
class Animator;
class EXPORTEDCLASSES_API Charater: public Object
{
public:

    Charater(std::vector<Mesh*>meshes, std::vector <Animation*> panimators) ;
    ~Charater();

    void ShadowDraw(Camera& cam);

    void Draw(Camera& cam);

    void UpdateAnimation(float deltaTime);

private:
    std::vector <Animation*> m_animations;

    std::vector <Animator*> m_animators;

private:




};
