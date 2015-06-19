/*
 * Copyright (c) 2001 Fabrice Bellard
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

#include <libavutil/opt.h>
#include <libavutil/adler32.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>


/*
 * Video decoding example
 */

static int video_decode_example(const char *input_filename)
{
    AVCodec *codec = NULL;
    AVCodecContext *origin_ctx = NULL, *ctx= NULL;
    FILE *f = NULL;
    AVFrame *fr = NULL;
    uint8_t *buffer = NULL, *byte_buffer = NULL;
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int i;
    int video_stream;
    int num_bytes;
    int get_frame = 0;
    int byte_buffer_size;
    int if_read_frame;


    if (avformat_open_input(&fmt_ctx, input_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", input_filename);
        exit(1);
    }

    printf("file is okay\n");
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }
    printf("stream is opened\n");
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
    printf("Found decode\n");
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
    printf("alloc codec and context\n");
    fr = av_frame_alloc();
    if (fr == NULL)
        return -1;

    num_bytes = avpicture_get_size(ctx->pix_fmt, ctx->width, ctx->height);
    buffer = (uint8_t*)av_malloc(num_bytes * sizeof(uint8_t));

    avpicture_fill((AVPicture*)fr, buffer, ctx->pix_fmt, ctx->width, ctx->height);

    //some staff with picture
    byte_buffer_size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 1);
    printf("%i -- buffer size\n", byte_buffer_size);
    byte_buffer = (uint8_t*)av_malloc(byte_buffer_size * sizeof(uint8_t) + FF_INPUT_BUFFER_PADDING_SIZE);

    av_init_packet(&pkt);
    printf("starting to decode frames\n");
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == video_stream) {
            get_frame = 0;
            avcodec_decode_video2(ctx, fr, &get_frame, &pkt);
            av_free_packet(&pkt);
            av_init_packet(&pkt);
            printf("YEAAAH WE DECODED IT\n");
            if (get_frame) {
                printf("we finally get frame\n");
                number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                        (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                        ctx->pix_fmt, ctx->width, ctx->height, 1);
                printf("%0x \n", av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
            }
        }
    }
    printf("End of file!!\n");
    int flag = 0;
    while (!flag) {
        get_frame = 0;
        pkt.data = NULL;
        pkt.size = 0;
        if (avcodec_decode_video2(ctx, fr, &get_frame, &pkt) < 0 || get_frame == 0)
            flag = 1;
        printf("YEAAAH WE DECODED IT\n");
        if (get_frame) {
            printf("we finally get frame\n");
            number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                    (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                    ctx->pix_fmt, ctx->width, ctx->height, 1);
            printf("%0x \n", av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    /* register all the codecs */
    av_register_all();
    printf("register codecs\n");
    video_decode_example(argv[1]);
//    video_encode_example("test.h264", AV_CODEC_ID_H264);
//    video_decode_example("test%02d.pgm", "test.mpg");

    return 0;
}






























///sdfsadf
