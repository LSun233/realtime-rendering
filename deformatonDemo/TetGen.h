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
// �������ݽṹ���洢��ͬ���͵��ļ�����
// ����ָ��Ŀ¼�о����ض���׺���ļ�
std::vector<std::string> findFilesByExtension(const std::string& directory, const std::string& extension);

// ��ȡ.node�ļ�
std::vector<glm::vec3> readNodeFile(const std::string& filename);


// ��ȡ.edge�ļ�
std::vector<glm::uvec2> readEdgeFile(const std::string& filename);

// ��ȡ.face�ļ�
std::vector<glm::uvec3> readFaceFile(const std::string& filename);

// ��ȡ.ele�ļ�
std::vector<glm::uvec4> readEleFile(const std::string& filename);

void  TetGenRead(std::string directory, std::vector<glm::vec3>& nodes, std::vector<glm::uvec2>& edges, std::vector<glm::uvec3>& faces, std::vector<glm::uvec4>& elements);