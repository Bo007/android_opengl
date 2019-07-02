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

    void rotate(const glm::vec3 &rotationVec);

    void setAspect(float aspect);

private:
    static float getAngleFromTime();

private:

    static const float CUBE_HALF_SIZE;

    std::unique_ptr<FlatRenderer> m_flatRenderer;

    std::array<glm::vec3, 27> m_translateVecs;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
};

