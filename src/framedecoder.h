/*
    SPDX-FileCopyrightText: 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MOVIEDECODER_H
#define MOVIEDECODER_H

#include <QString>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
}

struct VideoFrame {
    VideoFrame()
            : width(0), height(0), lineSize(0) {}

    VideoFrame(int width, int height, int lineSize)
            : width(width), height(height), lineSize(lineSize) {}

    quint32 width;
    quint32 height;
    quint32 lineSize;

    std::vector<quint8> frameData;
};

class FrameDecoder
{
public:
    explicit FrameDecoder(const QString& filename, AVFormatContext* pavContext = nullptr);
    ~FrameDecoder();

    QString getCodec();
    void seek(int timeInSeconds);
    bool decodeVideoFrame();
    void getScaledVideoFrame(int scaledSize, bool maintainAspectRatio, VideoFrame& videoFrame);

    int getWidth();
    int getHeight();
    int getDuration();

    void initialize(const QString& filename);
    void destroy();
    bool getInitialized();
    void writeFrame(VideoFrame& frame, QImage& image);

private:
    bool initializeVideo();

    bool decodeVideoPacket();
    bool getVideoPacket();
    void convertAndScaleFrame(AVPixelFormat format, int scaledSize, bool maintainAspectRatio, int& scaledWidth, int& scaledHeight);
    void createAVFrame(AVFrame** avFrame, quint8** frameBuffer, int width, int height, AVPixelFormat format);
    void calculateDimensions(int squareSize, bool maintainAspectRatio, int& destWidth, int& destHeight);

    void deleteFilterGraph();
    bool initFilterGraph(enum AVPixelFormat pixfmt, int width, int height);
    bool processFilterGraph(AVFrame *dst, const AVFrame *src, enum AVPixelFormat pixfmt, int width, int height);

private:
    int                     m_VideoStream;
    AVFormatContext*        m_pFormatContext;
    AVCodecContext*         m_pVideoCodecContext;
    AVCodec*                m_pVideoCodec;
    AVStream*               m_pVideoStream;
    AVFrame*                m_pFrame;
    quint8*                 m_pFrameBuffer;
    AVPacket*               m_pPacket;
    bool                    m_FormatContextWasGiven;
    bool                    m_AllowSeek;
    bool                    m_initialized;
    AVFilterContext*        m_bufferSinkContext;
    AVFilterContext*        m_bufferSourceContext;
    AVFilterGraph*          m_filterGraph;
    AVFrame*                m_filterFrame;
    int                     m_lastWidth;
    int                     m_lastHeight;
    enum AVPixelFormat      m_lastPixfmt;
};

#endif