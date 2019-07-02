#include "engine/include/cube_renderer.h"

#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "logger.h"

constexpr float CubeRenderer::CUBE_HALF_SIZE = 0.5F / 3;

CubeRenderer::CubeRenderer() {
    m_flatRenderer = std::make_unique<FlatRenderer>(-CUBE_HALF_SIZE, CUBE_HALF_SIZE);

    auto defaultTranslateVec = glm::vec3(0.0, 0.0, -4.0F);

    m_translateVecs = {};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {

            for (int k = 0; k < 3; ++k) {
                m_translateVecs[i * 9 + j * 3 + k].x =
                        defaultTranslateVec.x + 2 * CubeRenderer::CUBE_HALF_SIZE * (i - 1);
                m_translateVecs[i * 9 + j * 3 + k].y =
                        defaultTranslateVec.y + 2 * CubeRenderer::CUBE_HALF_SIZE * (j - 1);
                m_translateVecs[i * 9 + j * 3 + k].z =
                        defaultTranslateVec.z + 2 * CubeRenderer::CUBE_HALF_SIZE * (k - 1);
            }
        }
    }

    auto defaultRotationVec = glm::vec3(0.0, 2.0F, 0.0);
    auto rotationDirectionVec = glm::vec3(0.0, 1.0F, 0.0);
    m_viewMatrix = glm::lookAt(defaultRotationVec, defaultTranslateVec, rotationDirectionVec);

    setAspect(1);
}

CubeRenderer::~CubeRenderer() {
    m_flatRenderer.reset();
}

void CubeRenderer::render() {
//    float angle = getAngleFromTime();
    auto angle = static_cast<float>(M_PI / 180 * 0);

    for (auto &translateVec: m_translateVecs) {

        glm::vec3 rotationAxis = glm::vec3(0, 1, 0);
        glm::mat4 anim = glm::rotate(glm::mat4(1.0F), angle, rotationAxis);

        auto mvp = glm::translate(m_projectionMatrix * m_viewMatrix, translateVec) * anim;
        m_flatRenderer->setMvpMatrix(mvp);
        m_flatRenderer->render();
    }
}

void CubeRenderer::rotate(const glm::vec3 &rotationVec) {
}

void CubeRenderer::setAspect(float aspect) {
    const float far = 10;
    const float near = 0.1F;
    const float fov = 45;
    m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

float CubeRenderer::getAngleFromTime() {
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 10000 * 10000;
    auto currentMs = (millis1 - millis2) / 10000.0F;

    return static_cast<float>(currentMs * glm::radians(360.0));  // base 15° per second
}
