#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

#include "flat_renderer.h"

class CubeRenderer {
public:
    CubeRenderer();

    ~CubeRenderer();

    void render();

    void rotateCube(const glm::vec3 &rotationVec);

    void rotateCubeFace(int faceIndex, float angle);

    void setAspect(float aspect);

private:
    static float getAngleFromTime();

    void calculateMvpMatrix();

    void rebuildFacesIndeces();

private:

    static const float CUBE_SIZE;

    std::unique_ptr<FlatRenderer> m_flatRenderer;

    std::array<glm::vec3, 27> m_translateVecs;
    std::array<glm::mat4, 27> m_rotationMatrices;

    struct CubeFace {
        glm::vec3 m_rotationAxis;
        std::array<int, 9> m_indices;
    };
    std::array<CubeFace, 6> m_cubeFace;


    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_rotationMatrix;

    glm::mat4 m_mvpMatrix;
};

