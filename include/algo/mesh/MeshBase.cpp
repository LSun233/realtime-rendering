#include"MeshBase.h"
#include"../halfEdge.h"

#include"../camera.h"
    // constructor
MeshBase::MeshBase(glm::vec3 postion)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, postion);
    }
  void MeshBase::Draw(Shader* shaderpass, Camera& cam)
    {


        shaderpass->use();
        shaderpass->setVec3("camPos", cam.Position);

        glm::mat4 mat = GetModelMat();

        shaderpass->setMVPmatrix(GetModelMat(), cam.GetViewMatrix(), cam.GetPerspectiveMatrix());
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TYPE, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }


   void MeshBase::setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


    }

    void MeshBase::rotation(glm::vec3 axis, float angle)
    {

        glm::mat4 transM = glm::mat4(1.0f);
        transM = glm::rotate(transM, angle, axis);
        model = model * transM;
    }
    void MeshBase::translate(glm::vec3 trans)
    {
        glm::mat4 transM = glm::mat4(1.0f);
        transM = glm::translate(transM, trans);
        model = transM * model;
    }
    glm::vec3 MeshBase::GetPostion()
    {
        return glm::vec3(model[3][0], model[3][1], model[3][2]);
    }
    glm::mat4 MeshBase::GetRotation()
    {
        return Rotation;
    }
    void MeshBase::SetRotation(glm::qua<float> rotation)
    {
        Rotation_qua = rotation;
        Rotation = glm::mat4_cast(Rotation_qua);

        // model = glm::mat4(1.0f);
        model = Rotation * model;
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);
    }
    void MeshBase::SetScale(glm::vec3 scale)
    {
        Scale = scale;

        model = glm::mat4(1.0f);
        model = Rotation * model;
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);
    }
    glm::mat4 MeshBase::GetModelMat()
    {
        return model;
    }
    void MeshBase::SetModelMat(glm::mat4 mat)
    {
        model = mat;
    }

    void MeshBase::CreatAABB()
    {
        float xmax = -9999999;
        float ymax = -9999999;
        float zmax = -9999999;
        float xmin = +9999999;
        float ymin = +9999999;
        float zmin = +9999999;


        for (int j = 0; j < vertices.size(); j++)
        {
            // ¼ÆËãpMax
            if (xmax < vertices[j].Position.x)
                xmax = vertices[j].Position.x;
            if (ymax < vertices[j].Position.y)
                ymax = vertices[j].Position.y;
            if (zmax < vertices[j].Position.z)
                zmax = vertices[j].Position.z;

            // ¼ÆËãpMin
            if (xmin > vertices[j].Position.x)
                xmin = vertices[j].Position.x;
            if (ymin > vertices[j].Position.y)
                ymin = vertices[j].Position.y;
            if (zmin > vertices[j].Position.z)
                zmin = vertices[j].Position.z;
        }
        aabb.max = glm::vec3(xmax, ymax, zmax);
        aabb.min = glm::vec3(xmin, ymin, zmin);
        aabb.center = 0.5f * (aabb.max + aabb.min);

        //aabb.max -= aabb.center;
        //aabb.min -= aabb.center;
    }

    // TOPO
    void MeshBase::CreatBVH()
    {
        bvh = new BVH::BVH(vertices, indices);
    }
    void MeshBase::CreatHalfEdgeMesh()
    {
        halfEdge = new HalfEdge::HalfEdgeMesh(vertices.size(), indices);

    }
    float MeshBase::hit(Ray r)
    {
        return hitAABB(r, aabb.min, aabb.max);

    }
    float MeshBase::hitAABB(Ray r, glm::vec3 AA, glm::vec3 BB)
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
    MeshBase::~MeshBase()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

