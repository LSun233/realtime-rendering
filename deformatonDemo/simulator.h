#pragma once
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include"TetGen.h"
class simulator
{
public:
	simulator(std::vector< glm::vec3>nodes, std::vector<glm::uvec2> edges, std::vector<glm::uvec3> faces, std::vector<glm::uvec4> elements);
	void updata();

public:
	std::vector<glm::vec3> nodes; // Nodes of the mesh
	std::vector<glm::uvec2> edges;
	std::vector<glm::uvec3> faces;
	std::vector<glm::uvec4> elements;
	std::vector<glm::vec3> velocity; // Edges of the mesh, each edge is a pair of node indices
	
};