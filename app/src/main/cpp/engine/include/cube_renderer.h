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
    GLint m_colorHandle;

    static const std::vector<GLfloat> verticlesDatas[];
    static const glm::vec3 flatColors[];
    static const std::vector<GLushort> indices;

    glm::mat4 m_mvpMatrix;
};

