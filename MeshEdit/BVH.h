#pragma once
#include"type_define.h"
#include<vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
typedef struct Triangle {
    glm::vec3 p1, p2, p3;   // ����
    glm::vec3 center;       // ����
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        p1 = a, p2 = b, p3 = c;
        center = (p1 + p2 + p3) / glm::vec3(3, 3, 3);
    }
} Triangle;

// ������������������ -- �ȽϺ���
bool cmpx(const Triangle& t1, const Triangle& t2) {
    return t1.center.x < t2.center.x;
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
    return t1.center.y < t2.center.y;
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
    return t1.center.z < t2.center.z;
}

// BVH ���ڵ�
struct BVHNode {
    BVHNode* left = NULL;       // ������������
    BVHNode* right = NULL;
    int n, index;               // Ҷ�ӽڵ���Ϣ               
    glm::vec3 AA, BB;                // ��ײ��
};
// ���� BVH
BVHNode* buildBVH(std::vector<Triangle>& triangles, int l, int r, int n) {
    if (l > r) return 0;

    BVHNode* node = new BVHNode();
    node->AA = glm::vec3(1145141919, 1145141919, 1145141919);
    node->BB = glm::vec3(-1145141919, -1145141919, -1145141919);

    // ���� AABB
    for (int i = l; i <= r; i++) {
        // ��С�� AA
        float minx = glm::min(triangles[i].p1.x, glm::min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = glm::min(triangles[i].p1.y, glm::min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = glm::min(triangles[i].p1.z, glm::min(triangles[i].p2.z, triangles[i].p3.z));
        node->AA.x = glm::min(node->AA.x, minx);
        node->AA.y = glm::min(node->AA.y, miny);
        node->AA.z = glm::min(node->AA.z, minz);
        // ���� BB
        float maxx = glm::max(triangles[i].p1.x, glm::max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = glm::max(triangles[i].p1.y, glm::max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = glm::max(triangles[i].p1.z, glm::max(triangles[i].p2.z, triangles[i].p3.z));
        node->BB.x = glm::max(node->BB.x, maxx);
        node->BB.y = glm::max(node->BB.y, maxy);
        node->BB.z = glm::max(node->BB.z, maxz);
    }

    // ������ n �������� ����Ҷ�ӽڵ�
    if ((r - l + 1) <= n) {
        node->n = r - l + 1;
        node->index = l;
        return node;
    }

    // ����ݹ齨��
    float lenx = node->BB.x - node->AA.x;
    float leny = node->BB.y - node->AA.y;
    float lenz = node->BB.z - node->AA.z;
    // �� x ����
    if (lenx >= leny && lenx >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
    // �� y ����
    if (leny >= lenx && leny >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
    // �� z ����
    if (lenz >= lenx && lenz >= leny)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);

    // �ݹ�
    int mid = (l + r) / 2;
    node->left = buildBVH(triangles, l, mid, n);
    node->right = buildBVH(triangles, mid + 1, r, n);

    return node;
}

// ����
typedef struct Ray {
    glm::vec3 startPoint = glm::vec3(0, 0, 0);    // ���
    glm::vec3 direction = glm::vec3(0, 0, 0);     // ����
} Ray;

// �� aabb �����󽻣�û�н����򷵻� -1
float hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB)
{
    // 1.0 / direction
    glm::vec3 invdir = glm::vec3(1.0 / r.direction.x, 1.0 / r.direction.y, 1.0 / r.direction.z);

    glm::vec3 in = (BB - r.startPoint) * invdir;
    glm::vec3 out = (AA - r.startPoint) * invdir;

    glm::vec3 tmax = max(in, out);
    glm::vec3 tmin = min(in, out);

    float t1 = glm::min(tmax.x, glm::min(tmax.y, tmax.z));
    float t0 = glm::max(tmin.x, glm::max(tmin.y, tmin.z));

    return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
}

// �󽻽��
struct HitResult {
    Triangle* triangle = NULL;
    float distance = INF;
};

// ���ߺ��������� 
float hitTriangle(Triangle* triangle, Ray ray) {

    glm::vec3 p1 = triangle->p1, p2 = triangle->p2, p3 = triangle->p3;
    glm::vec3 S = ray.startPoint;        // �������
    glm::vec3 d = ray.direction;         // ���߷���
    glm::vec3 N = normalize(cross(p2 - p1, p3 - p1));    // ������
    if (dot(N, d) > 0.0f) N = -N;   // ��ȡ��ȷ�ķ�����

    // ������ߺ�������ƽ��
    if (fabs(dot(N, d)) < 0.00001f) return INF;

    // ����
    float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
    if (t < 0.0005f) return INF;    // ����������ڹ��߱���

    // �������
    glm::vec3 P = S + d * t;

    // �жϽ����Ƿ�����������
    glm::vec3 c1 = cross(p2 - p1, P - p1);
    glm::vec3 c2 = cross(p3 - p2, P - p2);
    glm::vec3 c3 = cross(p1 - p3, P - p3);
    if (dot(c1, N) > 0 && dot(c2, N) > 0 && dot(c3, N) > 0) return t;
    if (dot(c1, N) < 0 && dot(c2, N) < 0 && dot(c3, N) < 0) return t;
    return INF;
}

// ����������
HitResult hitTriangleArray(Ray ray, std::vector<Triangle>& triangles, int l, int r) {
    HitResult res;
    for (int i = l; i <= r; i++) {
        float d = hitTriangle(&triangles[i], ray);
        if (d < INF && d < res.distance) {
            res.distance = d;
            res.triangle = &triangles[i];
        }
    }
    return res;
}

// �� BVH �ϱ�����
HitResult hitBVH(Ray ray, std::vector<Triangle>& triangles, BVHNode* root) {
    if (root == NULL) return HitResult();

    // ��Ҷ�� ������
    if (root->n > 0) {
        return hitTriangleArray(ray, triangles, root->n, root->n + root->index - 1);
    }

    // ���������� AABB ��
    float d1 = INF, d2 = INF;
    if (root->left) d1 = hitAABB(ray, root->left->AA, root->left->BB);
    if (root->right) d2 = hitAABB(ray, root->right->AA, root->right->BB);

    // �ݹ���
    HitResult r1, r2;
    if (d1 > 0) r1 = hitBVH(ray, triangles, root->left);
    if (d2 > 0) r2 = hitBVH(ray, triangles, root->right);

    return r1.distance < r2.distance ? r1 : r2;
}

