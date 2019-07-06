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

    buildFacesIndeces();

    // Rotate cube faces
    auto angle = glm::radians(90.0F);
////  rotate left / right
    rotateCubeFace(0, angle);
//    rotateCubeFace(5, angle);

//// rotate bottom / top
    rotateCubeFace(1, angle);
//    rotateCubeFace(4, angle);

//// rotate back / front
//    rotateCubeFace(2, angle);
//    rotateCubeFace(3, angle);
}

CubeRenderer::~CubeRenderer() {
    m_flatRenderer.reset();
}

void CubeRenderer::render() {
// Rotate cube
//    auto angle = glm::radians(270.0F);
    auto angle = getAngleFromTime();
    auto rotationVec = glm::vec3(0, 1, 0) * angle;
    rotateCube(rotationVec);

    for (int i = 0; i < m_translateVecs.size(); ++i) {
        m_flatRenderer->setMvpMatrix(m_mvpMatrix * m_rotationMatrices[i], m_translateVecs[i]);
        m_flatRenderer->render();
    }
}

void CubeRenderer::rotateCube(const glm::vec3 &rotationVec) {
    float angle = glm::length(rotationVec);
    glm::vec3 axis = glm::normalize(rotationVec);

    m_rotationMatrix = glm::mat4(1);
    if (static_cast<bool>(angle)) {
        m_rotationMatrix = glm::rotate(m_rotationMatrix, angle, axis);
    }

    calculateMvpMatrix();
}

void CubeRenderer::rotateCubeFace(int faceIndex, float angle) {
    auto *cubeFace = &m_cubeFace[faceIndex];
    for (int i = 0; i < cubeFace->m_indices.size(); ++i) {
        int index = *cubeFace->m_indices[i];
//        m_rotationMatrices[index] = glm::mat4(1);
        if (static_cast<bool>(angle)) {
            m_rotationMatrices[index] =
                    glm::rotate(glm::mat4(1), angle, cubeFace->m_rotationAxis) * m_rotationMatrices[index];
        }
    }
    rebuildFaceIndeces(faceIndex);
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

void CubeRenderer::buildFacesIndeces() {

    for (int i = 0; i < 4; ++i) {
        m_rotatedVerticles[i] = i;
        m_rotatedVerticles[4 + i] = 5 + i;

        m_rotatedVerticles[8 + i] = 2 * i + 9;

        m_rotatedVerticles[12 + i] = i + 18;
        m_rotatedVerticles[16 + i] = i + 23;
    }

    m_rotatedVerticles[8] = 9;
    m_rotatedVerticles[9] = 11;
    m_rotatedVerticles[10] = 15;
    m_rotatedVerticles[11] = 17;

    // left / right
    int indexesSize = 8;
    for (int i = 0; i < indexesSize; ++i) {
        m_cubeFace[0].m_indices[i] = &m_rotatedVerticles[i];
        m_cubeFace[5].m_indices[i] = &m_rotatedVerticles[i + 12];
    }

    // bottom
    m_cubeFace[1].m_indices[0] = &m_rotatedVerticles[0];
    m_cubeFace[1].m_indices[1] = &m_rotatedVerticles[1];
    m_cubeFace[1].m_indices[2] = &m_rotatedVerticles[2];

    m_cubeFace[1].m_indices[3] = &m_rotatedVerticles[8];
    m_cubeFace[1].m_indices[4] = &m_rotatedVerticles[9];

    m_cubeFace[1].m_indices[5] = &m_rotatedVerticles[12];
    m_cubeFace[1].m_indices[6] = &m_rotatedVerticles[13];
    m_cubeFace[1].m_indices[7] = &m_rotatedVerticles[14];
    // top
    m_cubeFace[4].m_indices[0] = &m_rotatedVerticles[5];
    m_cubeFace[4].m_indices[1] = &m_rotatedVerticles[6];
    m_cubeFace[4].m_indices[2] = &m_rotatedVerticles[7];

    m_cubeFace[4].m_indices[3] = &m_rotatedVerticles[10];
    m_cubeFace[4].m_indices[4] = &m_rotatedVerticles[11];

    m_cubeFace[4].m_indices[5] = &m_rotatedVerticles[17];
    m_cubeFace[4].m_indices[6] = &m_rotatedVerticles[18];
    m_cubeFace[4].m_indices[7] = &m_rotatedVerticles[19];

    // back
    m_cubeFace[2].m_indices[0] = &m_rotatedVerticles[0];
    m_cubeFace[2].m_indices[1] = &m_rotatedVerticles[3];
    m_cubeFace[2].m_indices[2] = &m_rotatedVerticles[5];

    m_cubeFace[2].m_indices[3] = &m_rotatedVerticles[8];
    m_cubeFace[2].m_indices[4] = &m_rotatedVerticles[10];

    m_cubeFace[2].m_indices[5] = &m_rotatedVerticles[12];
    m_cubeFace[2].m_indices[6] = &m_rotatedVerticles[15];
    m_cubeFace[2].m_indices[7] = &m_rotatedVerticles[17];

    // front
    m_cubeFace[3].m_indices[0] = &m_rotatedVerticles[2];
    m_cubeFace[3].m_indices[1] = &m_rotatedVerticles[4];
    m_cubeFace[3].m_indices[2] = &m_rotatedVerticles[7];

    m_cubeFace[3].m_indices[3] = &m_rotatedVerticles[9];
    m_cubeFace[3].m_indices[4] = &m_rotatedVerticles[11];

    m_cubeFace[3].m_indices[5] = &m_rotatedVerticles[14];
    m_cubeFace[3].m_indices[6] = &m_rotatedVerticles[16];
    m_cubeFace[3].m_indices[7] = &m_rotatedVerticles[19];
}

void CubeRenderer::rebuildFaceIndeces(int index) {
    auto &faceIndices = m_cubeFace[index].m_indices;

    for (auto item: faceIndices) {
        LOGI("before rotation faceIndices = %d", *item);
    }

    // rotate left
    int temp = *faceIndices[0];
    *faceIndices[0] = *faceIndices[2];
    *faceIndices[2] = *faceIndices[7];
    *faceIndices[7] = *faceIndices[5];
    *faceIndices[5] = temp;

    temp = *faceIndices[1];
    *faceIndices[1] = *faceIndices[4];
    *faceIndices[4] = *faceIndices[6];
    *faceIndices[6] = *faceIndices[3];
    *faceIndices[3] = temp;

    // rotate right
//    int temp = *faceIndices[0];
//    *faceIndices[0] = *faceIndices[5];
//    *faceIndices[5] = *faceIndices[7];
//    *faceIndices[7] = *faceIndices[2];
//    *faceIndices[2] = temp;
//
//    temp = *faceIndices[1];
//    *faceIndices[1] = *faceIndices[3];
//    *faceIndices[3] = *faceIndices[6];
//    *faceIndices[6] = *faceIndices[4];
//    *faceIndices[4] = temp;

    for (auto item: faceIndices) {
        LOGI("after rotation faceIndices = %d", *item);
    }
}
