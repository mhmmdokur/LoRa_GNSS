/*
 * lora_gnss_main.h
 *
 *  Created on: Jul 25, 2024
 *      Author: Muhammed
 */

#ifndef SRC_LORA_GNSS_LORA_GNSS_MAIN_H_
#define SRC_LORA_GNSS_LORA_GNSS_MAIN_H_


#include "stdint.h"
#include "protokol.h"
#include "rtcm_decoder.h"
#include "ringbuffer.h"
#include "uart.h"



typedef struct
{
	uint32_t sayac_u32;
	uint8_t _1HzFlag_u8;
	uint8_t _2HzFlag_u8;
	uint8_t _5HzFlag_u8;
	uint8_t _10HzFlag_u8;
	uint8_t _20HzFlag_u8;
	uint8_t _50HzFlag_u8;
	uint8_t _100HzFlag_u8;
	uint8_t _200HzFlag_u8;

//	Dma_t usartDma2_st;
//	Dma_t usartDma3_st;
	ringbuffer_t usart2_rxRingBuffer_st;
	ringbuffer_t usart3_rxRingBuffer_st;
	ringbuffer_t usart2_txRingBuffer_st;
	ringbuffer_t usart3_txRingBuffer_st;

	uart_t usart2_st;
	uart_t usart3_st;

	veri_paketi_t veri_pkt_st;
	Rtcm_t rtcm_st;
	Lora_t lora_st;
	uint8_t usart2_rx_buffer[UART_RX_MAKS_BOYUT];
	uint8_t usart2_tx_buffer[UART_TX_MAKS_BOYUT];
	uint8_t usart3_rx_buffer[UART_RX_MAKS_BOYUT];
	uint8_t usart3_tx_buffer[UART_TX_MAKS_BOYUT];

}Global_t;

extern Global_t Glo_st;

void loraGnssMain();

#endif /* SRC_LORA_GNSS_LORA_GNSS_MAIN_H_ */
