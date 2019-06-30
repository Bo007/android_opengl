#pragma once

#include <GLES3/gl3.h>

class NativeRenderer {
public:
    NativeRenderer();

    ~NativeRenderer();

    void changed(int width, int height);

    void render();

private:
    GLuint m_programID;

    int m_screenWidth;
    int m_screenHeight;

};

