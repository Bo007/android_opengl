#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

class NativeRenderer {
public:
    NativeRenderer();

    ~NativeRenderer();

    void changed(int width, int height);

    void render();

private:
    glm::mat4x4 getMvpMatrix() const;
private:
    GLuint m_programID;

    GLint m_posititonCoordinateHandle;
    GLint m_mvpHandle;

    int m_screenWidth;
    int m_screenHeight;

    static const std::vector<GLfloat> verticlesData;
    static const std::vector<GLushort> indices;
};

