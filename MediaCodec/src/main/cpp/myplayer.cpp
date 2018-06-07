#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"
{
#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
}

#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "ffmpeg jni", __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ffmpeg jni", __VA_ARGS__);

static double r2d(AVRational rational) {
    return rational.den == 0 || rational.num == 0 ? 0 : (double)rational.num/(double)rational.den;
}
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
    avformat_network_init();//注册网络协议
    av_register_all();//注册格式，编解码器
    AVFormatContext *formatCtx = avformat_alloc_context();
    int result = avformat_open_input(&formatCtx, path, NULL, NULL);
    if (result != 0) {
        LOGW("%s open failde!" , path);
        return;
    }
    LOGI("duration = %lld, streamCount = %d", formatCtx->duration, formatCtx->nb_streams);
    int videoStream = -1;
    int audioStream = -1;
    for (int i = 0; i < formatCtx->nb_streams; i ++) {
        AVStream* stream = formatCtx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            LOGI("stream %d 是一个视频", i);
            videoStream = i;
            LOGI("fps = %d, width = %d, height = %d, codec_id = %d",
                 stream->avg_frame_rate,
                 stream->codecpar->width,
                 stream->codecpar->height,
                 stream->codecpar->codec_id);
        } else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            LOGI("stream %d 是一个音频", i);
            audioStream = i;
            LOGI("sample_rate = %d, channels = %d, format = %d",
                 stream->codecpar->sample_rate,
                 stream->codecpar->channels,
                 stream->codecpar->format);
        } else if (stream->codecpar->codec_type ==AVMEDIA_TYPE_SUBTITLE) {
            LOGI("stream %d 是一个字幕", i);
        }
    }

    avformat_close_input(&formatCtx);
    env->ReleaseStringUTFChars(path_, path);
}