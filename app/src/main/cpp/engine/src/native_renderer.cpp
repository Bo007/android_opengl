#include "engine/include/native_renderer.h"

#include <vector>
#include <string>
#include <chrono>

#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 NativeRenderer::CLEAR_COLOR = glm::vec3(0.3F, 0.3F, 0.3F);

NativeRenderer::NativeRenderer() {
    m_screenWidth = m_screenHeight = 0;
    m_cubeRenderer = std::make_unique<CubeRenderer>();

//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
//    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
}

NativeRenderer::~NativeRenderer() {
    m_cubeRenderer.reset();
}

void NativeRenderer::changed(int width, int height) {
    assert(width > 0 && height > 0);
    m_screenWidth = width;
    m_screenHeight = height;
    m_cubeRenderer->setAspect(1.0F * m_screenWidth / m_screenHeight);
}

void NativeRenderer::render() {
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_cubeRenderer->render();
}

void NativeRenderer::rotateFace(int faceId) {
    auto angle = glm::radians(90.0F);
    m_cubeRenderer->rotateCubeFace(faceId, angle);
}