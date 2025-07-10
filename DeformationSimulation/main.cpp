#include"TetGen.h"
void test1(int num);

void cudatest()
{    
    /*µ÷ÓÃCUDA*/
    test1(5000);
}
int main(void)
{
    std::vector<Node>nodes;
    std::vector<Edge> edges;
    std::vector<Face> faces;
    std::vector<Element> elements;
    TetGenRead("C:\\myApp\\code\\deformation_simulation\\PNCG_IPC-main\\PNCG_IPC-main\\model\\mesh\\e_2",nodes, edges, faces, elements);
    
    
   
    return 0;
}