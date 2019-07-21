package com.example.rubikcube

import android.app.Activity
import android.content.res.AssetManager
import android.os.Bundle
import android.widget.LinearLayout


class MainActivity : Activity() {
    init {
        System.loadLibrary("native-lib")
    }

    private lateinit var mView: GLESView

    override fun onCreate(icicle: Bundle?) {
        super.onCreate(icicle)
        initAssetManager(getAssets())
        setContentView(R.layout.activity_main)
        mView = GLESView(application)

        val layoutMain = findViewById(R.id.linear_layout) as LinearLayout
        layoutMain.addView(mView)
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
