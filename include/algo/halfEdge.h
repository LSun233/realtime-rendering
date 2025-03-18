#pragma once
#include"type_define.h"
#include <stdexcept>
struct HeVertex;
struct HeFace;
struct HeEdge;
struct HeVertex
{
public:
    HeEdge* edge = nullptr;
    int index = -1;
};

struct HeFace
{
public:
    HeEdge* edge = nullptr;
    int index = -1;
};

struct HeEdge
{
public:
    HeVertex* origin = nullptr;
    HeEdge* twin = nullptr;
    HeEdge* next = nullptr;
    HeFace* face = nullptr;
};
namespace HalfEdge
{
    class HalfEdgeMesh 
    {
        public:
            std::vector<HeVertex> m_vertices;
            std::vector<HeFace*> m_faces;
            std::vector<HeEdge*> m_halfEdges;
        public:
            HalfEdgeMesh(int num, const std::vector<unsigned int>& indices);
            void findSurroundingFaces(int index, std::vector<int>& surroundingFace);
            void findOuttingEdges(int index, std::vector<HeEdge*>& surroundingEdge);
            void findSurroundingVertices(int index, std::vector<int>& surroundingVertices);
    private:
        void addFace(const std::vector<unsigned int>& vertexIndices, int index);
    };


}


