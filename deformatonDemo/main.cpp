#include"renderAPP.h"
#include"mesh/mesh.h"
#include"mesh/primitive/plane.h"
#include"shader/BRDF.h""
#include"TetGen.h"
#include"type_define.h"
void start()
{
    std::vector< glm::vec3>nodes;
    std::vector<glm::uvec2> edges;
    std::vector<glm::uvec3> faces;
    std::vector<glm::uvec4> elements;
    TetGenRead("C:\\myApp\\code\\deformation_simulation\\PNCG_IPC-main\\PNCG_IPC-main\\model\\mesh\\e_2", nodes, edges, faces, elements);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (const auto& node : nodes) {
        Vertex v;
        v.Position = glm::vec3(node.x, node.y, node.z);
        vertices.push_back(v);
    }
    for (const auto& face : faces) {
        indices.push_back(face.x);
        indices.push_back(face.y);
        indices.push_back(face.z);
    }


    BRDF* shaderBRDF2 = new BRDF(glm::vec3(0.5, 0.2, 0.3));
 
    Mesh* mesh_e = new Mesh(vertices, indices);
     mesh_e->shader = shaderBRDF2;
   Object* mesh_e_o = new Object(mesh_e ,"E");
    Object* floor = new  plane(glm::vec3(10.0, 10.0, 10.0));

    RenderApp* pViewer = RenderApp::getInstance();
    pViewer->objectList.push_back(mesh_e_o);
}

void updata()
{
    RenderApp* pViewer = RenderApp::getInstance();
    Object* mesh_e_o = pViewer->objectList[0];
    for (int i = 0; i < mesh_e_o->m_meshes[0]->vertices.size(); i++)
    {
        Vertex v = mesh_e_o->m_meshes[0]->vertices[i];
        v.Position.y = v.Position.y-0.001f; // Example deformation
        mesh_e_o->m_meshes[0]->vertices[i] = v;
    }
    mesh_e_o->m_meshes[0]->MeshUpdate();
    pViewer->objectList[0] = mesh_e_o;
}


int main()
{
    RenderApp* pViewer = RenderApp::getInstance();

    pViewer->init(start);

    pViewer->run(updata);
}




