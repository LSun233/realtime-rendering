#include"TetGen.h"
// 定义数据结构来存储不同类型的文件内容
// 查找指定目录中具有特定后缀的文件
std::vector<std::string> findFilesByExtension(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// 读取.node文件
std::vector<glm::vec3> readNodeFile(const std::string& filename) {
    std::vector<glm::vec3> nodes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    std::string line;
    // 读取第一行(节点数量，维度，属性数量，边界标记)
    std::getline(file, line);
    std::istringstream iss(line);
    int numNodes, dim, numAttributes, boundaryMarkers;
    iss >> numNodes >> dim >> numAttributes >> boundaryMarkers;

    // 读取每个节点
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // 跳过空行和注释

        std::istringstream nodeStream(line);
        glm::vec3 node;
        int id;
        nodeStream >> id;

        // 读取坐标
        nodeStream >> node.x >> node.y >> node.z;
        // 忽略属性和边界标记
        nodes.push_back(node);
    }

    return nodes;
}

// 读取.edge文件
std::vector<glm::uvec2> readEdgeFile(const std::string& filename) {
    std::vector<glm::uvec2> edges;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    std::string line;
    // 读取第一行(边的数量，边界标记)
    std::getline(file, line);
    std::istringstream iss(line);
    int numEdges, boundaryMarkers;
    iss >> numEdges >> boundaryMarkers;
    int id;
    // 读取每条边
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // 跳过空行和注释

        std::istringstream edgeStream(line);
        glm::uvec2 edge;
        edgeStream >> id >> edge.x >> edge.y;
        edges.push_back(edge);
    }

    return edges;
}

// 读取.face文件
std::vector<glm::uvec3> readFaceFile(const std::string& filename) {
    std::vector<glm::uvec3> faces;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    std::string line;
    // 读取第一行(面的数量，边界标记)
    std::getline(file, line);
    std::istringstream iss(line);
    int numFaces, boundaryMarkers;
    iss >> numFaces >> boundaryMarkers;
    int id;
    // 读取每个面
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // 跳过空行和注释

        std::istringstream faceStream(line);
        glm::uvec3 face;
        faceStream >> id >> face.x >> face.y >> face.z;



        faces.push_back(face);
    }

    return faces;
}

// 读取.ele文件
std::vector<glm::uvec4> readEleFile(const std::string& filename) {
    std::vector<glm::uvec4> elements;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }

    std::string line;
    // 读取第一行(单元数量，每个单元的节点数，区域属性)
    std::getline(file, line);
    std::istringstream iss(line);
    int numElements, nodesPerElement, regionAttribute;
    iss >> numElements >> nodesPerElement >> regionAttribute;
    int id;
    // 读取每个单元
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // 跳过空行和注释

        std::istringstream eleStream(line);
        glm::uvec4 element;
        eleStream >> id >> element.x >> element.y
            >> element.z >> element.w;
        elements.push_back(element);
    }
    return elements;
}

void  TetGenRead(std::string directory, std::vector<glm::vec3>& nodes, std::vector<glm::uvec2>& edges, std::vector<glm::uvec3>& faces, std::vector<glm::uvec4>& elements) {
    try {
        // 读取文件(请根据实际情况修改文件名)
        std::vector<std::string> nodeFiles = findFilesByExtension(directory, ".node");
        std::vector<std::string> edgeFiles = findFilesByExtension(directory, ".edge");
        std::vector<std::string> faceFiles = findFilesByExtension(directory, ".face");
        std::vector<std::string> eleFiles = findFilesByExtension(directory, ".ele");
        nodes = readNodeFile(nodeFiles[0]);
        edges = readEdgeFile(edgeFiles[0]);
        faces = readFaceFile(faceFiles[0]);
        elements = readEleFile(eleFiles[0]);

        // 输出读取的信息
        std::cout << "成功读取 " << nodes.size() << " 个节点" << std::endl;
        std::cout << "成功读取 " << edges.size() << " 条边" << std::endl;
        std::cout << "成功读取 " << faces.size() << " 个面" << std::endl;
        std::cout << "成功读取 " << elements.size() << " 个单元" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }

}