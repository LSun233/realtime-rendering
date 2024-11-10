#pragma once
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
            HalfEdgeMesh(int num, const std::vector<unsigned int>& indices)
            {
                //初始化顶点
                m_vertices.resize(num);
                for (int i = 0; i < num; i++)
                {
                    HeVertex v;
                    v.index = i;
                    m_vertices[i]=v;
                }

                //添加面与半边
                for (int i = 0; i < indices.size(); i+=3)
                {
                    std::vector<unsigned int> vertexIndices = { indices[i],indices[i+1],indices[i+2] };
                 
                    addFace(vertexIndices,i/3);
                }
            }
            void findSurroundingFaces(int index, std::vector<int>& surroundingFace)
            {
                HeFace* targetFace = m_faces[index];
                HeEdge* startEdge = targetFace->edge;
                HeEdge* edge = startEdge;
                do {
                    
                    if ((edge != nullptr) ) 
                    {
                        if((edge->twin != nullptr))//避免边缘的边
                        surroundingFace.push_back(edge->twin->face->index);
                        edge = edge->next;
                    }
                } while (edge != startEdge);
            }
            void findSurroundingVertices(int index, std::vector<int>& surroundingVertices)
            {
                HeVertex Targetvetex = m_vertices[index];
                HeEdge* startEdge = Targetvetex.edge;
                HeEdge* edge = startEdge;
                do {

                    if ((edge != nullptr))
                    {
                        if ((edge->twin != nullptr))//避免边缘的边
                            surroundingVertices.push_back(edge->next->origin->index);
                        edge = edge->twin->next;
                    }
                } while (edge != startEdge);
            }
    private:
        void addFace(const std::vector<unsigned int>& vertexIndices, int index)
        {
            if (vertexIndices.size() != 3)
            {
                throw std::invalid_argument("Only triangular faces are supported.");
            }
            HeFace* newFace=new HeFace();
            newFace->index = index;
            m_faces.push_back(newFace);
         
            HeEdge* firstPtr= new HeEdge();
            HeEdge* secPtr = new HeEdge();
            HeEdge* thirdPtr = new HeEdge();

            //边的面
            firstPtr->face = newFace;
            secPtr->face = newFace;
            thirdPtr->face = newFace;

            //面的边
            newFace->edge = thirdPtr;

            //边的点
            firstPtr->origin = &m_vertices[vertexIndices[0]];
            secPtr->origin = &m_vertices[vertexIndices[1]];
            thirdPtr->origin = &m_vertices[vertexIndices[2]];

            //点的边
            m_vertices[vertexIndices[0]].edge = firstPtr;
            m_vertices[vertexIndices[1]].edge = secPtr;
            m_vertices[vertexIndices[2]].edge = thirdPtr;

            //nextEdge
            firstPtr->next = secPtr;
            secPtr->next = thirdPtr;
            thirdPtr->next = firstPtr;

            for (HeEdge* edge : m_halfEdges)
            {
                if (edge->next != nullptr) 
                {
                    if (edge->origin->index == m_vertices[vertexIndices[1]].index && edge->next->origin->index == m_vertices[vertexIndices[0]].index)
                    {
                        firstPtr->twin = edge;
                        edge->twin = firstPtr;
                        break;
                    }
                }
            }
            for (HeEdge* edge : m_halfEdges)
            {
                if (edge->next != nullptr) 
                {
                    if (edge->origin->index == m_vertices[vertexIndices[2]].index && edge->next->origin->index == m_vertices[vertexIndices[1]].index)
                    {
                        secPtr->twin = edge;
                        edge->twin = secPtr;
                 
                        break;
                    }
                }
            }
            for (HeEdge* edge : m_halfEdges){
                if (edge->next != nullptr) {
                    if (edge->origin->index == m_vertices[vertexIndices[0]].index && edge->next->origin->index == m_vertices[vertexIndices[2]].index)
                    {
                        thirdPtr->twin = edge;
                        edge->twin = thirdPtr;
                   
                        break;
                    }
                }
            }
            m_halfEdges.push_back(firstPtr);
            m_halfEdges.push_back(secPtr);
            m_halfEdges.push_back(thirdPtr);
        }
    };


}


