#ifndef MESH_H
#define MESH_H
#include"MeshBase.h"
struct Vertex;
struct Texture;
struct BVHNode;
class Shader;
class Camera;

class Mesh: public MeshBase
{
public:
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

    Mesh(string pathPly);

    void loadTex(vector<Texture> textures);
    void DrawBVH(Shader& shader, Camera& cam);
    void Draw(Shader* shaderPass, Camera& cam) override;


    ~Mesh();
private:

    void DrawBVH(Camera& cam, BVHNode* BVHRoot);
    void setupMesh();
   
 
};
#endif
