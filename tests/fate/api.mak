FATE_API_LIBAVCODEC-$(call ENCDEC, FLAC, FLAC) += fate-api-flac
fate-api-flac: $(APITESTSDIR)/api-flac-test$(EXESUF)
fate-api-flac: CMD = run $(APITESTSDIR)/api-flac-test
fate-api-flac: CMP = null
fate-api-flac: REF = /dev/null

FATE_API_SAMPLES_LIBAVFORMAT-$(call DEMDEC, H264, H264) += fate-api-decode-h264
fate-api-decode-h264: $(APITESTSDIR)/api-decode-test$(EXESUF)
fate-api-decode-h264: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/h264-conformance/SVA_NL2_E.264 bitexact

FATE_API_SAMPLES_LIBAVFORMAT-$(call DEMDEC, FLV, FLV) += fate-api-decode-flv
fate-api-decode-flv: $(APITESTSDIR)/api-decode-test$(EXESUF) fate-vsynth3-flv
fate-api-decode-flv: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_PATH)/tests/data/fate/vsynth3-flv.flv
FATE_API_SAMPLES-$(CONFIG_AVFORMAT) += $(FATE_API_SAMPLES_LIBAVFORMAT-yes)

FATE_API_SAMPLES_LIBAVFORMAT-yes += fate-api-decode-aac
fate-api-decode-aac: $(APITESTSDIR)/api-decode-test$(EXESUF)
fate-api-decode-aac: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al04_44.mp4
fate-api-decode-aac: REF = $(SAMPLES)/aac/al04_44.s16
fate-api-decode-aac: CMP  = oneoff
fate-api-decode-aac: FUZZ = 2

ifdef SAMPLES
    FATE_API_SAMPLES += $(FATE_API_SAMPLES-yes)
endif

FATE_API-$(CONFIG_AVCODEC) += $(FATE_API_LIBAVCODEC-yes)
FATE_API-$(CONFIG_AVFORMAT) += $(FATE_API_LIBAVFORMAT-yes)
FATE_API = $(FATE_API-yes)

FATE-yes += $(FATE_API) $(FATE_API_SAMPLES)

fate-api: $(FATE_API) $(FATE_API_SAMPLES)
