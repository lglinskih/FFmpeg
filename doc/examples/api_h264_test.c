/*
 * Copyright (c) 2015 Ludmila Glinskih
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * H264 codec test.
 */

#include <math.h>

#include "libavutil/opt.h"
#include "libavutil/adler32.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavformat/internal.h"

static int video_decode_example(const char *input_filename)
{
    AVCodec *codec = NULL;
    AVCodecContext *origin_ctx = NULL, *ctx= NULL;
    AVFrame *fr = NULL;
    uint8_t *byte_buffer = NULL;
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int video_stream;
    int get_frame = 0;
    int byte_buffer_size;
    int i = 0;


    if (avformat_open_input(&fmt_ctx, input_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", input_filename);
        exit(1);
    }

    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }

    video_stream = -1;
    for (i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = i;
            break;
        }
    }

    origin_ctx = fmt_ctx->streams[video_stream]->codec;

    codec = avcodec_find_decoder(origin_ctx->codec_id);
    if (codec == NULL) {
        return -1;
    }
    ctx = avcodec_alloc_context3(codec);
    if (ctx == NULL) {
        return -1;
    }

    if (avcodec_copy_context(ctx, origin_ctx)) {
        return -1;
    }

    if (avcodec_open2(ctx, codec, NULL) < 0) {
        return -1;
    }

    fr = av_frame_alloc();
    if (fr == NULL) {
        return -1;
    }

    byte_buffer_size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 1);
    byte_buffer = (uint8_t*)av_malloc(byte_buffer_size * sizeof(uint8_t));

    printf("#tb %d: %d/%d\n", video_stream, fmt_ctx->streams[video_stream]->time_base.num, fmt_ctx->streams[video_stream]->time_base.den);
    i = 0;
    av_init_packet(&pkt);
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == video_stream) {
            get_frame = 0;
            if (pkt.pts == AV_NOPTS_VALUE)
                pkt.pts = pkt.dts = i;
            avcodec_decode_video2(ctx, fr, &get_frame, &pkt);
            if (get_frame) {
                number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                        (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                        ctx->pix_fmt, ctx->width, ctx->height, 1);
                printf("%d, %10"PRId64", %10"PRId64", %8d, %8d, 0x%08"PRIx32"\n", video_stream,
                        fr->pkt_pts, fr->pkt_dts, fr->pkt_duration,
                        number_of_written_bytes, av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
            }
            av_free_packet(&pkt);
            av_init_packet(&pkt);
        }
        i++;
    }
    pkt.data = NULL;
    pkt.size = 0;
    if (pkt.pts == AV_NOPTS_VALUE)
        pkt.pts = pkt.dts = i;
    int flag = 0;
    while (!flag) {
        get_frame = 0;
        if (avcodec_decode_video2(ctx, fr, &get_frame, &pkt) < 0 || get_frame == 0)
            flag = 1;
        if (get_frame) {
            number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                    (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                    ctx->pix_fmt, ctx->width, ctx->height, 1);
            printf("%d, %10"PRId64", %10"PRId64", %8d, %8d, 0x%08"PRIx32"\n", video_stream,
                    fr->pkt_pts, fr->pkt_dts, fr->pkt_duration,
                    number_of_written_bytes, av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
        }
        i++;
    }
    av_free_packet(&pkt);
    av_frame_free(&fr);
    avcodec_close(ctx);
    avformat_close_input(&fmt_ctx);
    avcodec_free_context(&ctx);
    av_freep(&byte_buffer);
    return 0;
}

int main(int argc, char **argv)
{
    av_register_all();
    video_decode_example(argv[1]);
    return 0;
}






























///sdfsadf
