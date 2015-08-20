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
 * Decode test.
 */

#include "libavutil/adler32.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"

static int video_decode(const char *input_filename, int is_bitexact)
{
    AVCodec *codec = NULL;
    AVCodecContext *origin_ctx = NULL, *ctx= NULL;
    AVFrame *fr = NULL;
    uint8_t *byte_buffer = NULL;
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int video_stream;
    int got_frame = 0;
    int byte_buffer_size = 0;
    int i = 0;
    int result;
    int end_of_stream = 0;
    int j = 0;

    result = avformat_open_input(&fmt_ctx, input_filename, NULL, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file\n");
        return result;
    }

    result = avformat_find_stream_info(fmt_ctx, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't get stream info\n");
        return result;
    }

    video_stream = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream < 0) {
      av_log(NULL, AV_LOG_ERROR, "Can't find video stream in input file\n");
      return -1;
    }

    origin_ctx = fmt_ctx->streams[video_stream]->codec;

    codec = avcodec_find_decoder(origin_ctx->codec_id);
    if (!codec) {
        av_log(NULL, AV_LOG_ERROR, "Can't find decoder\n");
        return -1;
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate decoder context\n");
        return AVERROR(ENOMEM);
    }

    result = avcodec_copy_context(ctx, origin_ctx);
    if (result) {
        av_log(NULL, AV_LOG_ERROR, "Can't copy decoder context\n");
        return result;
    }

    ctx->flags |= AV_CODEC_FLAG_BITEXACT;

    result = avcodec_open2(ctx, codec, NULL);
    if (result < 0) {
        av_log(ctx, AV_LOG_ERROR, "Can't open decoder\n");
        return result;
    }

    fr = av_frame_alloc();
    if (!fr) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate frame\n");
        return AVERROR(ENOMEM);
    }

    byte_buffer_size = av_image_get_buffer_size(ctx->pix_fmt, ctx->width, ctx->height, 16);
    byte_buffer = av_malloc(byte_buffer_size);
    if (!byte_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer\n");
        return AVERROR(ENOMEM);
    }

    printf("#tb %d: %d/%d\n", video_stream, fmt_ctx->streams[video_stream]->time_base.num, fmt_ctx->streams[video_stream]->time_base.den);
    i = 0;
    av_init_packet(&pkt);
    do {
        if (!end_of_stream)
            if (av_read_frame(fmt_ctx, &pkt) < 0)
                end_of_stream = 1;
        if (end_of_stream) {
            pkt.data = NULL;
            pkt.size = 0;
        }
        if (pkt.stream_index == video_stream || end_of_stream) {
            got_frame = 0;
            if (pkt.pts == AV_NOPTS_VALUE)
                pkt.pts = pkt.dts = i;
            result = avcodec_decode_video2(ctx, fr, &got_frame, &pkt);
            if (result < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error decoding frame\n");
                return result;
            }
            if (got_frame) {
                number_of_written_bytes = av_image_copy_to_buffer(byte_buffer, byte_buffer_size,
                                        (const uint8_t* const *)fr->data, (const int*) fr->linesize,
                                        ctx->pix_fmt, ctx->width, ctx->height, 1);
                if (number_of_written_bytes < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Can't copy image to buffer\n");
                    return number_of_written_bytes;
                }
                if (is_bitexact)
                    printf("%d, %10"PRId64", %10"PRId64", %8"PRId64", %8d, 0x%08lx\n", video_stream,
                            fr->pkt_pts, fr->pkt_dts, av_frame_get_pkt_duration(fr),
                            number_of_written_bytes, av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
                else
                    for (j = 0; j < number_of_written_bytes; j++)
                        printf("%02x", byte_buffer[j]);
            }
            av_free_packet(&pkt);
            av_init_packet(&pkt);
        }
        i++;
    } while (!end_of_stream || got_frame);

    av_free_packet(&pkt);
    av_frame_free(&fr);
    avcodec_close(ctx);
    avformat_close_input(&fmt_ctx);
    avcodec_free_context(&ctx);
    av_freep(&byte_buffer);
    return 0;
}

static int audio_decode(const char *input_filename, int is_bitexact)
{
    AVCodec *codec = NULL;
    AVCodecContext *origin_ctx = NULL, *ctx= NULL;
    AVFrame *fr = NULL;
    uint8_t *byte_buffer = NULL;
    AVPacket pkt;
    AVFormatContext *fmt_ctx = NULL;
    int number_of_written_bytes;
    int audio_stream;
    int got_frame = 0;
    int byte_buffer_size = 0;
    int i = 0;
    int result = 0;
    int end_of_stream = 0;
    int j = 0;
    int k = 0;
    int length_read;
    struct SwrContext *swr_ctx;
    int dst_nb_samples, max_dst_nb_samples;
    int dst_bufsize;
    int dst_linesize = 0;
    uint8_t **dst_data = NULL;

    result = avformat_open_input(&fmt_ctx, input_filename, NULL, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file\n");
        return result;
    }

    result = avformat_find_stream_info(fmt_ctx, NULL);
    if (result < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't get stream info\n");
        return result;
    }

    audio_stream = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream < 0) {
      av_log(NULL, AV_LOG_ERROR, "Can't find audio stream in input file\n");
      return -1;
    }

    origin_ctx = fmt_ctx->streams[audio_stream]->codec;

    codec = avcodec_find_decoder(origin_ctx->codec_id);
    if (!codec) {
        av_log(NULL, AV_LOG_ERROR, "Can't find decoder\n");
        return -1;
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate decoder context\n");
        return AVERROR(ENOMEM);
    }

    result = avcodec_copy_context(ctx, origin_ctx);
    if (result) {
        av_log(NULL, AV_LOG_ERROR, "Can't copy decoder context\n");
        return result;
    }

    ctx->flags |= AV_CODEC_FLAG_BITEXACT;

    result = avcodec_open2(ctx, codec, NULL);
    if (result < 0) {
        av_log(ctx, AV_LOG_ERROR, "Can't open decoder\n");
        return result;
    }

    fr = av_frame_alloc();
    if (!fr) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate frame\n");
        return AVERROR(ENOMEM);
    }

    byte_buffer_size = fr->nb_samples * av_get_bytes_per_sample(fr->format);
    byte_buffer = av_malloc(byte_buffer_size);
    if (!byte_buffer) {
        av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer\n");
        return AVERROR(ENOMEM);
    }

    if (is_bitexact)
        printf("#tb %d: %d/%d\n", audio_stream, fmt_ctx->streams[audio_stream]->time_base.num, fmt_ctx->streams[audio_stream]->time_base.den);
    i = 0;
    av_init_packet(&pkt);
    do {
        if (!end_of_stream)
            if (av_read_frame(fmt_ctx, &pkt) < 0)
                end_of_stream = 1;
        if (end_of_stream) {
            pkt.data = NULL;
            pkt.size = 0;
        }
        if (pkt.stream_index == audio_stream || end_of_stream) {
            got_frame = 0;
            if (pkt.pts == AV_NOPTS_VALUE)
                pkt.pts = pkt.dts = i;
            length_read = 0;
            do {
                result = avcodec_decode_audio4(ctx, fr, &got_frame, &pkt);
                if (result < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error decoding frame\n");
                    return result;
                }
                length_read += result;
                if (got_frame) {
                    number_of_written_bytes = fr->nb_samples * av_get_bytes_per_sample(fr->format);
                    if (number_of_written_bytes < 0) {
                        av_log(NULL, AV_LOG_ERROR, "Can't copy image to buffer\n");
                        return number_of_written_bytes;
                    }
                    if (is_bitexact)
                        printf("%d, %10"PRId64", %10"PRId64", %8"PRId64", %8d, 0x%08lx\n", audio_stream,
                                fr->pkt_pts, fr->pkt_dts, av_frame_get_pkt_duration(fr),
                                number_of_written_bytes, av_adler32_update(0, (const uint8_t*)byte_buffer, number_of_written_bytes));
                    else {
                        if (ctx->channels > AV_NUM_DATA_POINTERS)
                            printf("AAAAAA!\n");




                        if (ctx->sample_fmt != AV_SAMPLE_FMT_S16) {
                            //awresample
//                            printf("We want to set some shit parameters!!\n");
                            swr_ctx = swr_alloc_set_opts(NULL,
                                            ctx->channel_layout,
                                            AV_SAMPLE_FMT_S16,
                                            48000,
                                            ctx->channel_layout,
                                            ctx->sample_fmt,
                                            ctx->sample_rate,
                                            0, NULL);
//                            printf("We are set some shit parameters!!\n");

/*                            swr_ctx = swr_alloc();
                            if (!swr_ctx) {
                                av_log(NULL, AV_LOG_ERROR, "Could not allocate resampler context\n");
                                return -1;
                            }*/

                            result = swr_init(swr_ctx);
                            if (result < 0) {
                                av_log(NULL, AV_LOG_ERROR, "Can't initialize the resampling context\n");
                                return result;
                            }

                            max_dst_nb_samples = dst_nb_samples =
                                av_rescale_rnd(fr->nb_samples, 48000, ctx->sample_rate, AV_ROUND_UP);
                            result = av_samples_alloc_array_and_samples(&dst_data, &dst_linesize, ctx->channels,
                                                                     dst_nb_samples, AV_SAMPLE_FMT_S16, 0);
                            if (result < 0) {
                                av_log(NULL, AV_LOG_ERROR, "Can't allocate buffer for samples after resampling\n");
                                return result;
                            }
                            dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, ctx->sample_rate) +
                                                            fr->nb_samples, 48000, ctx->sample_rate, AV_ROUND_UP);
//                            printf("We are alloced some shit!!\n");
                            if (dst_nb_samples > max_dst_nb_samples) {
                                av_freep(&dst_data[0]);
                                result = av_samples_alloc(dst_data, &dst_linesize, ctx->channels,
                                                       dst_nb_samples, AV_SAMPLE_FMT_S16, 1);
                                if (result < 0)
                                    break;
                                max_dst_nb_samples = dst_nb_samples;
                            }

//                            printf("We are ready to convert this shit!!\n");
                            /* convert to destination format */
                            result = swr_convert(swr_ctx, dst_data, dst_nb_samples, (const uint8_t **)fr->data, fr->nb_samples);
                            if (result < 0) {
                                av_log(NULL, AV_LOG_ERROR, "Error while resampling\n");
                                return result;
                            }
                            dst_bufsize = av_samples_get_buffer_size(&dst_linesize, ctx->channels,
                                                                     result, AV_SAMPLE_FMT_S16, 1);
                            if (dst_bufsize < 0) {
                                av_log(NULL, AV_LOG_ERROR, "Can'get buffer size after resampling\n");
                                return dst_bufsize;
                            }
                            fwrite(dst_data[0], 1, dst_bufsize, stdout);
                        }
                        else
                            for (j = 0; j < fr->nb_samples; j++)
                                for (k = 0; k < ctx->channels; k++)
                                    fwrite(fr->data[k] + av_get_bytes_per_sample(ctx->sample_fmt) * i, 1, av_get_bytes_per_sample(ctx->sample_fmt), stdout);
                    }
                }
            } while (length_read < pkt.size);
            av_free_packet(&pkt);
            av_init_packet(&pkt);
        }
        i++;
    } while (!end_of_stream || got_frame);

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
    int result;

    if (argc < 2)
    {
        av_log(NULL, AV_LOG_ERROR, "Incorrect input\n");
        return 1;
    }

    av_register_all();

    if (argc == 3) {
        if (!strcmp(argv[2], "bitexact"))
            result = video_decode(argv[1], 1);
        else {
            av_log(NULL, AV_LOG_ERROR, "Incorrect input\n");
            return 1;
        }
    }
    else
        result = audio_decode(argv[1], 0);
    if (result)
        return 1;

    return 0;
}
