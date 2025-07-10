#include "simulator.h"




simulator::simulator(std::vector< glm::vec3>nodes,std::vector<glm::uvec2> edges,std::vector<glm::uvec3> faces,std::vector<glm::uvec4> elements)
{
	this->nodes = nodes;
	this->velocity.resize(nodes.size(), glm::vec3(0.0f)); // Initialize velocity to zero
	this->elements = elements;
	//Initialize edges if needed, currently not used in this example
	this->edges = edges; // Assuming edges are not used in the current implementation
	this->faces = faces; // Assuming faces are not used in the current implementation



}

void simulator::updata()
{
	//更新模型
	for (int i = 0; i < this->velocity.size(); i++)
	{
		this->velocity[i] = this->velocity[i] - glm::vec3(0.0f, 0.001f, 0.0f); // Example deformation, applying a downward force
	}
	for (int i = 0; i < this->nodes.size(); i++)
	{
		this->nodes[i] = this->nodes[i] + this->velocity[i]; // Update node positions based on velocity
	}

}
