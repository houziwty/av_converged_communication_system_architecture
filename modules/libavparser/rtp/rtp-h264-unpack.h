// RFC6184 RTP Payload Format for H.264 Video

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

static void* rtp_h264_unpack_create(struct rtp_payload_t *handler, void* param);
static void rtp_h264_unpack_destroy(void* p);
static int rtp_h264_unpack_stap(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int stap_b);
static int rtp_h264_unpack_mtap(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int n);
static int rtp_h264_unpack_fu(struct rtp_decode_h264_t *unpacker, const uint8_t* ptr, int bytes, uint32_t timestamp, int fu_b);
static int rtp_h264_unpack_input(void* p, const void* packet, int bytes);

#ifdef __cplusplus
}
#endif
