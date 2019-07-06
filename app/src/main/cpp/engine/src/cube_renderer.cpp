#include "engine/include/cube_renderer.h"

#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "logger.h"

constexpr float CubeRenderer::CUBE_SIZE = 0.33F;

CubeRenderer::CubeRenderer() {
    m_flatRenderer = std::make_unique<FlatRenderer>(-0.5 * CUBE_SIZE, 0.5 * CUBE_SIZE);

    m_translateVecs = {};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                int index = i * 9 + j * 3 + k;
                m_translateVecs[index] = glm::vec3(i - 1, j - 1, k - 1) * CubeRenderer::CUBE_SIZE;
                m_rotationMatrices[index] = glm::mat4(1);
            }
        }
    }

    auto defaultRotationVec = glm::vec3(0.0, 2.0F, 0.0);
    auto defaultTranslateVec = glm::vec3(0.0, 0.0, -4.0F);
    auto rotationDirectionVec = glm::vec3(0.0, 1.0F, 0.0);
    m_viewMatrix = glm::lookAt(defaultRotationVec, defaultTranslateVec, rotationDirectionVec);

    m_rotationMatrix = glm::mat4(1);

    setAspect(1);

    m_cubeFace[0].m_rotationAxis = glm::vec3(1, 0, 0);
    m_cubeFace[5].m_rotationAxis = glm::vec3(-1, 0, 0);

    m_cubeFace[1].m_rotationAxis = glm::vec3(0, 1, 0);
    m_cubeFace[4].m_rotationAxis = glm::vec3(0, -1, 0);

    m_cubeFace[2].m_rotationAxis = glm::vec3(0, 0, 1);
    m_cubeFace[3].m_rotationAxis = glm::vec3(0, 0, -1);

    rebuildFacesIndeces();
}

CubeRenderer::~CubeRenderer() {
    m_flatRenderer.reset();
}

void CubeRenderer::render() {
    // Rotate cube faces
    auto angle = glm::radians(90.0f);
////  rotate left / right
    rotateCubeFace(0, angle);
//    rotateCubeFace(5, angle);

//// rotate bottom / top
//    rotateCubeFace(1, angle);
//    rotateCubeFace(4, angle);

//// rotate back / front
//    rotateCubeFace(2, angle);
//    rotateCubeFace(3, angle);

// Rotate cube
//    angle = getAngleFromTime();
//    auto rotationVec = glm::vec3(0, 1, 0) * angle;
//    rotateCube(rotationVec);


    for (int i = 0; i < m_translateVecs.size(); ++i) {
        m_flatRenderer->setMvpMatrix(m_mvpMatrix * m_rotationMatrices[i], m_translateVecs[i]);
        m_flatRenderer->render();
    }
}

void CubeRenderer::rotateCube(const glm::vec3 &rotationVec) {
    float angle = glm::length(rotationVec);
    glm::vec3 axis = glm::normalize(rotationVec);

    m_rotationMatrix = glm::mat4(1.0F);
    if (static_cast<bool>(angle)) {
        m_rotationMatrix = glm::rotate(m_rotationMatrix, angle, axis);
    }

    calculateMvpMatrix();
}

void CubeRenderer::rotateCubeFace(int faceIndex, float angle) {
    auto *cubeFace = &m_cubeFace[faceIndex];
    for (int i = 0; i < cubeFace->m_indices.size(); ++i) {
        int index = cubeFace->m_indices[i];
        m_rotationMatrices[index] = glm::mat4(1.0F);
        if (static_cast<bool>(angle)) {
            m_rotationMatrices[index] = glm::rotate(m_rotationMatrices[index], angle, cubeFace->m_rotationAxis);
        }
    }
}

void CubeRenderer::setAspect(float aspect) {
    const float far = 10;
    const float near = 0.1F;
    const float fov = 45;
    m_projectionMatrix = glm::perspective(fov, aspect, near, far);

    calculateMvpMatrix();
}

float CubeRenderer::getAngleFromTime() {
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 10000 * 10000;
    auto currentMs = (millis1 - millis2) / 10000.0F;

    return static_cast<float>(currentMs * glm::radians(360.0));  // base 15° per second
}

void CubeRenderer::calculateMvpMatrix() {
    auto defaultTranslateVec = glm::vec3(0.0, 0.0, -4.0F);
    m_mvpMatrix = glm::translate(m_projectionMatrix * m_viewMatrix, defaultTranslateVec) * m_rotationMatrix;
}

void CubeRenderer::rebuildFacesIndeces() {
    const int rows = 3;
    const int cols = 3;
    constexpr int matrixSize = rows * cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * 3 + j;
            m_cubeFace[0].m_indices[index] = index;
            m_cubeFace[5].m_indices[index] = m_cubeFace[0].m_indices[index] + 2 * matrixSize;

            m_cubeFace[1].m_indices[index] = m_cubeFace[0].m_indices[j] + i * matrixSize;
            m_cubeFace[4].m_indices[index] = m_cubeFace[1].m_indices[index] + 6;

            m_cubeFace[2].m_indices[index] = m_cubeFace[0].m_indices[j * 3 + i] + i * matrixSize;
            m_cubeFace[3].m_indices[index] = m_cubeFace[2].m_indices[index] + 2;
        }
    }
}