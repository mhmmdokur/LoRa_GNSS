/**
  ******************************************************************
  * @file    ringbuffer.h
  * @author  Yusuf Aydin
  * @date    04.08.2024
  * @brief   Ring Buffer Data Structure Header File
  ******************************************************************
  */

#ifndef EVYEPA_RINGBUFFER_H_
#define EVYEPA_RINGBUFFER_H_
#include "stm32f1xx_hal.h"


typedef enum
{
	RINGBUFFER_STATUS_WRITE = 0,
	RINGBUFFER_STATUS_READ,
	RINGBUFFER_STATUS_IDLE,
}ringbuffer_status_t;

typedef enum
{
	RINGBUFFER_RES_ERROR = 0,
	RINGBUFFER_RES_OK,
}ringbuffer_result_t;

typedef struct
{
	uint8_t *pData_u8a;
	uint16_t len_u16;
	uint16_t readPtr_u16;
	uint16_t writePtr_u16;
	uint16_t pending_u16;
	ringbuffer_status_t status_et;
}ringbuffer_t;


ringbuffer_result_t ringbuffer_init(uint8_t *pData, uint16_t len_u16, ringbuffer_t *pBuffer);

void ringbuffer_write_byte(ringbuffer_t *pBuffer, uint8_t data_u8);
ringbuffer_result_t ringbuffer_write(ringbuffer_t *pBuffer, uint8_t *pVeri, uint16_t len_u16);
ringbuffer_result_t ringbuffer_shift_writePtr(ringbuffer_t *pBuffer, uint16_t len_u16);

void ringbuffer_read_char(ringbuffer_t *pBuffer, char *pVeri, uint16_t len_u16);
uint8_t ringbuffer_read_byte(ringbuffer_t *pBuffer);
ringbuffer_result_t ringbuffer_read(ringbuffer_t *pBuffer, uint8_t *pVeri, uint16_t len_u16);
ringbuffer_result_t ringbuffer_shift_readPtr(ringbuffer_t *pBuffer, uint16_t len_u16);



#endif /* EVYEPA_RINGBUFFER_H_ */
