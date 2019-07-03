package com.example.rubikcube


// Wrapper for native library
object NativeRenderer {

    init {
        System.loadLibrary("native-lib")
    }

    external fun init()
    external fun resize(width: Int, height: Int)
    external fun render()
}
