/*
 * protokol.c
 *
 *  Created on: Jul 25, 2024
 *      Author: Muhammed
 */

#include"protokol.h"
#include"lora_gnss_main.h"
#include"rtcm_decoder.h"



void veri_paketle( uint8_t *data, uint8_t uzunluk, veri_paketi_t *veri_pkt )
{
	static uint8_t durum_u8 = BASLANGIC_BAYT_1;
	uint16_t indeks_u16   = 0;

	for( int i = 1; i < DURUM_SAYISI; i++ )
	{
		switch( durum_u8 )
		{
			case BASLANGIC_BAYT_1:
			{
				veri_pkt->data[indeks_u16++] = 0x4D;
				durum_u8 =  BASLANGIC_BAYT_2;
				break;
			}
			case BASLANGIC_BAYT_2:
			{
				veri_pkt->data[indeks_u16++] = 0x26;
				durum_u8 =  UZUNLUK;
				break;
			}
			case UZUNLUK:
			{
				veri_pkt->data[indeks_u16++] = uzunluk;
				veri_pkt->veri_boyutu_u8 = uzunluk;

				durum_u8 =  DATA_AL;
				break;
			}
			case DATA_AL:
			{
				for( int k=0; k < uzunluk; k++)
				{
					veri_pkt->data[indeks_u16++] = data[k];
				}

				durum_u8 =  CRC_MSB;
				break;
			}
			case CRC_MSB:
			{
				uint16_t tCrc_u16 = crc16_ccitt(&veri_pkt->data[3], indeks_u16-3);  //ayrı bir fonksiyon yap LORA başllangıç caseini kaldır

				veri_pkt->data[indeks_u16++]  = ((tCrc_u16 >> 8) & 0xFF);
				veri_pkt->data[indeks_u16]    = (tCrc_u16 & 0xFF);
				veri_pkt->crc_u16 = tCrc_u16;

				durum_u8 =  BASLANGIC_BAYT_1;
				indeks_u16 = 0;
				break;
			}
		}
	}
}


void veri_paket_coz(ringbuffer_t *pBuffer, veri_paketi_t *veri_pkt )
{
	static uint8_t durum_u8  = BASLANGIC_BAYT_1;
	static uint16_t indeks_u16 = 0;
	static uint8_t gelenCrcMsb_u8  = 0;
	static uint8_t gelenCrcLsb_u8  = 0;
	uint8_t tVeri_u8 = 0;
	uint8_t tBuf_u8a[2048] = {0};
	uint16_t tVeriSayaci_u16 = 0;
	uint8_t okunanVeri_u8 = pBuffer->pending_u16;

	ringbuffer_read(pBuffer, tBuf_u8a, okunanVeri_u8);

	while(okunanVeri_u8 > 0)
	{
		tVeri_u8 = tBuf_u8a[tVeriSayaci_u16++];

		switch( durum_u8 )
		{
			case BASLANGIC_BAYT_1:
			{
				if( tVeri_u8 ==  0x4D )
				{
					durum_u8 =  BASLANGIC_BAYT_2;
				}

				break;
			}
			case BASLANGIC_BAYT_2:
			{
				if( tVeri_u8 ==  0x26 )
				{
					durum_u8 =  UZUNLUK;
				}
				else //makina sifirlanir
				{
					durum_u8  = BASLANGIC_BAYT_1;
				}

				break;
			}
			case UZUNLUK:
			{
				veri_pkt->veri_boyutu_u8  = tVeri_u8;

				if( veri_pkt->veri_boyutu_u8 > 0 )
				{
					durum_u8 =  DATA_AL;
				}
				else //makina sifirlanir
				{
					durum_u8  = BASLANGIC_BAYT_1;
				}
				break;
			}
			case DATA_AL:
			{
				veri_pkt->data[indeks_u16++] = tVeri_u8;

				if( indeks_u16 == veri_pkt->veri_boyutu_u8 )
				{
					durum_u8 =  CRC_MSB;
				}

				break;
			}
			case CRC_MSB:
			{
				gelenCrcMsb_u8 = tVeri_u8;

				durum_u8  =  CRC_LSB;
				break;
			}
			case CRC_LSB:
			{

				gelenCrcLsb_u8 = tVeri_u8;

				veri_pkt->crc_u16 = (gelenCrcMsb_u8 << 8) | (gelenCrcLsb_u8);

				if(veri_pkt->crc_u16 == crc16_ccitt(veri_pkt->data, indeks_u16))
				{
					parse_rtcm_v3_message(veri_pkt->data, indeks_u16, &GL.rtcm_st);
					veri_pkt->yakalanan_paket_u32++;
				}
				else
				{
					veri_pkt->hatali_paket_u32++;
				}
				//makina sifirlanir
				durum_u8  =  BASLANGIC_BAYT_1;
				indeks_u16 = 0;
				gelenCrcLsb_u8 = 0;
				gelenCrcMsb_u8 = 0;

				break;
			}
		}
	}
}


uint16_t crc16_ccitt(const uint8_t* buffer, size_t size)
{
    uint16_t crc = 0;
    while (size-- > 0)
    {
    	crc = (crc << 8) ^ ccitt_hash[((crc >> 8) ^ *(buffer++)) & 0x00FF];
    }
    return crc;
}



void Lora_paketle(veri_paketi_t *pVeri_pkt, Lora_t *pLora_st)
{
	pLora_st->data[0] = 0x00;
	pLora_st->data[1] = pLora_st->adres_u8;
	pLora_st->data[2] = pLora_st->kanal_u8;

	for(uint16_t i=0; i < (pVeri_pkt->veri_boyutu_u8+PROTOKOL_FAZLALIK); i++)
	{
		pLora_st->data[3+i] = pVeri_pkt->data[i];
	}

	pLora_st->veri_boyutu_u8 = (pVeri_pkt->veri_boyutu_u8 +
			                    LORA_E22_FAZLALIK +
							    PROTOKOL_FAZLALIK);
}



void UbloxVeriYakala(ringbuffer_t *pKaynak_st, ringbuffer_t *pHedef_st)
{
    uint8_t buffer[2048] = {0};
    uint16_t boyut = pKaynak_st->pending_u16;

    if(boyut > 0)
    {
    	ringbuffer_read(pKaynak_st, buffer, boyut);

    	ringbuffer_write(pHedef_st, buffer, boyut);
    }

}

void LoraVeriGonder(Uart_t *pUart_st, veri_paketi_t *pVeri_pkt, Lora_t *pLora_st )
{
	uint8_t buffer[200] = {0};
	uint16_t boyut = pUart_st->dma_st.txRingbuffer_st.pending_u16;

	if(boyut > 0)
	{
		if(boyut >200)
		{
			boyut = 200;
		}
		ringbuffer_read(&pUart_st->dma_st.txRingbuffer_st, buffer, boyut);

		veri_paketle(buffer, boyut, pVeri_pkt);
		Lora_paketle(pVeri_pkt, pLora_st);

		DmaVeriGonder(pUart_st->pUartHandle_st, pLora_st->data, pLora_st->veri_boyutu_u8);
	}

}











