#include <jni.h>
#include <string>

#include <android/asset_manager_jni.h>

#include <GLES3/gl3.h>

#include "jni_resource_reader.h"

#include "engine/include/native_renderer.h"

extern "C"
{
// main activity methods
//    -----------------------------------

#define MAIN_ACTIVITY_METHOD(Type, funcName) \
JNIEXPORT Type JNICALL \
Java_com_example_rubikcube_MainActivity_##funcName

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

std::unique_ptr<NativeRenderer> g_renderer;
#define NATIVE_RENDERER_METHOD(Type, funcName) \
JNIEXPORT Type JNICALL \
Java_com_example_rubikcube_NativeRenderer_##funcName

NATIVE_RENDERER_METHOD(void, init)
(JNIEnv *, jobject) {
    g_renderer = std::make_unique<NativeRenderer>();
}

NATIVE_RENDERER_METHOD(void, resize)
(JNIEnv *, jobject,
 jint width, jint height) {
    g_renderer->changed(width, height);
}

NATIVE_RENDERER_METHOD(void, render)
(JNIEnv *, jobject) {
    g_renderer->render();
}

}