#pragma once#include <string>#include <android/asset_manager_jni.h>class JniResourceReader {public:    static void InitAssetManager(AAssetManager *assetManager) {        m_assetManager = assetManager;    }private:    static AAssetManager *m_assetManager;};