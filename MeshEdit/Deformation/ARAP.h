#ifndef _ARAP_H
#include"../mesh.h"
#include <Eigen/Sparse>

class ARAP
{
public:
	Mesh* m_pmesh;
	ARAP(Mesh& _mesh);
	~ARAP();

private:
	Eigen::SparseMatrix<double>  CalculateLapalcianMatrix()
	{
		int vetex_num = m_pmesh->vertices.size();
		Eigen::SparseMatrix<double> L(vetex_num, vetex_num);
		for (int i = 0; i < vetex_num; i++)
		{
			//获取顶点i的相邻顶点索引。
			std::vector<int> surroundingVertices;
			m_pmesh->halfEdge->findSurroundingVertices(i, surroundingVertices);
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
};



ARAP::ARAP(Mesh& _mesh)
{
}

ARAP::~ARAP()
{
}






#endif // !_ARAP_H



