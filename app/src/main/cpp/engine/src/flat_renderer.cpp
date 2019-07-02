#include "engine/include/flat_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "engine/include/engine_utils.h"


const std::array<GLushort, 6> FlatRenderer::VERTEX_INDICES = {
        0, 1, 2, 2, 1, 3
};

static const GLchar *VERTEX_SHADER = R"glsl(
precision mediump float;

attribute vec4 aPosition;

//uniform mat4 uMvp;

void main(void)
{
    gl_Position = vec4( aPosition );
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

FlatRenderer::FlatRenderer(float minCubeSize, float maxCubeSize) {
    bool res = engine_utils::BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);

    m_posititonCoordinateHandle = glGetAttribLocation(m_programID, "aPosition");

    m_mvpHandle = glGetUniformLocation(m_programID, "uMvp");

    m_colorHandle = glGetUniformLocation(m_programID, "uColor");

    m_mvpMatrix = glm::mat4x4(1);


    const std::vector<GLfloat> vertexData =
            {
                    minCubeSize, maxCubeSize,
                    minCubeSize, minCubeSize,
                    maxCubeSize, maxCubeSize,
                    maxCubeSize, minCubeSize,
            };

    for (int i = 0; i < vertexData.size() / 2; ++i) {
        int firstShift = 0;
        int secondShift = 4;
        m_vertex[i + firstShift].x = minCubeSize;
        m_vertex[i + secondShift].x = maxCubeSize;
        m_vertex[i + firstShift].y = m_vertex[i + secondShift].y = FACE_KOEF * vertexData[2 * i];
        m_vertex[i + firstShift].z = m_vertex[i + secondShift].z = FACE_KOEF * vertexData[2 * i + 1];

        firstShift = 8;
        secondShift = 12;
        m_vertex[i + firstShift] = m_vertex[i + firstShift - vertexData.size()];
        m_vertex[i + secondShift] = m_vertex[i + secondShift - vertexData.size()];

        std::swap(m_vertex[i + firstShift].x, m_vertex[i + firstShift].y);
        std::swap(m_vertex[i + secondShift].x, m_vertex[i + secondShift].y);

        firstShift = 16;
        secondShift = 20;
        m_vertex[i + firstShift] = m_vertex[i + firstShift - vertexData.size()];
        m_vertex[i + secondShift] = m_vertex[i + secondShift - vertexData.size()];

        std::swap(m_vertex[i + firstShift].z, m_vertex[i + firstShift].y);
        std::swap(m_vertex[i + secondShift].z, m_vertex[i + secondShift].y);
    }

    m_facesColors = {glm::vec3(1, 0, 0),
                     glm::vec3(0, 1, 0),
                     glm::vec3(0, 0, 1),

                     glm::vec3(0, 1, 1),
                     glm::vec3(1, 0, 1),
                     glm::vec3(1, 1, 0),};
}

FlatRenderer::~FlatRenderer() {
    glDeleteProgram(m_programID);
}

void FlatRenderer::render() {
    glUseProgram(m_programID);

    if (-1 != m_mvpHandle) {
        glUniformMatrix4fv(m_mvpHandle, 1, GL_FALSE, glm::value_ptr(m_mvpMatrix));
    }

    for (int i = 0; i < FlatRenderer::FACE_NUMBERS; ++i) {
        if (-1 != m_posititonCoordinateHandle) {

            glVertexAttribPointer(m_posititonCoordinateHandle,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0,
                                  m_rotatedVertex.data() + i * 4);

            glEnableVertexAttribArray(m_posititonCoordinateHandle);
        }

        if (-1 != m_colorHandle) {
            glUniform3fv(m_colorHandle, 1, glm::value_ptr(m_facesColors[i]));

        }


        glDrawElements(GL_TRIANGLES, VERTEX_INDICES.size(), GL_UNSIGNED_SHORT, VERTEX_INDICES.data());
    }

    if (m_posititonCoordinateHandle != -1)
        glDisableVertexAttribArray(m_posititonCoordinateHandle);

    engine_utils::CheckGLError("CubeRenderer::render");
}

void FlatRenderer::setMvpMatrix(const glm::mat4 &mvpMatrix, const glm::vec3 &translateVec) {
    m_mvpMatrix = mvpMatrix;

    for (int i = 0; i < m_rotatedVertex.size(); ++i) {
        glm::vec4 movedVertex;
        movedVertex.x = m_vertex[i].x + translateVec.x;
        movedVertex.y = m_vertex[i].y + translateVec.y;
        movedVertex.z = m_vertex[i].z + translateVec.z;
        movedVertex.w = 1;

        m_rotatedVertex[i] = mvpMatrix * movedVertex;
    }
}
