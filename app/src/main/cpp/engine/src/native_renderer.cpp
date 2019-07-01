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
}

NativeRenderer::~NativeRenderer() {
    m_cubeRenderer.reset();
}

void NativeRenderer::changed(int width, int height) {
    assert(width > 0 && height > 0);
    m_screenWidth = width;
    m_screenHeight = height;

}

void NativeRenderer::render() {
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 10000 * 10000;
    auto currentMs = (millis1 - millis2) / 10000.0F;

    float angle = currentMs * glm::radians(360.0);  // base 15° per second
    auto currentMvpMatrix = getMvpMatrix(angle);

    m_cubeRenderer->setMvpMatrix(currentMvpMatrix);
    m_cubeRenderer->render();
}

glm::mat4x4 NativeRenderer::getMvpMatrix(float angle) const {
    glm::mat4 anim =
            glm::rotate(glm::mat4(1.0F), angle * 3.0f, glm::vec3(1, 0, 0)) *  // X axis
            glm::rotate(glm::mat4(1.0F), angle * 2.0f, glm::vec3(0, 1, 0)) *  // Y axis
            glm::rotate(glm::mat4(1.0F), angle * 4.0f, glm::vec3(0, 0, 1));   // Z axis

    glm::mat4 model = glm::translate(glm::mat4(1.0F), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0F, 2.0F, 0.0F),
                                 glm::vec3(0.0F, 0.0F, -4.0F),
                                 glm::vec3(0.0F, 1.0F, 0.0F));

    const float far = 10;
    const float near = 0.1F;
    const float fov = 45;
    glm::mat4 projection = glm::perspective(fov, 1.0F * m_screenWidth / m_screenHeight, near, far);

    glm::mat4 mvp = projection * view * model * anim;

    return mvp;
}