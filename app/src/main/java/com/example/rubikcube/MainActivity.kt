package com.example.rubikcube

import android.app.Activity
import android.content.res.AssetManager
import android.os.Bundle
import android.widget.LinearLayout
import android.widget.Button
import android.view.View


class MainActivity : Activity(), View.OnClickListener {
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

        val mClickButton1 = findViewById(R.id.clickButton1) as Button
        mClickButton1.setOnClickListener(this)
        val mClickButton2 = findViewById(R.id.clickButton2) as Button
        mClickButton2.setOnClickListener(this)
        val mClickButton3 = findViewById(R.id.clickButton3) as Button
        mClickButton3.setOnClickListener(this)
        val mClickButton4 = findViewById(R.id.clickButton4) as Button
        mClickButton4.setOnClickListener(this)
        val mClickButton5 = findViewById(R.id.clickButton5) as Button
        mClickButton5.setOnClickListener(this)
        val mClickButton6 = findViewById(R.id.clickButton6) as Button
        mClickButton6.setOnClickListener(this)
    }

    override fun onPause() {
        super.onPause()
        mView.onPause()
    }

    override fun onResume() {
        super.onResume()
        mView.onResume()
    }

    override fun onClick(v: View) {
        var faceId: Int = 0
        when (v.getId()) {
            R.id.clickButton1 -> {
                faceId = 0
            }
            R.id.clickButton2 -> {
                faceId = 1
            }
            R.id.clickButton3 -> {
                faceId = 2
            }
            R.id.clickButton4 -> {
                faceId = 3
            }
            R.id.clickButton5 -> {
                faceId = 4
            }
            R.id.clickButton6 -> {
                faceId = 5
            }
        }
        mView.rotateFace(faceId)
    }

    external fun initAssetManager(assetManager: AssetManager)
}
