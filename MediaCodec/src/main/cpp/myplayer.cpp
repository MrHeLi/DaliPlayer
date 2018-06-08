#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"
{
#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
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
        LOGW("%s open failed!" , path);
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

    //查找解碼器
    //audio
    AVCodec *audioCodec = avcodec_find_decoder(formatCtx->streams[audioStream]->codecpar->codec_id);
    if (!audioCodec) {
        LOGW("cannot find specified audio codec");
        return;
    }
    AVCodecContext *audioCodecContext = avcodec_alloc_context3(audioCodec);
    int openCodecError = 0;
    if ((openCodecError = avcodec_open2(audioCodecContext, audioCodec, NULL)) < 0) {
        LOGW("cannot open specified audio codec");
    }

    //video
    AVCodec *videoCodec = avcodec_find_decoder(formatCtx->streams[videoStream]->codecpar->codec_id);
    if (!videoCodec) {
        LOGW("cannot find specified video codec");
    }
    AVCodecContext *videoCodecContext = avcodec_alloc_context3(videoCodec);
    if ((openCodecError = avcodec_open2(videoCodecContext, videoCodec, NULL)) < 0) {
        LOGW("cannot open specified video codec");
    }
    //分配解码后的数据存储位置
    AVPacket* avPacket = av_packet_alloc();
    AVFrame* avFrame = av_frame_alloc();
    for (;;) {
        int re = av_read_frame(formatCtx, avPacket);
        if (re != 0) {
            LOGW("EOF")
//            av_seek_frame(formatCtx, videoStream,  formatCtx->duration/2,
//                          AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
            break;
        }
        if (avPacket->stream_index != videoStream) {
            continue;
        }
        LOGI("Stream %d, size = %d, pts = %d, flag = %d",
        avPacket->stream_index, avPacket->size, avPacket->pts, avPacket->flags);
        re = avcodec_send_packet(videoCodecContext, avPacket);
        av_packet_unref(avPacket);
        if (re != 0) {
            LOGW("packet send error");
        }
        re = avcodec_receive_frame(videoCodecContext, avFrame);
        if (re != 0) {
            LOGW("frame receive error");
        }
        LOGI("avcodec_receive_frame fps = %lld", avFrame->pts);

    }
    //音频
    //视频


    avformat_close_input(&formatCtx);
    env->ReleaseStringUTFChars(path_, path);
}