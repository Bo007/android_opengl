cmake_minimum_required(VERSION 3.4.1)

add_library(
        native-lib
        SHARED
        native-lib.cpp
        engine/src/native_renderer.cpp
        engine/src/cube_renderer.cpp
)

target_include_directories(
        native-lib
        PRIVATE
        thirdparty/include
        jni_wrapper/include
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
        native-lib

        GLESv3
        android
        EGL
        log
        m)