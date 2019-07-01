#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

class CubeRenderer {
public:
    CubeRenderer();

    ~CubeRenderer();

    void render();

    void setMvpMatrix(const glm::mat4 &mvpMatrix);

private:

    GLuint m_programID;

    GLint m_posititonCoordinateHandle;
    GLint m_mvpHandle;

    static const std::vector<GLfloat> verticlesData;
    static const std::vector<GLushort> indices;

    glm::mat4 m_mvpMatrix;
};

