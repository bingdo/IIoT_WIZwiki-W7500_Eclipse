
#include "arp.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "common.h"

uint16_t mac_remain_time;
uint16_t mac_arp_received = 0;
uint8_t* pMACRAW;

ARPMSG pARPMSG;
ARPMSG * aARPMSG;

extern uint8_t gDATABUF[2048];

void arp(uint8_t s, uint16_t aPort, uint8_t * SrcIp, uint8_t *SrcMac, uint8_t *TgtIp, uint8_t count)
{
	uint16_t len = 0;
	uint16_t i= 0;
	uint16_t rlen =0;
	uint16_t cnt =0;

	for ( i=0 ; i<count +1; i++ ){
				
		switch(getSn_SR(s))
			{
				case SOCK_CLOSED:
					close(s);                                                     // close the SOCKET
					socket(s,Sn_MR_MACRAW,aPort,0);            // open the SOCKET with MACRAW mode
				break;
	
				case SOCK_MACRAW:
	
					//delay_ms(1000);
						
					    mac_arp_received	= 0;
						arp_request(s, aPort, SrcIp, SrcMac, TgtIp);
						while(1){
							if ( (rlen = getSn_RX_RSR(s) ) > 0){
									arp_reply(s, rlen);	
								if (mac_arp_received)  break;
							}
							if ( (cnt > 100) ) {
								printf("Request Time out.\n");
								//PRINTLN("Request Time out.");
								break;
							}else { 
								cnt++; 	
								delay_ms(20);
							}
					     }
	
					break;
				default:		
					break;
			}
		}
}

void arp_request(uint8_t s, uint16_t port, uint8_t *SrcIp, uint8_t *SrcMac, uint8_t *TgtIp)
{
		uint32_t tip = 0xFFFFFFFF;
		uint16_t i =0;
	
		for( i=0; i<6 ; i++) {
			pARPMSG.dst_mac[i] = 0xFF;//BROADCAST_MAC[i];
			pARPMSG.src_mac[i] = SrcMac[i];//BROADCAST_MAC[i];
			pARPMSG.sender_mac[i] = SrcMac[i];//BROADCAST_MAC[i];
			pARPMSG.tgt_mac[i] = 0;	
		}
	
		pARPMSG.msg_type = htons(ARP_TYPE);
		pARPMSG.hw_type   = htons(ETHER_TYPE);
		pARPMSG.pro_type  = htons(PRO_TYPE);        // IP	(0x0800)
		pARPMSG.hw_size    =  HW_SIZE;		        // 6
		pARPMSG.pro_size   = PRO_SIZE;		        // 4
		pARPMSG.opcode      =  htons(ARP_REQUEST);		// request (0x0001), reply(0x0002)
		for( i=0; i<4 ; i++) {
			pARPMSG.sender_ip[i] = SrcIp[i];//BROADCAST_MAC[i];
			pARPMSG.tgt_ip[i] = TgtIp[i];
		}

	if( sendto(s,(uint8_t*)&pARPMSG,sizeof(pARPMSG),(uint8_t *)&tip,port) ==0){
			printf( "\r\n Fail to send ping-reply packet  \r\n") ;
			//PRINTLN( "\r\n Fail to send ping-reply packet  ") ;
	}else{
				if(pARPMSG.opcode==htons(ARP_REQUEST)){
					printf("\r\nWho has ");
					printf( "%.3d.%.3d.%.3d.%.3d ? ", (pARPMSG.tgt_ip[0]), (pARPMSG.tgt_ip[1]), (pARPMSG.tgt_ip[2]), (pARPMSG.tgt_ip[3])) ;
					printf( "  Tell %.3d.%.3d.%.3d.%.3d ? \r\n", (pARPMSG.sender_ip[0]) , (pARPMSG.sender_ip[1]), (pARPMSG.sender_ip[2]), (pARPMSG.sender_ip[3]));
					//PRINTLN("\r\nWho has ");
					//PRINTLN4( "%.3d.%.3d.%.3d.%.3d ? ", (pARPMSG.tgt_ip[0]), (pARPMSG.tgt_ip[1]), (pARPMSG.tgt_ip[2]), (pARPMSG.tgt_ip[3])) ;
					//PRINTLN4( "  Tell %.3d.%.3d.%.3d.%.3d ? ", (pARPMSG.sender_ip[0]) , (pARPMSG.sender_ip[1]), (pARPMSG.sender_ip[2]), (pARPMSG.sender_ip[3]));
				}else{
					printf("opcode has wrong value. check opcode !\r\n");
					//PRINTLN("opcode has wrong value. check opcode !");
				}
	}

}


void arp_reply(uint8_t s, uint16_t rlen)
{
	uint16_t mac_destport;
	//uint8_t * data_buf = 0x007000;
	uint8_t data_buf[100];
	uint16_t len =0;
	uint8_t mac_destip[4];
	
		/* receive data from a destination */
	len = recvfrom(s,(uint8_t *)gDATABUF,rlen,mac_destip,&mac_destport);
	memcpy(data_buf, gDATABUF, len);
	//for(int i=0; i<len; i++)
	//{
	//	printf("data_buf[%d]:0x%.2X\r\n",i , data_buf[i]);
	//}
	if( data_buf[12]==ARP_TYPE_HI && data_buf[13]==ARP_TYPE_LO ){
			//aARPMSG = 0x007000;
			aARPMSG = data_buf;
				  if( ((aARPMSG->opcode) == htons(ARP_REPLY)) &&(mac_arp_received == 0)  ) {
							mac_arp_received = 1;
							printf( "%.3d.%.3d.%.3d.%.3d ",
								 (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]),
								 (aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3])) ;
							printf(" is at  ");
							printf( "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X \r\n",
								 (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]),
								 (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;
							//PRINTLN4( "%.3d.%.3d.%.3d.%.3d ",
							//	 (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]), 
							//	 (aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3])) ;
							//PRINTLN(" is at  ");
							//PRINTLN6( "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X ",
							//	 (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]), 
							//	 (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;
                  }else if( (aARPMSG->opcode) == ARP_REQUEST && (mac_arp_received == 1)  ) {
		 				    //mac_arp_received = 1;
							printf( "Who has %.3d.%.3d.%.3d.%.3d ? ",
								 (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]),
								 (aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3])) ;
							printf(" Tell  ");
							printf( "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X \r\n",
								 (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]),
								 (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;
							//PRINTLN4( "Who has %.3d.%.3d.%.3d.%.3d ? ",
							//	 (aARPMSG->sender_ip[0]),  (aARPMSG->sender_ip[1]), 
							//	 (aARPMSG->sender_ip[2]),  (aARPMSG->sender_ip[3])) ;
							//PRINTLN(" Tell  ");
							//PRINTLN6( "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X ",
							//	 (aARPMSG->sender_mac[0]),  (aARPMSG->sender_mac[1]),  (aARPMSG->sender_mac[2]), 
							//	 (aARPMSG->sender_mac[3]), (aARPMSG->sender_mac[4]),  (aARPMSG->sender_mac[5])) ;

				  }else{
					        //printf(" This msg is not ARP reply : opcode is not 0x02 \n");
				  }
	}else{
			 //printf(" This msg is not ARP TYPE : ARP TYPE is 0x0806 \n");
	}
  
}
