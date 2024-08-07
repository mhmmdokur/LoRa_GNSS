/*
 * lora_gnss_main.c
 *
 *  Created on: Jul 25, 2024
 *      Author: Muhammed
 */

#include "lora_gnss_main.h"
#include "protokol.h"
#include "main.h"
#include "uart.h"

Global_t Glo_st = {0};

void loraGnssMain()
{
//		UartDmaInit(&huart1, &hdma_usart1_rx, &Glo_st.usartDma1_st);
//		UartDmaInit(&huart2, &hdma_usart2_rx, &Glo_st.usartDma2_st);
//		UartDmaInit(&huart3, &hdma_usart3_rx, &Glo_st.usartDma3_st);

	ringbuffer_init(Glo_st.usart2_rx_buffer, sizeof(Glo_st.usart2_rx_buffer), &Glo_st.usart2_rxRingBuffer_st);
	ringbuffer_init(Glo_st.usart3_rx_buffer, sizeof(Glo_st.usart3_rx_buffer), &Glo_st.usart3_rxRingBuffer_st);

	HAL_UART_Transmit_DMA(&huart2, Glo_st.usart2_tx_buffer, sizeof(Glo_st.usart2_tx_buffer));
	HAL_UART_Transmit_DMA(&huart3, Glo_st.usart3_tx_buffer, sizeof(Glo_st.usart3_tx_buffer));

	UART_Start_Receive_DMA(&huart2, Glo_st.usart2_rxRingBuffer_st.pData_u8a, Glo_st.usart2_rxRingBuffer_st.len_u16);
	UART_Start_Receive_DMA(&huart3, Glo_st.usart3_rxRingBuffer_st.pData_u8a, Glo_st.usart3_rxRingBuffer_st.len_u16);

	Glo_st.lora_st.adres_u8 = 0x01;
	Glo_st.lora_st.kanal_u8 = 0x04;

	while(1)
	{
		if(1 == Glo_st._100HzFlag_u8)
		{
			uart_oku_dma(&hdma_usart2_rx, &Glo_st.usart2_st, &Glo_st.usart2_rxRingBuffer_st);
			uart_oku_dma(&hdma_usart3_rx, &Glo_st.usart3_st, &Glo_st.usart3_rxRingBuffer_st);

			Glo_st._100HzFlag_u8 = 0;
		}
		if(1 == Glo_st._50HzFlag_u8)
		{
//			Lora_veri_gonderme_cevrimi(&Glo_st.usart2_rxRingBuffer_st, &Glo_st.veri_pkt_st, &Glo_st.lora_st);
			Lora_veri_alma_cevrimi(&Glo_st.usart3_rxRingBuffer_st, &Glo_st.veri_pkt_st);

			Glo_st._50HzFlag_u8 = 0;
		}

	}
}
