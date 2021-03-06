#include <stdint.h>

#define ARP_TYPE	    0x0806
#define ARP_TYPE_HI	    0x08
#define ARP_TYPE_LO     0x06
#define ETHER_TYPE	    0x0001
#define PRO_TYPE	    0x0800
#define HW_SIZE	        6
#define PRO_SIZE	    4
#define ARP_REQUEST	    0x0001
#define ARP_REPLY	    0x0002

typedef struct _ARPMSG
{
	uint8_t  dst_mac[6];	// ff.ff.ff.ff.ff.ff
	uint8_t  src_mac[6];
	uint16_t msg_type;   // ARP (0x0806)
	uint16_t hw_type;		// Ethernet (0x0001)
	uint16_t pro_type;	// IP	(0x0800)
	uint8_t  hw_size;		// 6
	uint8_t  pro_size;	    // 4
	uint16_t opcode;		// request (0x0001), reply(0x0002)
	uint8_t  sender_mac[6];
	uint8_t  sender_ip[4];
	uint8_t  tgt_mac[6];	// 00.00.00.00.00.00
	uint8_t  tgt_ip[4];
	uint8_t  trailer[18];	// All zeros
}ARPMSG;

void arp(uint8_t s, uint16_t aPort, uint8_t * SrcIp, uint8_t *SrcMac, uint8_t *TgtIp, uint8_t count);
void arp_request(uint8_t s, uint16_t port, uint8_t * SrcIP, uint8_t *SrcMac, uint8_t *TgtIp);
void arp_reply(uint8_t s, uint16_t rlen);
