FATE_API_LIBAVCODEC-$(call ENCDEC, FLAC, FLAC) += fate-api-flac
fate-api-flac: $(APITESTSDIR)/api-flac-test$(EXESUF)
fate-api-flac: CMD = run $(APITESTSDIR)/api-flac-test
fate-api-flac: CMP = null
fate-api-flac: REF = /dev/null

#!!!! it's now unavailable
FATE_API_SAMPLES_LIBAVFORMATs-$(call DEMDEC, H264, H264) += fate-api-decode-h264
fate-api-decode-h264: $(APITESTSDIR)/api-decode-test$(EXESUF)
fate-api-decode-h264: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/h264-conformance/SVA_NL2_E.264 bitexact

#!!!! it's now unavailable
FATE_API_SAMPLES_LIBAVFORMATs-$(call DEMDEC, FLV, FLV) += fate-api-decode-flv
fate-api-decode-flv: $(APITESTSDIR)/api-decode-test$(EXESUF) fate-vsynth3-flv
fate-api-decode-flv: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_PATH)/tests/data/fate/vsynth3-flv.flv
FATE_API_SAMPLES-$(CONFIG_AVFORMAT) += $(FATE_API_SAMPLES_LIBAVFORMAT-yes)

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al04_44
fate-api-decode-aac-al04_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al04_44.mp4
fate-api-decode-aac-al04_44: REF = $(SAMPLES)/aac/al04_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al05_44
fate-api-decode-aac-al05_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al05_44.mp4
fate-api-decode-aac-al05_44: REF = $(SAMPLES)/aac/al05_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al06_44
fate-api-decode-aac-al06_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al06_44.mp4
fate-api-decode-aac-al06_44: REF = $(SAMPLES)/aac/al06_44_reorder.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al07_96
fate-api-decode-aac-al07_96: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al07_96.mp4
fate-api-decode-aac-al07_96: REF = $(SAMPLES)/aac/al07_96_reorder.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al15_44
fate-api-decode-aac-al15_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al15_44.mp4
fate-api-decode-aac-al15_44: REF = $(SAMPLES)/aac/al15_44_reorder.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al17_44
fate-api-decode-aac-al17_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al17_44.mp4
fate-api-decode-aac-al17_44: REF = $(SAMPLES)/aac/al17_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al18_44
fate-api-decode-aac-al18_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al18_44.mp4
fate-api-decode-aac-al18_44: REF = $(SAMPLES)/aac/al18_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-am00_88
fate-api-decode-aac-am00_88: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/am00_88.mp4
fate-api-decode-aac-am00_88: REF = $(SAMPLES)/aac/am00_88.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-am05_44
fate-api-decode-aac-am05_44: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/am05_44.mp4
fate-api-decode-aac-am05_44: REF = $(SAMPLES)/aac/am05_44_reorder.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al_sbr_hq_cm_48_2
fate-api-decode-aac-al_sbr_hq_cm_48_2: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al_sbr_cm_48_2.mp4
fate-api-decode-aac-al_sbr_hq_cm_48_2: REF = $(SAMPLES)/aac/al_sbr_hq_cm_48_2.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al_sbr_hq_cm_48_5.1
fate-api-decode-aac-al_sbr_hq_cm_48_5.1: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al_sbr_cm_48_5.1.mp4
fate-api-decode-aac-al_sbr_hq_cm_48_5.1: REF = $(SAMPLES)/aac/al_sbr_hq_cm_48_5.1_reorder.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al_sbr_hq_sr_48_2_fsaac48
fate-api-decode-aac-al_sbr_hq_sr_48_2_fsaac48: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al_sbr_sr_48_2_fsaac48.mp4
fate-api-decode-aac-al_sbr_hq_sr_48_2_fsaac48: REF = $(SAMPLES)/aac/al_sbr_hq_sr_48_2_fsaac48.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-al_sbr_ps_06_ur
fate-api-decode-aac-al_sbr_ps_06_ur: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/al_sbr_ps_06_new.mp4
fate-api-decode-aac-al_sbr_ps_06_ur: REF = $(SAMPLES)/aac/al_sbr_ps_06_ur.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-ap05_48
fate-api-decode-aac-ap05_48: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/ap05_48.mp4
fate-api-decode-aac-ap05_48: REF = $(SAMPLES)/aac/ap05_48.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-er_ad6000np_44_ep0
fate-api-decode-aac-er_ad6000np_44_ep0: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/er_ad6000np_44_ep0.mp4
fate-api-decode-aac-er_ad6000np_44_ep0: REF = $(SAMPLES)/aac/er_ad6000np_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-er_eld1001np_44_ep0
fate-api-decode-aac-er_eld1001np_44_ep0: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/er_eld1001np_44_ep0.mp4
fate-api-decode-aac-er_eld1001np_44_ep0: REF = $(SAMPLES)/aac/er_eld1001np_44.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-er_eld2000np_48_ep0
fate-api-decode-aac-er_eld2000np_48_ep0: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/er_eld2000np_48_ep0.mp4
fate-api-decode-aac-er_eld2000np_48_ep0: REF = $(SAMPLES)/aac/er_eld2000np_48_ep0.s16

FATE_API_SAMPLES_AAC += fate-api-decode-aac-er_eld2100np_48_ep0
fate-api-decode-aac-er_eld2100np_48_ep0: CMD = run $(APITESTSDIR)/api-decode-test $(TARGET_SAMPLES)/aac/er_eld2100np_48_ep0.mp4
fate-api-decode-aac-er_eld2100np_48_ep0: REF = $(SAMPLES)/aac/er_eld2100np_48.s16

FATE_API_SAMPLES_AAC-yes      += $(FATE_API_SAMPLES_AAC)
#FATE_API_SAMPLES_AAC-$(call      DEMDEC, MOV,    AAC)      += $(FATE_API_SAMPLES_AAC)
#FATE_API_SAMPLES_AAC-$(call      DEMDEC, AAC,    AAC_FIXED)+= $(FATE_API_SAMPLES_AAC_FIXED)

FATE_API_SAMPLES_AAC_ALL = $(FATE_API_SAMPLES_AAC-yes)

$(FATE_API_SAMPLES_AAC_ALL): $(APITESTSDIR)/api-decode-test$(EXESUF)
$(FATE_API_SAMPLES_AAC_ALL): CMP  = oneoff
$(FATE_API_SAMPLES_AAC_ALL): FUZZ = 2


FATE_API_SAMPLES-yes += $(FATE_API_SAMPLES_AAC_ALL)

ifdef SAMPLES
    FATE_API_SAMPLES += $(FATE_API_SAMPLES-yes)
endif

FATE_API-$(CONFIG_AVCODEC) += $(FATE_API_LIBAVCODEC-yes)
FATE_API-$(CONFIG_AVFORMAT) += $(FATE_API_LIBAVFORMAT-yes)
FATE_API = $(FATE_API-yes)

FATE-yes += $(FATE_API) $(FATE_API_SAMPLES)

fate-api: $(FATE_API) $(FATE_API_SAMPLES)
fate-api-decode-aac: $(FATE_API_SAMPLES_AAC_ALL)
