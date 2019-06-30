package com.example.myfirstgame

import android.app.Activity
import android.content.res.AssetManager
import android.os.Bundle

class MainActivity : Activity() {
    init {
        System.loadLibrary("native-lib")
    }

    internal lateinit var mView: GLESView

    override fun onCreate(icicle: Bundle?) {
        super.onCreate(icicle)
        initAssetManager(getAssets())
        mView = GLESView(application)
        setContentView(mView)
    }

    override fun onPause() {
        super.onPause()
        mView.onPause()
    }

    override fun onResume() {
        super.onResume()
        mView.onResume()
    }

    external fun initAssetManager(assetManager: AssetManager)
}
