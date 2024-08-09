#include "uart.h"
#include "main.h"
#include "lora_gnss_main.h"
#include <string.h>



void DmaVeriOku(Uart_t *pUart_st)
{
	uint16_t tOkunan = 0;

	if(pUart_st->dma_st.pDmaHandle_st->Instance->CNDTR < pUart_st->dma_st.dmaSayac_u16)
	{
		tOkunan = (pUart_st->dma_st.dmaSayac_u16 - pUart_st->dma_st.pDmaHandle_st->Instance->CNDTR);
	}
	else if(pUart_st->dma_st.pDmaHandle_st->Instance->CNDTR > pUart_st->dma_st.dmaSayac_u16)
	{
		tOkunan = (pUart_st->dma_st.dmaSayac_u16 + pUart_st->dma_st.rxRingbuffer_st.len_u16 - pUart_st->dma_st.pDmaHandle_st->Instance->CNDTR);
	}
	pUart_st->dma_st.dmaSayac_u16 = pUart_st->dma_st.pDmaHandle_st->Instance->CNDTR;

	ringbuffer_shift_writePtr(&pUart_st->dma_st.rxRingbuffer_st, tOkunan);
}



void DmaBaslat(Uart_t *pUart_st, UART_HandleTypeDef *pUartHandle, DMA_HandleTypeDef *pDmaHandle)
{
	pUart_st->pUartHandle_st       = pUartHandle;
	pUart_st->dma_st.pDmaHandle_st = pDmaHandle;

	HAL_UART_Transmit_DMA(pUart_st->pUartHandle_st, pUart_st->txBuffer, UART_TX_MAKS_BOYUT);
	UART_Start_Receive_DMA(pUart_st->pUartHandle_st, pUart_st->rxBuffer, UART_RX_MAKS_BOYUT);

	ringbuffer_init(pUart_st->txBuffer, sizeof(pUart_st->txBuffer), &pUart_st->dma_st.txRingbuffer_st);
	ringbuffer_init(pUart_st->rxBuffer, sizeof(pUart_st->rxBuffer), &pUart_st->dma_st.rxRingbuffer_st);

}


void DmaVeriGonder(Uart_t *pUart_st, uint8_t *pBuffer, uint16_t boyut_u16)
{
	if(pUart_st->txCallBackFlag_u8)
	{
		memcpy(pUart_st->txBuffer, pBuffer, boyut_u16);

		HAL_UART_Transmit_DMA(pUart_st->pUartHandle_st, pUart_st->txBuffer, boyut_u16);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

		pUart_st->txCallBackFlag_u8 = 0;
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
//		Glo_st.usartDma1_st.txCallBackFlag_u8 = 1;
	}
	if (huart->Instance == USART2)
	{
		GL.usart2_st.txCallBackFlag_u8 = 1;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}
	if (huart->Instance == USART3)
	{
		GL.usart3_st.txCallBackFlag_u8 = 1;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}

}
