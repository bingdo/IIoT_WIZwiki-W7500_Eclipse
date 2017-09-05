/**
  ******************************************************************************
  * @file    main.c
  * @author  IOP Team
  * @version V1.0.0
  * @date    01-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WIZnet SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 WIZnet Co.,Ltd.</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "W7500x_crg.h"
#include "W7500x_wztoe.h"
#include "W7500x_miim.h"
#include "W7500x_i2c.h"
#include "common.h"
#include "uartHandler.h"
#include "flashHandler.h"
#include "storageHandler.h"
#include "gpioHandler.h"
#include "timerHandler.h"
#include "tftp.h"
#include "ConfigData.h"
#include "ConfigMessage.h"
#include "extiHandler.h"
#include "DHCP/dhcp.h"
#include "DNS/dns.h"
#include "S2E.h"
#include "dhcp_cb.h"
#include "atcmd.h"
#include "bsp_dht11.h"
#include "loopback.h"
#include "wztoeHandler.h"
#include "rtcHandler.h"
#include "arp.h"
#include "ping.h"

/* Private typedef -----------------------------------------------------------*/
UART_InitTypeDef UART_InitStructure;

/* Private define ------------------------------------------------------------*/
#define __DEF_USED_MDIO__
//#define __W7500P__ // for W7500P
#ifndef __W7500P__ // for W7500
	#define __DEF_USED_IC101AG__ //for W7500 Test main Board V001
#endif

///////////////////////////////////////
// Debugging Message Printout enable //
///////////////////////////////////////
#define _MAIN_DEBUG_
#define F_APP_DHCP
//#define F_APP_DNS
//#define F_APP_ATC

///////////////////////////
// Demo Firmware Version //
///////////////////////////
#define VER_H		1
#define VER_L		00

/* Private function prototypes -----------------------------------------------*/
void delay(__IO uint32_t milliseconds); //Notice: used ioLibray
void TimingDelay_Decrement(void);

/* Private variables ---------------------------------------------------------*/
/* Transmit and receive buffers */
static __IO uint32_t TimingDelay;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
uint8_t g_send_buf[WORK_BUF_SIZE];
uint8_t g_recv_buf[WORK_BUF_SIZE];
uint8_t gDATABUF[DATA_BUF_SIZE];

uint8_t run_dns = 1;
uint8_t op_mode;
volatile uint8_t factory_flag;
volatile uint32_t factory_time;

uint8_t socket_buf[2048];
uint8_t g_op_mode = NORMAL_MODE;

volatile uint8_t g_check_temp;
#if defined(F_ENABLE_TCPClient)
char str[256];
#endif

volatile int32_t g_int_cnt;
volatile int32_t g_int_rflag;
volatile int32_t g_rtcalarm_flag;

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main()
{
    //uint8_t tx_size[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
    //uint8_t rx_size[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
    //uint8_t mac_addr[6] = {0x00, 0x08, 0xDC, 0x11, 0x22, 0x33};
    //uint8_t src_addr[4] = {192, 168,  0,  80};
    //uint8_t gw_addr[4]  = {192, 168,  0,  1};
    //uint8_t sub_addr[4] = {255, 255, 255,  0};
    //uint8_t dns_server[4] = {8, 8, 8, 8};           // for Example domain name server
    //uint8_t tmp[8];
	int ret;
#if defined(F_APP_DHCP) || defined(F_APP_DNS)
	S2E_Packet *value = get_S2E_Packet_pointer();
#endif
#if defined(F_APP_DNS)
	uint8_t dns_server_ip[4];
#endif
#if defined(F_ENABLE_DHT11)
	DHT11_DATA_TypeDef DHT11_DATA;
#endif

    /* External Clock */
    CRG_PLL_InputFrequencySelect(CRG_OCLK);

    /* Clock */
#if defined(F_ENABLE_DHT11)
    *(volatile uint32_t *)(0x41001014) = 0x00050200; // 20MHz, Default
#else
    *(volatile uint32_t *)(0x41001014) = 0x00060100; // 48MHz
    //*(volatile uint32_t *)(0x41001014) = 0x000C0200; // 48MHz
    //*(volatile uint32_t *)(0x41001014) = 0x00050200; // 20MHz, Default
    //*(volatile uint32_t *)(0x41001014) = 0x00040200; // 16MHz
#endif

    /* Set System init */
    SystemInit();

    /* UART2 Init */
    S_UART_Init(115200);

    /* SysTick_Config */
    SysTick_Config((GetSystemClock()/1000));

    /* Set WZ_100US Register */
    setTIC100US((GetSystemClock()/10000));
    //getTIC100US();	
    //printf(" GetSystemClock: %X, getTIC100US: %X, (%X) \r\n", 
    //      GetSystemClock, getTIC100US(), *(uint32_t *)TIC100US);        

	LED_Init(LED1);
	LED_Init(LED2);
	LED_Init(LED3);

	LED_Off(LED1);
	LED_Off(LED2);
	LED_Off(LED3);

	//BOOT_Pin_Init();
	//Board_factory_Init();
	//EXTI_Configuration();
	//RORT2_Configuration();
#if defined(F_ENABLE_DHT11)
	DHT11_Init();
#endif

#if defined(EEPROM_ENABLE)
    I2C1_Init();
#endif

	/* Load Configure Information */
	load_S2E_Packet_from_storage();
	UART_Configuration();

	/* Check MAC Address */
	//check_mac_address();

    //UART_StructInit(&UART_InitStructure);
    //UART_Init(UART_DEBUG,&UART_InitStructure);
	Timer0_Configuration();
	//WZTOE_Configuration();
	//RTC_Configuration();

	uint8_t tmpstr[6] = {0,};
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
#ifdef _MAIN_DEBUG_
    printf("\r\n============================================\r\n");
	printf(" WIZnet %s EVB Demo v%d.%.2d\r\n", tmpstr, VER_H, VER_L);
	printf("============================================\r\n");
	printf(" WIZ750S2E Platform based S2E App           \r\n");
	printf("============================================\r\n");
#endif

#ifdef __DEF_USED_IC101AG__ //For using IC+101AG
    *(volatile uint32_t *)(0x41003068) = 0x64; //TXD0 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x4100306C) = 0x64; //TXD1 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003070) = 0x64; //TXD2 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003074) = 0x64; //TXD3 - set PAD strengh and pull-up
    *(volatile uint32_t *)(0x41003050) = 0x64; //TXE  - set PAD strengh and pull-up
#endif	
#ifdef __W7500P__
	*(volatile uint32_t *)(0x41003070) = 0x61;
	*(volatile uint32_t *)(0x41003054) = 0x61;
#endif

#ifdef __DEF_USED_MDIO__ 
    /* mdio Init */
    mdio_init(GPIOB, MDC, MDIO);
    
//    printf("%d \r\n", PHY_ADDR);
    /* PHY Link Check via gpio mdio */
    while( link() == 0x0)
    {
        printf(".");  
        delay(500);
    }
    printf("PHY is linked. \r\n");
#else
    delay(1000);
    delay(1000);
#endif


	Mac_Conf();
#if defined(F_APP_DHCP)
	DHCP_init(SOCK_DHCP, g_send_buf);

	/* Initialize Network Information */
	if(value->options.dhcp_use) {		// DHCP
		uint32_t ret;
		uint8_t dhcp_retry = 0;

		//printf("Start DHCP...\r\n");
		while(1) {
			ret = DHCP_run();

			if(ret == DHCP_IP_LEASED)
				break;
			else if(ret == DHCP_FAILED)
				dhcp_retry++;

			if(dhcp_retry > 3) {
				Net_Conf();
				break;
			}
			do_udp_config(SOCK_CONFIG);
		}
	} else 								// Static
		Net_Conf();
#else
	Net_Conf();
#endif

#if defined(F_APP_DNS)
	DNS_init(SOCK_DNS, g_send_buf);
	if(value->options.dns_use) {
		uint8_t dns_retry = 0;

		memcpy(dns_server_ip, value->options.dns_server_ip, sizeof(dns_server_ip));

		while(1) {
			if(DNS_run(dns_server_ip, (uint8_t *)value->options.dns_domain_name, value->network_info[0].remote_ip) == 1)
				break;
			else
				dns_retry++;

			if(dns_retry > 3) {
				break;
			}

			do_udp_config(SOCK_CONFIG);

			if(value->options.dhcp_use)
				DHCP_run();
		}
	}
#endif

	display_Net_Info();

#if defined(F_APP_ATC)
	atc_init(&rxring, &txring);
#endif

	op_mode = OP_DATA;

	while (1) {
#if defined(F_APP_ATC)
		if(op_mode == OP_COMMAND)
		{			// Command Mode
			atc_run();
			sockwatch_run();
		}
		else
#endif
		{							// DATA Mode
			s2e_run(SOCK_DATA);
		}

		do_udp_config(SOCK_CONFIG);

		if (g_check_temp == 1)
		{
			g_check_temp = 0;

#if defined(F_ENABLE_DHT11)
			if(Read_TempAndHumidity(&DHT11_DATA) == SUCCESS)
			{
				printf("\r\nResult\r\n\r\nHumidity: %d.%d %%RH, Temperature: %d.%d C\r\n", DHT11_DATA.humi_int, DHT11_DATA.humi_deci, DHT11_DATA.temp_int, DHT11_DATA.temp_deci);
			}
			else
			{
				printf("\r\nError\r\n\r\nHumidity: %d.%d %%RH, Temperature: %d.%d C\r\n", DHT11_DATA.humi_int, DHT11_DATA.humi_deci, DHT11_DATA.temp_int, DHT11_DATA.temp_deci);
			}
#endif
#if defined(F_ENABLE_TCPClient)
			sprintf(str, "Humidity: %d.%d %%RH, Temperature: %d.%d C\r\n", DHT11_DATA.humi_int, DHT11_DATA.humi_deci, DHT11_DATA.temp_int, DHT11_DATA.temp_deci);
			struct __network_info *network = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
			if(network->state == net_connect)
			{
				RingBuffer_InsertMult(&rxring, &str, strlen(str));
			}
#endif
		}

#if defined(F_APP_DHCP)
		if(value->options.dhcp_use)
			DHCP_run();
#endif

#if defined(F_APP_DNS)
		if(value->options.dns_use && run_dns == 1) {
			memcpy(dns_server_ip, value->options.dns_server_ip, sizeof(dns_server_ip));

			if(DNS_run(dns_server_ip, (uint8_t *)value->options.dns_domain_name, value->network_info[0].remote_ip) == 1) {
				run_dns = 0;
			}
		}
#endif
	}

    return 0;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void delay(__IO uint32_t milliseconds)
{
  TimingDelay = milliseconds;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

