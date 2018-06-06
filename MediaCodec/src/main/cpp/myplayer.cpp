#include <jni.h>
#include <string>
#include "android/log.h"
extern "C" {
//#include "libavcodec/avcodec.h"
//#include <libavformat/avformat.h>
}

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ffmpeg jni", __VA_ARGS__);


extern "C" JNIEXPORT jstring

JNICALL
Java_com_dali_daliplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_dali_daliplayer_FFmpegJni_play(JNIEnv *env, jclass type, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    // TODO
    LOGI("path = %s", path);
//    av_register_all();
//    avcodec_register_all();
    env->ReleaseStringUTFChars(path_, path);
}