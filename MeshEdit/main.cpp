#include"renderAPP.h"
#include"mesh/mesh.h"
#include"Animation/animation.h"
#include"FileIO/daeRead.h"
#include"Animation/Charater.h"
#include"mesh/primitive/plane.h"
#include"shader/BRDF.h""

void start()
{
    RenderApp* pViewer= RenderApp::getInstance();
    std::string xmlpath = "../data/model/Walking.dae";
    vector<Mesh*> meshes;
    vector<Animation*>animations;
    daeRead(xmlpath, meshes, animations);

    BRDF* shaderBRDF1 = new BRDF(glm::vec3(0.5, 0.0, 0.0));
    meshes[0]->shader = shaderBRDF1;


    BRDF* shaderBRDF2 = new BRDF(glm::vec3(0.5, 0.2, 0.3));
    meshes[1]->shader = shaderBRDF2;

    Charater* robot = new Charater(meshes, animations);

    pViewer->objectList.push_back(robot);

    Object* floor =new  plane(glm::vec3(10.0, 10.0, 10.0));
    pViewer->objectList.push_back(floor);

}

void updata()
{
    RenderApp* pViewer = RenderApp::getInstance();
}

int main()
{
    RenderApp* pView = new RenderApp();

    pView->init(start);

    pView->run(updata);

}




