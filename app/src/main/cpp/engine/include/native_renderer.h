#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

#include "engine/include/cube_renderer.h"

class NativeRenderer {
public:
    NativeRenderer();

    ~NativeRenderer();

    void changed(int width, int height);

    void render();

private:
    glm::mat4x4 getMvpMatrix(float angle) const;
private:

    const static glm::vec3 CLEAR_COLOR;

    std::unique_ptr<CubeRenderer> m_cubeRenderer;

    int m_screenWidth;
    int m_screenHeight;
};

