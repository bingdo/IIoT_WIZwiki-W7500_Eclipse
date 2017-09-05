
#include <stdint.h>

#define BUF_LEN 32
#define PING_REQUEST 8
#define PING_REPLY 0
#define CODE_ZERO 0

typedef struct pingmsg
{
  uint8_t	Type; 		// 0 - Ping Reply, 8 - Ping Request
  uint8_t	Code;		// Always 0
  uint16_t	CheckSum;	// Check sum
  uint16_t	ID;         // Identification
  uint16_t	SeqNum; 	// Sequence Number
  uint8_t	Data[BUF_LEN];// Ping Data  : 1452 = IP RAW MTU - sizeof(Type+Code+CheckSum+ID+SeqNum)
} PINGMSGR;

uint8_t ping(uint8_t s, uint16_t pCount, uint8_t *addr, uint16_t Pprt);
uint8_t ping_request(uint8_t s, uint8_t *addr, uint16_t port);
uint8_t ping_reply(uint8_t s,  uint8_t *addr, uint16_t port, uint16_t rlen);

