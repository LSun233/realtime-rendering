#include"BVH.h"

namespace BVH
{


    // 按照三角形中心排序 -- 比较函数
    bool cmpx(const Triangle& t1, const Triangle& t2) {
        return t1.center.x < t2.center.x;
    }
    bool cmpy(const Triangle& t1, const Triangle& t2) {
        return t1.center.y < t2.center.y;
    }
    bool cmpz(const Triangle& t1, const Triangle& t2) {
        return t1.center.z < t2.center.z;
    }


    BVH::BVH(const vector<Vertex>& vertices, const vector<unsigned int>& indices)
    {
        //生成三角面
        for (int i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 a = vertices[indices[i]].Position;
            glm::vec3 b = vertices[indices[i + 1]].Position;
            glm::vec3 c = vertices[indices[i + 2]].Position;
            Triangle tri = Triangle(a, b, c);
            tri.index = i / 3;
            Triangles.push_back(tri);
        }
        //构建bvh树
        creat(20);
    };
    void  BVH::creat(int N)  //叶子系节点的三角面片数量
    {
        BVHRoot = buildBVH(Triangles, 0, Triangles.size() - 1, N);
    }

    HitResult BVH::hit(Ray ray)
    {
        HitResult hr = hitBVH(ray, Triangles, BVHRoot);
        return hr;
    }
    BVH::~BVH()
    {

    }


       // 构建 BVH
        BVHNode* BVH::buildBVH(std::vector<Triangle>& triangles, int l, int r, int n) {
        if (l > r) return 0;

        BVHNode* node = new BVHNode();
        node->aabb.min = glm::vec3(9999999999, 9999999999, 9999999999);
        node->aabb.max = glm::vec3(-9999999999, -9999999999, -9999999999);

        // 计算 AABB
        for (int i = l; i <= r; i++) {
            // 最小点 AA
            float minx = glm::min(triangles[i].p1.x, glm::min(triangles[i].p2.x, triangles[i].p3.x));
            float miny = glm::min(triangles[i].p1.y, glm::min(triangles[i].p2.y, triangles[i].p3.y));
            float minz = glm::min(triangles[i].p1.z, glm::min(triangles[i].p2.z, triangles[i].p3.z));
            node->aabb.min.x = glm::min(node->aabb.min.x, minx);
            node->aabb.min.y = glm::min(node->aabb.min.y, miny);
            node->aabb.min.z = glm::min(node->aabb.min.z, minz);
            // 最大点 BB
            float maxx = glm::max(triangles[i].p1.x, glm::max(triangles[i].p2.x, triangles[i].p3.x));
            float maxy = glm::max(triangles[i].p1.y, glm::max(triangles[i].p2.y, triangles[i].p3.y));
            float maxz = glm::max(triangles[i].p1.z, glm::max(triangles[i].p2.z, triangles[i].p3.z));
            node->aabb.max.x = glm::max(node->aabb.max.x, maxx);
            node->aabb.max.y = glm::max(node->aabb.max.y, maxy);
            node->aabb.max.z = glm::max(node->aabb.max.z, maxz);
        }

        // 不多于 n 个三角形 返回叶子节点
        if ((r - l + 1) <= n) {
            node->n = r - l + 1;
            node->index = l;
            return node;
        }

        // 否则递归建树
        float lenx = node->aabb.max.x - node->aabb.min.x;
        float leny = node->aabb.max.y - node->aabb.min.y;
        float lenz = node->aabb.max.z - node->aabb.min.z;

        // 按 x 划分
        if (lenx >= leny && lenx >= lenz)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
        // 按 y 划分
        if (leny >= lenx && leny >= lenz)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
        // 按 z 划分
        if (lenz >= lenx && lenz >= leny)
            std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);

        // 递归
        int mid = (l + r) / 2;
        node->left = buildBVH(triangles, l, mid, n);
        node->right = buildBVH(triangles, mid + 1, r, n);

        return node;
    }
    // 和 aabb 盒子求交，没有交点则返回 -1
    float  BVH::hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB)
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
    // 光线和三角形求交 
    float  BVH::hitTriangle(Triangle* triangle, Ray ray)
    {


        glm::vec3 p1 = triangle->p1, p2 = triangle->p2, p3 = triangle->p3;
        glm::vec3 S = ray.startPoint;        // 射线起点
        glm::vec3 d = ray.direction;         // 射线方向
        glm::vec3 N = normalize(cross(p2 - p1, p3 - p1));    // 法向量
        if (dot(N, d) > 0.0f) N = -N;   // 获取正确的法向量

        // 如果视线和三角形平行
        if (fabs(dot(N, d)) < 0.00001f) return INF;

        // 距离
        float t = (dot(N, p1) - dot(S, N)) / dot(d, N);
        if (t < 0.0005f) return INF;    // 如果三角形在光线背面

        // 交点计算
        glm::vec3 P = S + d * t;

        // 判断交点是否在三角形中
        glm::vec3 c1 = cross(p2 - p1, P - p1);
        glm::vec3 c2 = cross(p3 - p2, P - p2);
        glm::vec3 c3 = cross(p1 - p3, P - p3);
        if (dot(c1, N) > 0 && dot(c2, N) > 0 && dot(c3, N) > 0) return t;
        if (dot(c1, N) < 0 && dot(c2, N) < 0 && dot(c3, N) < 0) return t;
        return INF;
    }
    // 暴力查数组
    HitResult  BVH::hitTriangleArray(Ray ray, std::vector<Triangle>& triangles, int l, int r)
    {
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

    // 在 BVH 上遍历求交
    HitResult  BVH::hitBVH(Ray ray, std::vector<Triangle>& triangles, BVHNode* root)
    {
        if (root == NULL) return HitResult();

        // 是叶子 暴力查
        if (root->n > 0) {
            return hitTriangleArray(ray, triangles, root->n, root->n + root->index - 1);
        }

        // 和左右子树 AABB 求交
        float d1 = INF, d2 = INF;
        if (root->left) d1 = hitAABB(ray, root->left->aabb.min, root->left->aabb.max);
        if (root->right) d2 = hitAABB(ray, root->right->aabb.min, root->right->aabb.max);

        // 递归结果
        HitResult r1, r2;
        if (d1 > 0) r1 = hitBVH(ray, triangles, root->left);
        if (d2 > 0) r2 = hitBVH(ray, triangles, root->right);

        return r1.distance < r2.distance ? r1 : r2;
    }


}











