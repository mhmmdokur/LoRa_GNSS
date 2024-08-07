#include "uart.h"
#include "main.h"
#include "lora_gnss_main.h"



void uart_oku_dma(DMA_HandleTypeDef *pDma, uart_t *pUart_st, ringbuffer_t *pBuffer)
{
	uint16_t tOkunan = 0;

	if(pDma->Instance->CNDTR < pUart_st->dmaSayac_u16)
	{
		tOkunan = (pUart_st->dmaSayac_u16 - pDma->Instance->CNDTR);
	}
	else if(pDma->Instance->CNDTR > pUart_st->dmaSayac_u16)
	{
		tOkunan = (pUart_st->dmaSayac_u16 + pBuffer->len_u16 - pDma->Instance->CNDTR);
	}
	pUart_st->dmaSayac_u16 = pDma->Instance->CNDTR;

	ringbuffer_shift_writePtr(pBuffer, tOkunan);
}

void uart_yaz(UART_HandleTypeDef *pUart, uart_t *pUart_st, ringbuffer_t *pBuffer)
{
	uint8_t tVeri[UART_MAKS_BOYUT] = {0};
	uint32_t tBoyut = pBuffer->pending_u16;

	ringbuffer_read(pBuffer, tVeri, pBuffer->pending_u16);

	if(HAL_OK == HAL_UART_Transmit(pUart, tVeri, tBoyut, 100))
	{
	}

}

void DmaVeriGonder(UART_HandleTypeDef *pUart, uart_t *pUart_st, uint8_t *pBuffer, uint16_t boyut_u16)
{
	if(pUart_st->txCallBackFlag_u8)
	{
		HAL_UART_Transmit_DMA(pUart, pBuffer, boyut_u16);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

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
		Glo_st.usart2_st.txCallBackFlag_u8 = 1;
	}
	if (huart->Instance == USART3)
	{
		Glo_st.usart3_st.txCallBackFlag_u8 = 1;
	}

}
