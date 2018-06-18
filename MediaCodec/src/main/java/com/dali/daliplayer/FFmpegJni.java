package com.dali.daliplayer;

public class FFmpegJni {
    static {
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("swscale-4");
        System.loadLibrary("myplayer");
    }
    public static native void play(String path);

    public static native void startPlay(String url, Object surface);
}
