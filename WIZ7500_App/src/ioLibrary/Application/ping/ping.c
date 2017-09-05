
#include "ping.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "common.h"

PINGMSGR PingRequest;	 // Variable for Ping Request
PINGMSGR *PingReply;	 // Variable for Ping Reply
uint16_t RandomID = 0x1234;
uint16_t RandomSeqNum = 0x4321;
uint8_t ping_reply_received = 0;
//uint8_t RX_BUF[1024];

extern uint8_t gDATABUF[2048];

uint8_t ping(uint8_t s, uint16_t pCount, uint8_t *addr, uint16_t port)
{
	uint16_t received_len;
	uint16_t cnt;
	uint16_t sent_count = 0;

	while(pCount > sent_count)
	{	 	 
  		switch(getSn_SR(s))
		{
			case SOCK_CLOSED:
				close(s); // close the SOCKET

				/* Create Socket */  
#if ( _WIZCHIP_ < 5200 )
				IINCHIP_WRITE(Sn_PROTO(s), IPPROTO_ICMP); // set ICMP Protocol
#elif ( _WIZCHIP_ == 7500 )
				setSn_PROTO(s, IPPROTO_ICMP); // set ICMP Protocol
#endif
				if(socket(s,Sn_MR_IPRAW,port,0) != s){ // open the SOCKET with IPRAW mode, if fail then Error
					printf("\r\nsocket %.2d fail\r\n", s);
				}
					
				/* Check socket register */
			    while(getSn_SR(s)!=SOCK_IPRAW); 
				delay_ms(200);
				break;

			case SOCK_IPRAW:
				printf("\r\nNo.%d\r\n", sent_count+1);
				ping_request(s, addr, port);
				sent_count++;

				while(1){
					if ((received_len = getSn_RX_RSR(s)) > 0){
						ping_reply(s, addr, port, received_len);	
						if (ping_reply_received) {
							//printf("\r\n");
							break;
						}
					}
				
					/* wait_time for 3 seconds, Break on fail*/
					if ((cnt > 100)) {
						printf("Request Time out.\r\n");					
						//printf("\r\n");
						cnt = 0;
						break;
					}else { 
						cnt++; 	
						delay_ms(2);
					}
				}
				break;

			default:		
				break;
		}
	}
	
	return ping_reply_received;
}

uint8_t ping_request(uint8_t s, uint8_t *addr, uint16_t port)
{
	uint16_t tmp_count = 0;
	uint16_t i;
 
	//Initialize flag for ping reply
	ping_reply_received = 0;

	/* make header of the ping-request  */
	PingRequest.Type = PING_REQUEST;            // Ping-Request 
	PingRequest.Code = CODE_ZERO;	            // Always '0'
	PingRequest.ID = htons(RandomID++);	        // set ping-request's ID to random integer value
	PingRequest.SeqNum = htons(RandomSeqNum++); // set ping-request's sequence number to ramdom integer value      
	//size = 32;                                // set Data size

	/* Fill in Data[]  as size of BIF_LEN (Default = 32)*/
  	for(i = 0 ; i < BUF_LEN; i++){	                                
		PingRequest.Data[i] = 0x61 + tmp_count;
		tmp_count++;
		if(tmp_count > 22) tmp_count = 0;
	}

	/* Do checksum of Ping Request */
	PingRequest.CheckSum = 0; // value of checksum before calculating checksum of ping-request packet
	PingRequest.CheckSum = htons(checksum((uint8_t *)&PingRequest,sizeof(PingRequest))); // Calculate checksum
	
	/* sendto ping_request to destination */
	if(sendto(s,(uint8_t *)&PingRequest,sizeof(PingRequest),addr,port)==0){ // Send Ping-Request to the specified peer.
	  	 printf("Fail to send Ping Request packet [");					
	  	 printf("%.3d.%.3d.%.3d.%.3d]\r\n", addr[0], addr[1], addr[2], addr[3]);
	}else{
	 	 printf("Send Ping Request to Destination [");					
         printf("%.3d.%.3d.%.3d.%.3d]", addr[0], addr[1], addr[2], addr[3]) ;
		 printf("\tID:%.x, SeqNum:%.x, CheckSum:%.4x\r\n", htons(PingRequest.ID), htons(PingRequest.SeqNum), htons(PingRequest.CheckSum));
	}

	return 0;
} // ping request

uint8_t ping_reply(uint8_t s, uint8_t *addr, uint16_t port, uint16_t rlen)
{
	uint16_t len;
	//PingReply = (PINGMSGR*)RX_BUF;
	uint8_t data_buf[100];
	
	/* receive data from a destination */
	if ((len = recvfrom(s, (uint8_t *)gDATABUF, rlen, addr, &port)) > 0)
	{
		ping_reply_received = 1;
		memcpy(data_buf, gDATABUF, len);
		PingReply = data_buf;
		//for(int i=0; i<len; i++)
		//{
		//	printf("PingReply[%d]:0x%.2X\r\n",i , data_buf[i]);
		//}
	}

	/* check the Type, If TYPE is 0, the received msg is the ping reply msg.*/
	if (PingReply->Type == PING_REPLY) {
		printf("Receive Ping Reply from Destination [");
		printf("%.3d.%.3d.%.3d.%.3d]", addr[0], addr[1], addr[2], addr[3]);
		printf("\tID:%.x, SeqNum:%.x, CheckSum:%.4x\r\n", htons(PingReply->ID), htons(PingReply->SeqNum), htons(PingReply->CheckSum));
	}

	//delay_ms(200);

	return 0;

} // ping_reply
