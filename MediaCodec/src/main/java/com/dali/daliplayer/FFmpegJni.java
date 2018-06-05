package com.dali.daliplayer;

public class FFmpegJni {
    static {
        System.loadLibrary("myplayer");
    }
    public static native void play(String path);
}
