#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/jni.h>
}

#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "ffmpeg jni", __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ffmpeg jni", __VA_ARGS__);

static double r2d(AVRational r)
{
    return r.num==0||r.den == 0 ? 0 :(double)r.num/(double)r.den;
}

long long getCurrentMs() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int sec = tv.tv_sec%360000;
    long long t = sec*1000+tv.tv_usec/1000;
    return t;
}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res)
{
    av_jni_set_java_vm(vm,0);
    return JNI_VERSION_1_4;
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
    avcodec_register_all();
    int result = avformat_open_input(&formatCtx, path, NULL, NULL);
    if (result != 0) {
        LOGW("%s open failed!" , path);
        return;
    }
    //获取流信息
    int re = avformat_find_stream_info(formatCtx,0);//如果不执行这个函数，一些原数据取不到
    if(re != 0)
    {
        LOGW("avformat_find_stream_info failed!");
    }
    LOGI("duration = %lld, streamCount = %d", formatCtx->duration, formatCtx->nb_streams);
    int videoStream = -1;
    int audioStream = -1;
    for (int i = 0; i < formatCtx->nb_streams; i ++) {
        AVStream* stream = formatCtx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            LOGI("stream %d 是一个视频", i);
            videoStream = i;
            int fps = r2d(stream->avg_frame_rate);
            LOGI("fps = %d, width = %d, height = %d, codec_id = %d",
                 fps,
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
    avcodec_parameters_to_context(audioCodecContext,formatCtx->streams[audioStream]->codecpar);
    audioCodecContext->thread_count = 8;
    int openCodecError = 0;
    if ((openCodecError = avcodec_open2(audioCodecContext, audioCodec, NULL)) < 0) {
        LOGW("cannot open specified audio codec");
    }

    //video
    AVCodec *videoCodec = avcodec_find_decoder(formatCtx->streams[videoStream]->codecpar->codec_id);
    //硬解码
//    videoCodec = avcodec_find_decoder_by_name("h264_mediacodec");
    if (!videoCodec) {
        LOGW("cannot find specified video codec");
    }
    AVCodecContext *videoCodecContext = avcodec_alloc_context3(videoCodec);
    avcodec_parameters_to_context(videoCodecContext,formatCtx->streams[videoStream]->codecpar);
    videoCodecContext->thread_count = 8;
    if ((openCodecError = avcodec_open2(videoCodecContext, videoCodec, NULL)) < 0) {
        LOGW("cannot open specified video codec");
    }
    //分配解码后的数据存储位置
    AVPacket* avPacket = av_packet_alloc();
    AVFrame* avFrame = av_frame_alloc();
    long long start = getCurrentMs();
    int frameCount = 0;

    //初始化像素格式上下文
    SwsContext *vCtx = NULL;
    int outWidth = 720;
    int outHeight = 1280;
    char *rgb = new char[1920*1080*4];
    char *pcm = new char[48000*4*2];

    //音频重采样上下文初始化
    SwrContext *aCtx = swr_alloc();
    aCtx = swr_alloc_set_opts(aCtx,
                              av_get_default_channel_layout(2),
                              AV_SAMPLE_FMT_S16, audioCodecContext->sample_rate,
                              av_get_default_channel_layout(audioCodecContext->channels),
                              audioCodecContext->sample_fmt, audioCodecContext->sample_rate,
                              0, 0
                               );

    re = swr_init(aCtx);
    if(re != 0)
    {
        LOGW("swr_init failed!");
    }
    else
    {
        LOGI("swr_init success!");
    }


    for (;;) {

        if (getCurrentMs() - start >= 3000) {
            LOGW("now decode fps is %d",frameCount/3);
            start = getCurrentMs();
            frameCount = 0;
        }

        int re = av_read_frame(formatCtx, avPacket);
        if (re != 0) {
            LOGI("已经读到流结尾");//记得传递空帧
            int pos = 20 * r2d(formatCtx->streams[videoStream]->time_base);
            av_seek_frame(formatCtx,videoStream,pos,AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME );
            continue;
        }
//        LOGW("stream = %d size =%d pts=%lld flag=%d",
//                     avPacket->stream_index,avPacket->size,avPacket->pts,avPacket->flags
//                );

//        if (avPacket->stream_index != videoStream) {//只测试视频
//            continue;
//        }
        AVCodecContext *cc = videoCodecContext;
        if (avPacket->stream_index != videoStream) {
            cc = audioCodecContext;
        }
        re = avcodec_send_packet(cc, avPacket);
        //清理
        av_packet_unref(avPacket);
        if (re != 0) {
            LOGW("发送到线程解码失败");
            continue;
        }

        for(;;) {//一个数据包包含多帧数据，循环避免漏帧
            re = avcodec_receive_frame(cc, avFrame);
            if (re != 0) {
//                LOGW("接受解码数据失败");
                break;
            }
//            LOGW("avcodec_receive_frame   pts = %lld", avFrame->pts);
            if (cc == videoCodecContext) {
                frameCount ++;

                vCtx = sws_getCachedContext(vCtx,
                                            avFrame->width,
                                            avFrame->height,
                                            (AVPixelFormat)avFrame->format,
                                            outWidth,
                                            outHeight,
                                            AV_PIX_FMT_RGBA,
                                            SWS_FAST_BILINEAR,
                                            0,0,0
                );

                if (!vCtx) {
                    LOGW("sws_getCachedContext failed!");
                } else {
                    uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
                    data[0] =(uint8_t *)rgb;
                    int lines[AV_NUM_DATA_POINTERS] = {0};
                    lines[0] = outWidth * 4;
                    int h = sws_scale(vCtx,
                                      (const uint8_t **)avFrame->data,
                                      avFrame->linesize,0,
                                      avFrame->height,
                                      data,lines);
                    LOGW("sws_scale = %d",h);
                }
            } else if (cc == audioCodecContext) {//音频重采样
                uint8_t *out[2] = {0};
                out[0] = (uint8_t*)pcm;
                int len = swr_convert(aCtx, out,
                                      avFrame->nb_samples,
                                      (const uint8_t **)avFrame->data,
                                      avFrame->nb_samples
                                      );
                LOGW("swr_convert = %d",len);

            }
        }

    }
    //音频
    //视频
    delete rgb;
    delete pcm;

    avformat_close_input(&formatCtx);
    env->ReleaseStringUTFChars(path_, path);
}