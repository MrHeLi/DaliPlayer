#include <jni.h>
#include <string>
#include "android/log.h"

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
    env->ReleaseStringUTFChars(path_, path);
}