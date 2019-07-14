package com.example.rubikcube

import android.app.Activity
import android.content.res.AssetManager
import android.os.Bundle
import android.view.ViewGroup
import android.view.ViewGroup.LayoutParams.FILL_PARENT
import android.view.ViewGroup.LayoutParams.MATCH_PARENT
import android.widget.RelativeLayout
import kotlinx.android.synthetic.main.activity_main.view.*

class MainActivity : Activity() {
    init {
        System.loadLibrary("native-lib")
    }

    internal lateinit var mView: GLESView

    override fun onCreate(icicle: Bundle?) {
        super.onCreate(icicle)
        initAssetManager(getAssets())
        mView = GLESView(application)
        setContentView(R.layout.activity_main)
        addContentView(mView, RelativeLayout.LayoutParams(300, 300))
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
