#include "engine/include/flat_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/include/engine_utils.h"

const std::vector<GLfloat> FlatRenderer::verticlesData =
        {
                -0.5F, 0.5F,
                -0.5F, -0.5F,
                0.5F, 0.5F,
                0.5F, -0.5F,
        };

const std::vector<GLushort> FlatRenderer::indices = {
        0, 1, 2, 2, 1, 3
};

static const GLchar *VERTEX_SHADER = R"glsl(
precision mediump float;

attribute vec3 aPosition;

varying vec3 vColor;

uniform mat4 uMvp;

void main(void)
{
    gl_Position = uMvp * vec4( aPosition, 1.0 );
    vColor = aPosition;
}

)glsl";

static const GLchar *FRAGMENT_SHADER = R"glsl(
precision mediump float;

varying vec3 vColor;

void main(void)
{
    gl_FragColor = vec4( vColor, 1.0 );
}

)glsl";

FlatRenderer::FlatRenderer() {
    bool res = engine_utils::BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);

    m_posititonCoordinateHandle = glGetAttribLocation(m_programID, "aPosition");

    m_mvpHandle = glGetUniformLocation(m_programID, "uMvp");

    m_mvpMatrix = glm::mat4x4(1);

    for (int i = 0; i < verticlesData.size() / 2; ++i) {
        int firstShift = 0;
        int secondShift = 4;
        m_vertex[i + firstShift].x = -0.5F;
        m_vertex[i + secondShift].x = 0.5F;
        m_vertex[i + firstShift].y = m_vertex[i + secondShift].y = verticlesData[2 * i];
        m_vertex[i + firstShift].z = m_vertex[i + secondShift].z = verticlesData[2 * i + 1];

        firstShift = 8;
        secondShift = 12;
        m_vertex[i + firstShift].x = m_vertex[i + secondShift].x = verticlesData[2 * i];
        m_vertex[i + firstShift].y = -0.5F;
        m_vertex[i + secondShift].y = 0.5F;
        m_vertex[i + firstShift].z = m_vertex[i + secondShift].z = verticlesData[2 * i + 1];

        firstShift = 16;
        secondShift = 20;
        m_vertex[i + firstShift].x = m_vertex[i + secondShift].x = verticlesData[2 * i];
        m_vertex[i + firstShift].y = m_vertex[i + secondShift].y = verticlesData[2 * i + 1];
        m_vertex[i + firstShift].z = -0.5F;
        m_vertex[i + secondShift].z = 0.5F;
    }

    for (int i = 0; i < FACE_NUMBERS; ++i) {
        for (int j = 0; j < FlatRenderer::indices.size(); ++j) {
            m_indeces[i * FACE_NUMBERS + j] = i * FlatRenderer::verticlesData.size() / 2 + FlatRenderer::indices[j];
        }
    }
}

FlatRenderer::~FlatRenderer() {
    glDeleteProgram(m_programID);
}

void FlatRenderer::render() {
    glUseProgram(m_programID);

    if (-1 != m_mvpHandle) {
        glUniformMatrix4fv(m_mvpHandle, 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));
    }

    if (-1 != m_posititonCoordinateHandle) {

        glVertexAttribPointer(m_posititonCoordinateHandle,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              m_vertex.data());

        glEnableVertexAttribArray(m_posititonCoordinateHandle);
    }

    glDrawElements(GL_TRIANGLES, m_indeces.size(), GL_UNSIGNED_SHORT, m_indeces.data());

    if (m_posititonCoordinateHandle != -1)
        glDisableVertexAttribArray(m_posititonCoordinateHandle);

    engine_utils::CheckGLError("CubeRenderer::render");
}

void FlatRenderer::setMvpMatrix(const glm::mat4 &mvpMatrix) {
    m_mvpMatrix = mvpMatrix;
}
