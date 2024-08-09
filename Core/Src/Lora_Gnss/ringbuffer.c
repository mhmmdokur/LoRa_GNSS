#include "ringbuffer.h"


ringbuffer_result_t ringbuffer_init(uint8_t *pData, uint16_t len_u16, ringbuffer_t *pBuffer)
{
	if(len_u16 < 1)
	{
		return RINGBUFFER_RES_ERROR;
	}

	pBuffer->len_u16 = len_u16;
	pBuffer->pData_u8a = pData;
	pBuffer->readPtr_u16 = 0;
	pBuffer->writePtr_u16 = 0;
	pBuffer->pending_u16 = 0;
	pBuffer->status_et = RINGBUFFER_STATUS_IDLE;

	return RINGBUFFER_RES_OK;
}

void ringbuffer_write_byte(ringbuffer_t *pBuffer, uint8_t data_u8)
{
	if(pBuffer->writePtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->writePtr_u16 %= pBuffer->len_u16;
	}

	pBuffer->pData_u8a[pBuffer->writePtr_u16++] = data_u8;
	pBuffer->pending_u16++;

	if(pBuffer->writePtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->writePtr_u16 %= pBuffer->len_u16;
	}
	if(pBuffer->pending_u16 >= pBuffer->len_u16)
	{
		pBuffer->pending_u16 %= pBuffer->len_u16;
	}
}

ringbuffer_result_t ringbuffer_write(ringbuffer_t *pBuffer, uint8_t *pVeri, uint16_t len_u16)
{
	uint16_t tPtr_u16 = 0;

	if(len_u16 >= pBuffer->len_u16)
	{
		return RINGBUFFER_RES_ERROR;
	}

	for(tPtr_u16 = 0; tPtr_u16 < len_u16; tPtr_u16++)
	{
		ringbuffer_write_byte(pBuffer, pVeri[tPtr_u16]);
	}

	return RINGBUFFER_RES_OK;
}

ringbuffer_result_t ringbuffer_shift_writePtr(ringbuffer_t *pBuffer, uint16_t len_u16)
{
	if(len_u16 >= pBuffer->len_u16)
	{
		return RINGBUFFER_RES_ERROR;
	}

	pBuffer->pending_u16 += len_u16;
	pBuffer->writePtr_u16 += len_u16;
	if(pBuffer->writePtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->writePtr_u16 %= pBuffer->len_u16;
	}
	if(pBuffer->pending_u16 >= pBuffer->len_u16)
	{
		pBuffer->pending_u16 %= pBuffer->len_u16;
	}

	return RINGBUFFER_RES_OK;
}


void ringbuffer_read_char(ringbuffer_t *pBuffer, char *pVeri, uint16_t len_u16)
{
	uint16_t tPtr_u16 = 0;

	if(len_u16 >= pBuffer->len_u16)
	{
		len_u16 = pBuffer->len_u16;
	}

	for(tPtr_u16 = 0; tPtr_u16 < len_u16; tPtr_u16++)
	{
		if(pBuffer->readPtr_u16 >= pBuffer->len_u16)
		{
			pBuffer->readPtr_u16 %= pBuffer->len_u16;
		}
		pVeri[tPtr_u16] = pBuffer->pData_u8a[pBuffer->readPtr_u16++];
		if(pBuffer->pending_u16 > 0)
		{
			pBuffer->pending_u16--;
		}

		if(pBuffer->readPtr_u16 >= pBuffer->len_u16)
		{
			pBuffer->readPtr_u16 %= pBuffer->len_u16;
		}
	}
}

uint8_t ringbuffer_read_byte(ringbuffer_t *pBuffer)
{
	uint8_t tVeri_u8 = 0;

	if(pBuffer->readPtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->readPtr_u16 %= pBuffer->len_u16;
	}
	tVeri_u8 = pBuffer->pData_u8a[pBuffer->readPtr_u16++];
	if(pBuffer->pending_u16 > 0)
	{
		pBuffer->pending_u16--;
	}

	if(pBuffer->readPtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->readPtr_u16 %= pBuffer->len_u16;
	}

	return tVeri_u8;
}

ringbuffer_result_t ringbuffer_read(ringbuffer_t *pBuffer, uint8_t *pVeri, uint16_t len_u16)
{
	uint16_t tPtr_u16 = 0;

	if(len_u16 >= pBuffer->len_u16)
	{
		return RINGBUFFER_RES_ERROR;
	}

	for(tPtr_u16 = 0; tPtr_u16 < len_u16; tPtr_u16++)
	{
		pVeri[tPtr_u16] = ringbuffer_read_byte(pBuffer);
	}

	return RINGBUFFER_RES_OK;
}

ringbuffer_result_t ringbuffer_shift_readPtr(ringbuffer_t *pBuffer, uint16_t len_u16)
{
	if(len_u16 >= pBuffer->len_u16)
	{
		return RINGBUFFER_RES_ERROR;
	}

	pBuffer->pending_u16 -= len_u16;
	pBuffer->readPtr_u16 += len_u16;
	if(pBuffer->readPtr_u16 >= pBuffer->len_u16)
	{
		pBuffer->readPtr_u16 %= pBuffer->len_u16;
	}
	if(pBuffer->pending_u16 >= pBuffer->len_u16)
	{
		pBuffer->pending_u16 %= pBuffer->len_u16;
	}

	return RINGBUFFER_RES_OK;
}
