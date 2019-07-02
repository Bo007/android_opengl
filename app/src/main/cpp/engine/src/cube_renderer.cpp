#include "engine/include/cube_renderer.h"

#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CubeRenderer::CubeRenderer() {
    m_flatRenderer = std::make_unique<FlatRenderer>();
    m_aspect = 1.0;
}

CubeRenderer::~CubeRenderer() {
    m_flatRenderer.reset();
}

void CubeRenderer::render() {
    float angle = getAngleFromTime();
//    auto angle = static_cast<float>(M_PI / 180 * 45);
    auto mvp = getMvpMatrix(angle);

    m_flatRenderer->setMvpMatrix(mvp);
    m_flatRenderer->render();
}

glm::mat4 CubeRenderer::getMvpMatrix(float angle) const {
    glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
    glm::mat4 anim = glm::rotate(glm::mat4(1.0F), angle, rotationAxis);

    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0F, 0.0), glm::vec3(0.0, 0.0, -4.0F), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 model = glm::translate(glm::mat4(1.0F), glm::vec3(0.0, 0.0, -4.0F));

    const float far = 10;
    const float near = 0.1F;
    const float fov = 45;
    glm::mat4 projection = glm::perspective(fov, m_aspect, near, far);

    return projection * view * model * anim;
}

float CubeRenderer::getAngleFromTime() {
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 10000 * 10000;
    auto currentMs = (millis1 - millis2) / 10000.0F;

    return static_cast<float>(currentMs * glm::radians(360.0));  // base 15° per second
}

void CubeRenderer::rotate(const glm::vec3 &rotationVec) {
}

void CubeRenderer::setAspect(float aspect) {
    m_aspect = aspect;
}
