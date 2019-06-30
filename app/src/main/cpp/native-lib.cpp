#include <jni.h>
#include <string>

#include <android/asset_manager_jni.h>

#include <GLES3/gl3.h>

#include "jni_resource_reader.h"

extern "C"
{
// main activity methods
//    -----------------------------------

#define MAIN_ACTIVITY_METHOD(Type, funcName) \
JNIEXPORT Type JNICALL \
Java_com_example_myfirstgame_MainActivity_##funcName

MAIN_ACTIVITY_METHOD(void, initAssetManager)
(JNIEnv *env, jobject,
 jobject assetManager) {
    // use asset manager to open asset by filename
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    assert(nullptr != mgr);
    JniResourceReader::InitAssetManager(mgr);
}

// renderer methods
//    -----------------------------------
#define NATIVE_RENDERER_METHOD(Type, funcName) \
JNIEXPORT Type JNICALL \
Java_com_example_myfirstgame_NativeRenderer_##funcName

NATIVE_RENDERER_METHOD(void, init)
(JNIEnv *, jobject) {
}

NATIVE_RENDERER_METHOD(void, resize)
(JNIEnv *, jobject,
 jint width, jint height) {
}

NATIVE_RENDERER_METHOD(void, render)
(JNIEnv *, jobject) {
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

}