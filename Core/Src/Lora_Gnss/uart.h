
#ifndef EVYEPA_UART_H_
#define EVYEPA_UART_H_

#include "stm32f1xx_hal.h"
#include "ringbuffer.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;


#define UART_RX_MAKS_BOYUT 4096U
#define UART_TX_MAKS_BOYUT 1024U
#define UART_MAKS_BOYUT	   UART_RX_MAKS_BOYUT


typedef struct
{
	uint16_t dmaSayac_u16;
	uint8_t txCallBackFlag_u8;
}uart_t;

void uart_oku_dma(DMA_HandleTypeDef *pDma, uart_t *pUart_st, ringbuffer_t *pBuffer);
void uart_yaz(UART_HandleTypeDef *pUart, uart_t *pUart_st, ringbuffer_t *pBuffer);
void DmaVeriGonder(UART_HandleTypeDef *pUart, uart_t *pUart_st, uint8_t *pBuffer, uint16_t boyut_u16);

#endif /* INC_UART_H_ */
