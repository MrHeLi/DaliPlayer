package com.dali.daliplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;

public class XPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback{

    private String URL;

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(this.getClass().getSimpleName(), "XPlayer structure methos time = "
                + SystemClock.currentThreadTimeMillis());
    }

    @Override
    public void run() {
        Log.i("TAG", "run time = " + SystemClock.currentThreadTimeMillis());
        FFmpegJni.startPlay(URL, getHolder().getSurface());
    }


    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.i("TAG", "surfaceChanged time = " + SystemClock.currentThreadTimeMillis());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i("TAG", "surfaceCreated time = " + SystemClock.currentThreadTimeMillis());
        new Thread( this ).start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i("TAG", "surfaceDestroyed time = " + SystemClock.currentThreadTimeMillis());
    }

    public void setURL(String URL) {
        this.URL = URL;
    }
}
