#ifndef MODULE_AV_STREAM_RTP_H264_UNPACK_H
#define MODULE_AV_STREAM_RTP_H264_UNPACK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "rtp-packet.h"
#include "rtp-payload-internal.h"

#define H264_NAL(v)	(v & 0x1F)
#define FU_START(v) (v & 0x80)
#define FU_END(v)	(v & 0x40)
#define FU_NAL(v)	(v & 0x1F)

struct rtp_decode_h264_t
{
	struct rtp_payload_t handler;
	void* cbparam;

	uint16_t seq; // rtp seq
	uint32_t timestamp;

	uint8_t* ptr;
	int size, capacity;

	int flags;
};

void* rtp_h264_unpack_create(struct rtp_payload_t *handler, void* param);
void rtp_h264_unpack_destroy(void* p);
int rtp_h264_unpack_stap(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int stap_b);
int rtp_h264_unpack_mtap(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int n);
int rtp_h264_unpack_fu(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int fu_b);
int rtp_h264_unpack_input(void* p, const void* packet, int bytes);

#ifdef __cplusplus
}
#endif

#endif//MODULE_AV_STREAM_RTP_H264_UNPACK_H
