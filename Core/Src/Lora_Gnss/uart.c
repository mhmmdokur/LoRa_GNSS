#include "uart.h"
#include "main.h"
#include "lora_gnss_main.h"
#include <string.h>



void DmaVeriOku(Uart_t *pUart_st, DMA_HandleTypeDef *dmaHandler)
{
	uint16_t tOkunan = 0;

	if(dmaHandler->Instance->CNDTR < pUart_st->dma_st.dmaSayac_u16)
	{
		tOkunan = (pUart_st->dma_st.dmaSayac_u16 - dmaHandler->Instance->CNDTR);
	}
	else if(dmaHandler->Instance->CNDTR > pUart_st->dma_st.dmaSayac_u16)
	{
		tOkunan = (pUart_st->dma_st.dmaSayac_u16 + pUart_st->dma_st.rxRingbuffer_st.len_u16 - dmaHandler->Instance->CNDTR);
	}
	pUart_st->dma_st.dmaSayac_u16 = dmaHandler->Instance->CNDTR;

	ringbuffer_shift_writePtr(&pUart_st->dma_st.rxRingbuffer_st, tOkunan);
}



void DmaBaslat(Uart_t *pUart_st, UART_HandleTypeDef *pUartHandle)
{

	UART_Start_Receive_DMA(pUartHandle, pUart_st->rxBuffer, UART_RX_MAKS_BOYUT);

	ringbuffer_init(pUart_st->txBuffer, sizeof(pUart_st->txBuffer), &pUart_st->dma_st.txRingbuffer_st);
	ringbuffer_init(pUart_st->rxBuffer, sizeof(pUart_st->rxBuffer), &pUart_st->dma_st.rxRingbuffer_st);
}


void DmaVeriGonder(UART_HandleTypeDef *pUartHandle, uint8_t *pBuffer, uint16_t boyut_u16)
{
		HAL_UART_Transmit(pUartHandle, pBuffer, boyut_u16, 100);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

}


//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == USART1)
//	{
////		Glo_st.usartDma1_st.txCallBackFlag_u8 = 1;
//	}
//	if (huart->Instance == USART2)
//	{
//		GL.usart2_st.txCallBackFlag_u8 = 1;
//		HAL_UART_Transmit_DMA(GL.usart2_st.pUartHandle_st, GL.usart2_st.txBuffer, UART_TX_MAKS_BOYUT);
//		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//	}
//	if (huart->Instance == USART3)
//	{
//		GL.usart3_st.txCallBackFlag_u8 = 1;
//		HAL_UART_Transmit_DMA(GL.usart3_st.pUartHandle_st, GL.usart3_st.txBuffer, UART_TX_MAKS_BOYUT);
//		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//	}
//
//}
