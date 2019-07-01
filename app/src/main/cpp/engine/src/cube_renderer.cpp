#include "engine/include/cube_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/include/engine_utils.h"

const std::vector<GLfloat> CubeRenderer::verticlesData =
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

const std::vector<GLushort> CubeRenderer::indices = {
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

CubeRenderer::CubeRenderer() {
    bool res = engine_utils::BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);

    m_posititonCoordinateHandle = glGetAttribLocation(m_programID, "aPosition");

    m_mvpHandle = glGetUniformLocation(m_programID, "uMvp");

    m_mvpMatrix = glm::mat4x4(1);
}

CubeRenderer::~CubeRenderer() {
    glDeleteProgram(m_programID);
}

void CubeRenderer::render() {
    glUseProgram(m_programID);

    if (m_posititonCoordinateHandle != -1) {

        glVertexAttribPointer(m_posititonCoordinateHandle,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              CubeRenderer::verticlesData.data());

        glEnableVertexAttribArray(m_posititonCoordinateHandle);
    }

    glUniformMatrix4fv(m_mvpHandle, 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));

    glDrawElements(GL_TRIANGLES, CubeRenderer::indices.size(), GL_UNSIGNED_SHORT, CubeRenderer::indices.data());

    if (m_posititonCoordinateHandle != -1)
        glDisableVertexAttribArray(m_posititonCoordinateHandle);

    engine_utils::CheckGLError("GLSimpleRenderer::draw");
}

void CubeRenderer::setMvpMatrix(const glm::mat4 &mvpMatrix) {
    m_mvpMatrix = mvpMatrix;
}