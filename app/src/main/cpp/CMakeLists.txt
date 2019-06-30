cmake_minimum_required(VERSION 3.4.1)

add_library(
        native-lib
        SHARED
        native-lib.cpp)

target_include_directories(
        native-lib
        PRIVATE
        thirdparty/include
        jni_wrapper/include)

target_link_libraries(
        native-lib

        GLESv3
        android
        EGL
        log
        m)