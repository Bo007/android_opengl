#include "engine/include/cube_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/include/engine_utils.h"

const std::vector<GLfloat> CubeRenderer::verticlesDatas[] =
        {
                {
                        1.0F, 0.0F, 1.0F,
                        1.0F, 0.0F, 0.0F,
                        1.0F, 1.0F, 1.0F,
                        1.0F, 1.0F, 0.0F,
                },

                {
                        0.0F, 0.0F, 1.0F,
                        0.0F, 0.0F, 0.0F,
                        0.0F, 1.0F, 1.0F,
                        0.0F, 1.0F, 0.0F,
                },

                {
                        1.0F, 0.0F, 0.0F,
                        1.0F, 0.0F, 1.0F,
                        0.0F, 0.0F, 0.0F,
                        0.0F, 0.0F, 1.0F,
                },

                {
                        1.0F, 1.0F, 0.0F,
                        1.0F, 1.0F, 1.0F,
                        0.0F, 1.0F, 0.0F,
                        0.0F, 1.0F, 1.0F,
                },

                {
                        0.0F, 1.0F, 0.0F,
                        0.0F, 0.0F, 0.0F,
                        1.0F, 1.0F, 0.0F,
                        1.0F, 0.0F, 0.0F,
                },

                {
                        0.0F, 1.0F, 1.0F,
                        0.0F, 0.0F, 1.0F,
                        1.0F, 1.0F, 1.0F,
                        1.0F, 0.0F, 1.0F,
                },
        };


const glm::vec3 CubeRenderer::flatColors[] = {
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),

        glm::vec3(0, 0, 1),
        glm::vec3(1, 1, 0),

        glm::vec3(0, 1, 1),
        glm::vec3(1, 1, 1)
};

const std::vector<GLushort> CubeRenderer::indices = {
        0, 1, 2, 2, 1, 3
};

static const GLchar *VERTEX_SHADER = R"glsl(
precision mediump float;

attribute vec3 aPosition;

uniform mat4 uMvp;

void main(void)
{
    gl_Position = uMvp * vec4( aPosition, 1.0 );
}

)glsl";

static const GLchar *FRAGMENT_SHADER = R"glsl(
precision mediump float;

uniform vec3 uColor;

void main(void)
{
    gl_FragColor = vec4( uColor, 1.0 );
}

)glsl";

CubeRenderer::CubeRenderer() {
    bool res = engine_utils::BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);

    m_posititonCoordinateHandle = glGetAttribLocation(m_programID, "aPosition");

    m_mvpHandle = glGetUniformLocation(m_programID, "uMvp");

    m_colorHandle = glGetUniformLocation(m_programID, "uColor");

    m_mvpMatrix = glm::mat4x4(1);
}

CubeRenderer::~CubeRenderer() {
    glDeleteProgram(m_programID);
}

void CubeRenderer::render() {
    glUseProgram(m_programID);

    if (-1 != m_mvpHandle) {
        glUniformMatrix4fv(m_mvpHandle, 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));
    }

    for (int i = 0; i < 6; ++i) {
        if (-1 != m_posititonCoordinateHandle) {

            glVertexAttribPointer(m_posititonCoordinateHandle,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0,
                                  CubeRenderer::verticlesDatas[i].data());

            glEnableVertexAttribArray(m_posititonCoordinateHandle);
        }

        if (-1 != m_colorHandle) {
            auto color = CubeRenderer::flatColors[i];
            glUniform3fv(m_colorHandle, 1, glm::value_ptr(color));
        }

        glDrawElements(GL_TRIANGLES, CubeRenderer::indices.size(), GL_UNSIGNED_SHORT, CubeRenderer::indices.data());

        if (m_posititonCoordinateHandle != -1)
            glDisableVertexAttribArray(m_posititonCoordinateHandle);

        engine_utils::CheckGLError("CubeRenderer::render");
    }
}

void CubeRenderer::setMvpMatrix(const glm::mat4 &mvpMatrix) {
    m_mvpMatrix = mvpMatrix;
}