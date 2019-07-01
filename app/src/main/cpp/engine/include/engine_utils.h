#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <GLES3/gl3.h>

namespace engine_utils {
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
        GLuint vertexShaderId;
        if (!BuildShader(vertShaderSource, vertexShaderId, GL_VERTEX_SHADER)) {
            LOGE("failed to build vertex shader");
            return false;
        }
        GLuint fragmentShaderId;
        if (!BuildShader(fragShaderSource, fragmentShaderId, GL_FRAGMENT_SHADER)) {
            LOGE("failed to build fragment shader");
            return false;
        }

        programObject = glCreateProgram();
        glAttachShader(programObject, vertexShaderId);
        glAttachShader(programObject, fragmentShaderId);
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

    void CheckGLError(const char *label) {
        int gl_error = glGetError();
        if (gl_error != GL_NO_ERROR) {
            LOGE("GL error @ %s: %d", label, gl_error);
            // Crash immediately to make OpenGL errors obvious.
            abort();
        }
    }
}

