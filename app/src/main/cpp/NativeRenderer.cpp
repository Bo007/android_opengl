#include "NativeRenderer.h"

#include <vector>
#include <string>

#include <GLES3/gl3.h>

#include "logger.h"

static const GLchar *VERTEX_SHADER = R"glsl(
attribute vec2 texcoord;
varying vec2 Texcoord;
void main()
{
    gl_Position = vec4( texcoord * 2.0 - 1.0, 0.0, 1.0 );
    Texcoord = texcoord;
}
)glsl";

static const GLchar *FRAGMENT_SHADER = R"glsl(
precision mediump float;

varying vec2 Texcoord;

void main()
{
    gl_FragColor = vec4( Texcoord, 0.0, 1.0 );
}
)glsl";

static bool BuildShader(std::string const &shader_source, GLuint &shader, GLenum type) {
    int size = static_cast<int>(shader_source.length());

    shader = glCreateShader(type);

    if (shader == 0)
        return false;

    char const *c_shader_source = shader_source.c_str();
    glShaderSource(shader, 1, &c_shader_source, &size);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        LOGI("failed to compile shader");
        int length = 1024;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<unsigned long>(length));
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        LOGE("%s", &log[0]);
        return false;
    }

    return true;
}

static bool BuildProgram(const std::string &vertShaderSource,
                         const std::string &fragShaderSource,
                         GLuint &programObject) {
    GLuint v_shader, f_shader;
    if (!BuildShader(vertShaderSource, v_shader, GL_VERTEX_SHADER)) {
        LOGE("failed to build vertex shader");
        return false;
    }

    if (!BuildShader(fragShaderSource, f_shader, GL_FRAGMENT_SHADER)) {
        LOGE("failed to build fragment shader");
        return false;
    }

    programObject = glCreateProgram();
    glAttachShader(programObject, v_shader);
    glAttachShader(programObject, f_shader);
    glLinkProgram(programObject);
    GLint status;
    glGetProgramiv(programObject, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        LOGE("failed to link program");
        int length;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(programObject, length, &length, &log[0]);
        LOGE("%s", &log[0]);
        return false;
    }

    return true;
}

NativeRenderer::NativeRenderer() {
    bool res = BuildProgram(VERTEX_SHADER, FRAGMENT_SHADER, m_programID);
    assert(res);
    m_screenWidth = m_screenHeight = 0;
}

NativeRenderer::~NativeRenderer() {
    glDeleteProgram(m_programID);
}

void NativeRenderer::changed(int width, int height) {
    assert(width >= 0 || height >= 0);
    m_screenWidth = width;
    m_screenHeight = height;
}

void NativeRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float color = 0;
    static bool direction = true;
    static float step = 0.01f;

    if (direction)
        color += step;
    else
        color -= step;

    if (color > 1)
        direction = false;
    else if (color < 0)
        direction = true;

    glClearColor(color, color, color, 1.0F);
}