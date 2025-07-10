#include"TetGen.h"
// �������ݽṹ���洢��ͬ���͵��ļ�����
// ����ָ��Ŀ¼�о����ض���׺���ļ�
std::vector<std::string> findFilesByExtension(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// ��ȡ.node�ļ�
std::vector<glm::vec3> readNodeFile(const std::string& filename) {
    std::vector<glm::vec3> nodes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("�޷����ļ�: " + filename);
    }

    std::string line;
    // ��ȡ��һ��(�ڵ�������ά�ȣ������������߽���)
    std::getline(file, line);
    std::istringstream iss(line);
    int numNodes, dim, numAttributes, boundaryMarkers;
    iss >> numNodes >> dim >> numAttributes >> boundaryMarkers;

    // ��ȡÿ���ڵ�
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // �������к�ע��

        std::istringstream nodeStream(line);
        glm::vec3 node;
        int id;
        nodeStream >> id;

        // ��ȡ����
        nodeStream >> node.x >> node.y >> node.z;
        // �������Ժͱ߽���
        nodes.push_back(node);
    }

    return nodes;
}

// ��ȡ.edge�ļ�
std::vector<glm::uvec2> readEdgeFile(const std::string& filename) {
    std::vector<glm::uvec2> edges;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("�޷����ļ�: " + filename);
    }

    std::string line;
    // ��ȡ��һ��(�ߵ��������߽���)
    std::getline(file, line);
    std::istringstream iss(line);
    int numEdges, boundaryMarkers;
    iss >> numEdges >> boundaryMarkers;
    int id;
    // ��ȡÿ����
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // �������к�ע��

        std::istringstream edgeStream(line);
        glm::uvec2 edge;
        edgeStream >> id >> edge.x >> edge.y;
        edges.push_back(edge);
    }

    return edges;
}

// ��ȡ.face�ļ�
std::vector<glm::uvec3> readFaceFile(const std::string& filename) {
    std::vector<glm::uvec3> faces;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("�޷����ļ�: " + filename);
    }

    std::string line;
    // ��ȡ��һ��(����������߽���)
    std::getline(file, line);
    std::istringstream iss(line);
    int numFaces, boundaryMarkers;
    iss >> numFaces >> boundaryMarkers;
    int id;
    // ��ȡÿ����
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // �������к�ע��

        std::istringstream faceStream(line);
        glm::uvec3 face;
        faceStream >> id >> face.x >> face.y >> face.z;



        faces.push_back(face);
    }

    return faces;
}

// ��ȡ.ele�ļ�
std::vector<glm::uvec4> readEleFile(const std::string& filename) {
    std::vector<glm::uvec4> elements;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("�޷����ļ�: " + filename);
    }

    std::string line;
    // ��ȡ��һ��(��Ԫ������ÿ����Ԫ�Ľڵ�������������)
    std::getline(file, line);
    std::istringstream iss(line);
    int numElements, nodesPerElement, regionAttribute;
    iss >> numElements >> nodesPerElement >> regionAttribute;
    int id;
    // ��ȡÿ����Ԫ
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // �������к�ע��

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
        // ��ȡ�ļ�(�����ʵ������޸��ļ���)
        std::vector<std::string> nodeFiles = findFilesByExtension(directory, ".node");
        std::vector<std::string> edgeFiles = findFilesByExtension(directory, ".edge");
        std::vector<std::string> faceFiles = findFilesByExtension(directory, ".face");
        std::vector<std::string> eleFiles = findFilesByExtension(directory, ".ele");
        nodes = readNodeFile(nodeFiles[0]);
        edges = readEdgeFile(edgeFiles[0]);
        faces = readFaceFile(faceFiles[0]);
        elements = readEleFile(eleFiles[0]);

        // �����ȡ����Ϣ
        std::cout << "�ɹ���ȡ " << nodes.size() << " ���ڵ�" << std::endl;
        std::cout << "�ɹ���ȡ " << edges.size() << " ����" << std::endl;
        std::cout << "�ɹ���ȡ " << faces.size() << " ����" << std::endl;
        std::cout << "�ɹ���ȡ " << elements.size() << " ����Ԫ" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "����: " << e.what() << std::endl;
    }

}