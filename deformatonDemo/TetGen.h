#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace fs = std::filesystem;
// 定义数据结构来存储不同类型的文件内容
// 查找指定目录中具有特定后缀的文件
std::vector<std::string> findFilesByExtension(const std::string& directory, const std::string& extension);

// 读取.node文件
std::vector<glm::vec3> readNodeFile(const std::string& filename);


// 读取.edge文件
std::vector<glm::uvec2> readEdgeFile(const std::string& filename);

// 读取.face文件
std::vector<glm::uvec3> readFaceFile(const std::string& filename);

// 读取.ele文件
std::vector<glm::uvec4> readEleFile(const std::string& filename);

void  TetGenRead(std::string directory, std::vector<glm::vec3>& nodes, std::vector<glm::uvec2>& edges, std::vector<glm::uvec3>& faces, std::vector<glm::uvec4>& elements);