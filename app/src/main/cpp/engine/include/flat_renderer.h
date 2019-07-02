#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

class FlatRenderer {
public:
    FlatRenderer(float minCubeSize, float maxCubeSize);

    ~FlatRenderer();

    void render();

    void setMvpMatrix(const glm::mat4 &mvpMatrix);

private:

    GLuint m_programID;

    GLint m_posititonCoordinateHandle;
    GLint m_mvpHandle;
    GLint m_colorHandle;

    glm::mat4 m_mvpMatrix;

    static constexpr int FACE_NUMBERS = 6;
    static constexpr float FACE_KOEF = 0.9F;

    static const std::array<GLushort, 6> VERTEX_INDICES;

    std::array<glm::vec3, FACE_NUMBERS * 4> m_vertex;

    std::array<glm::vec3, FACE_NUMBERS> m_facesColors;
};

