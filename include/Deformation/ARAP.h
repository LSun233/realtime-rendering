#ifndef _ARAP_H

#include <Eigen/Dense>
#include"../halfEdge.h"
#include<vector>

class ARAP
{
public:
	ARAP(std::vector<float>vertices,std::vector<unsigned int> indices);
	~ARAP();

private:
	Eigen::SparseMatrix<float> L_orig;
	std::vector<std::vector<float>> weights;
	Eigen::SparseMatrix<double>  CalculateLapalcianMatrix()
	{

		Eigen::SparseMatrix<double> L(m_vetex_num, m_vetex_num);
		for (int i = 0; i < m_vetex_num; i++)
		{
			//获取顶点i的相邻顶点索引。
			std::vector<int> surroundingVertices;
			m_halfedge->findSurroundingVertices(i, surroundingVertices);
			int num = surroundingVertices.size();
			L.insert(i,i) = -num;
			for (int j = 0; j < num; j++)
			{
				int index = surroundingVertices[j];
				L.insert(i, index) = -1;
			}
		}
		return L;
	}
	float weightsCalculate(HeEdge* he)
	{
		HeVertex* v1 = he->origin;
		HeVertex* v2 = he->next->origin;

		// 获取第一个三角形的第三个顶点
		HeVertex* v3 = he->next->next->origin;

		// 获取第二个三角形的第三个顶点
		HeVertex* v4 = he->twin->next->next->origin;

	     
		// 计算向量
		Vertex vec1 = { v2->x - v1->x, v2->y - v1->y, v2->z - v1->z };
		Vertex vec2 = { v3->x - v1->x, v3->y - v1->y, v3->z - v1->z };
		Vertex vec3 = { v4->x - v2->x, v4->y - v2->y, v4->z - v2->z };

		// 计算cot值
		float cotAlpha = cot(vec1, vec2);
		float cotBeta = cot(vec1, vec3);

		// 计算cot权重
		return 0.5f * (cotAlpha + cotBeta);
	}

	}

	void weightsCalculate()
	{

		for (int i = 0; i < m_vetex_num; i++)
		{
			//获取顶点i的相邻顶点索引。
			std::vector<HeEdge*> surroundingEdges;

			m_halfedge->findOuttingEdges(i, surroundingEdges);
			int num = surroundingEdges.size();
			weights[i].resize(num);
			for (int j = 0; j < num; j++)
			{
				HeEdge* edge = surroundingEdges[j];
				weights[i][j] = weightsCalculate(edge);

			}


		}



	}


	std::vector<float>m_vertices;
	std::vector<unsigned int> m_indices;
	HalfEdge::HalfEdgeMesh* m_halfedge;
	int m_vetex_num;
};



ARAP::ARAP(std::vector<float>vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	m_vetex_num= vertices.size()/3;
	Eigen::Matrix vertices_matrix(m_vetex_num, 3);
	Eigen::Matrix vertices(100, 3);
	m_halfedge = new HalfEdge::HalfEdgeMesh(m_vetex_num, indices);
	weights.resize(m_vetex_num);
}

ARAP::~ARAP()
{
}






#endif // !_ARAP_H



