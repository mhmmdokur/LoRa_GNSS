
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
#define UART_TX_MAKS_BOYUT 2048U
#define UART_MAKS_BOYUT	   UART_RX_MAKS_BOYUT



typedef struct
{
	uint16_t dmaSayac_u16;
	ringbuffer_t txRingbuffer_st;
	ringbuffer_t rxRingbuffer_st;
}Dma_t;

typedef struct
{
	uint8_t txCallBackFlag_u8;
	uint8_t rxBuffer[UART_RX_MAKS_BOYUT];
	uint8_t txBuffer[UART_TX_MAKS_BOYUT];
	Dma_t dma_st;
}Uart_t;



void DmaVeriOku(Uart_t *pUart_st, DMA_HandleTypeDef *dmaHandler);
void DmaBaslat(Uart_t *pUart_st, UART_HandleTypeDef *pUartHandle);
void DmaVeriGonder(UART_HandleTypeDef *pUartHandle, uint8_t *pBuffer, uint16_t boyut_u16);

#endif /* INC_UART_H_ */
