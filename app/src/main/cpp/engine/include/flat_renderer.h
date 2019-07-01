#pragma once

#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

class FlatRenderer {
public:
    FlatRenderer();

    ~FlatRenderer();

    void render();

    void setMvpMatrix(const glm::mat4 &mvpMatrix);

private:

    GLuint m_programID;

    GLint m_posititonCoordinateHandle;
    GLint m_mvpHandle;

    glm::mat4 m_mvpMatrix;

    static const int FACE_NUMBERS = 6;
    static const std::vector<GLfloat> verticlesData;
    static const std::vector<GLushort> indices;

    std::array<glm::vec3, FACE_NUMBERS * 4> m_vertex;
    std::array<GLushort, FACE_NUMBERS * 6> m_indeces;
};

