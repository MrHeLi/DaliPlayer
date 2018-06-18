package com.dali.daliplayer;

public class FFmpegJni {
    static {
<<<<<<< HEAD
=======
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
>>>>>>> 5940151616ee17a59b2a690a443d061717f76f9e
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
<<<<<<< HEAD
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
=======
>>>>>>> 5940151616ee17a59b2a690a443d061717f76f9e
        System.loadLibrary("swscale-4");
        System.loadLibrary("myplayer");
    }
    public static native void play(String path);

    public static native void startPlay(String url, Object surface);
}
