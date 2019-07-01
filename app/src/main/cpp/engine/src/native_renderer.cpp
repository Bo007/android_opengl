#include "engine/include/native_renderer.h"

#include <vector>
#include <string>
#include <chrono>

#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "logger.h"

#include "engine/include/engine_utils.h"

const std::vector<GLfloat> NativeRenderer::verticlesData =
        {
                1.0F, 0.0F, 1.0F,
                1.0F, 0.0F, 0.0F,
                1.0F, 1.0F, 1.0F,
                1.0F, 1.0F, 0.0F,

                0.0F, 0.0F, 1.0F,
                0.0F, 0.0F, 0.0F,
                0.0F, 1.0F, 1.0F,
                0.0F, 1.0F, 0.0F,


                1.0F, 0.0F, 0.0F,
                1.0F, 0.0F, 1.0F,
                0.0F, 0.0F, 0.0F,
                0.0F, 0.0F, 1.0F,

                1.0F, 1.0F, 0.0F,
                1.0F, 1.0F, 1.0F,
                0.0F, 1.0F, 0.0F,
                0.0F, 1.0F, 1.0F,


                0.0F, 1.0F, 0.0F,
                0.0F, 0.0F, 0.0F,
                1.0F, 1.0F, 0.0F,
                1.0F, 0.0F, 0.0F,

                0.0F, 1.0F, 1.0F,
                0.0F, 0.0F, 1.0F,
                1.0F, 1.0F, 1.0F,
                1.0F, 0.0F, 1.0F,
        };

const std::vector<GLushort> NativeRenderer::indices = {
        0, 1, 2, 2, 1, 3,
        4, 5, 6, 6, 5, 7,

        8, 9, 10, 10, 9, 11,
        12, 13, 14, 14, 13, 15,

        16, 17, 18, 18, 17, 19,
        20, 21, 22, 22, 21, 23,
};

static const GLchar *VERTEX_SHADER = R"glsl(
precision mediump float;

attribute vec3 aPosition;

varying vec3 vColors;

uniform mat4 uMvp;

void main(void)
{
    gl_Position = uMvp * vec4( aPosition, 1.0 );
    vColors = aPosition;
}

)glsl";

static const GLchar *FRAGMENT_SHADER = R"glsl(
precision mediump float;

varying vec3 vColors;

void main(void)
{
    gl_FragColor = vec4( vColors, 1.0 );
}

)glsl";

NativeRenderer::NativeRenderer() {
    bool res = engine_utils::BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);
    m_screenWidth = m_screenHeight = 0;

    m_posititonCoordinateHandle = glGetAttribLocation(m_programID, "aPosition");

    m_mvpHandle = glGetUniformLocation(m_programID, "uMvp");
}

NativeRenderer::~NativeRenderer() {
    glDeleteProgram(m_programID);
}

void NativeRenderer::changed(int width, int height) {
    assert(width > 0 && height > 0);
    m_screenWidth = width;
    m_screenHeight = height;

}

void NativeRenderer::render() {
    glUseProgram(m_programID);

    if (m_posititonCoordinateHandle != -1) {

        glVertexAttribPointer(m_posititonCoordinateHandle,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              NativeRenderer::verticlesData.data());

        glEnableVertexAttribArray(m_posititonCoordinateHandle);
    }

    auto mvp = getMvpMatrix();
    glUniformMatrix4fv(m_mvpHandle, 1, GL_FALSE, glm::value_ptr(mvp));

    glViewport(0, 0, m_screenWidth, m_screenHeight);

    glClearColor(0.3F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, NativeRenderer::indices.size(), GL_UNSIGNED_SHORT, NativeRenderer::indices.data());

    if (m_posititonCoordinateHandle != -1)
        glDisableVertexAttribArray(m_posititonCoordinateHandle);

    engine_utils::CheckGLError("GLSimpleRenderer::draw");
}

glm::mat4x4 NativeRenderer::getMvpMatrix() const {

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis1 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    auto millis2 = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 10000 * 10000;
    auto currentMs = (millis1 - millis2) / 10000.0F;

    float angle = currentMs * glm::radians(360.0);  // base 15° per second
    LOGI("angle = %f", angle);
    glm::mat4 anim =
            glm::rotate(glm::mat4(1.0f), angle * 3.0f, glm::vec3(1, 0, 0)) *  // X axis
            glm::rotate(glm::mat4(1.0f), angle * 2.0f, glm::vec3(0, 1, 0)) *  // Y axis
            glm::rotate(glm::mat4(1.0f), angle * 4.0f, glm::vec3(0, 0, 1));   // Z axis

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f * m_screenWidth / m_screenHeight, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model * anim;

    return mvp;
}