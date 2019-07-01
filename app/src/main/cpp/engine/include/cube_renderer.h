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
    glm::mat4 getMvpMatrix(float angle) const;

    static float getAngleFromTime();

private:
    std::unique_ptr<FlatRenderer> m_flatRenderer;

    float m_aspect;
};

