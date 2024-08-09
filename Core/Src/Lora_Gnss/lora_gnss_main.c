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

Global_t GL = {0};


void loraGnssMain()
{

	DmaBaslat(&GL.usart2_st);
	DmaBaslat(&GL.usart3_st);

	GL.lora_st.adres_u8 = 0x01;
	GL.lora_st.kanal_u8 = 0x04;

	while(1)
	{
		if(1 == GL._50HzFlag_u8)
		{
			DmaVeriOku(&GL.usart2_st);
			DmaVeriOku(&GL.usart3_st);


			veri_paket_coz(&GL.usart2_st.dma_st.rxRingbuffer_st, &GL.veri_pkt_st);

			UbloxVeriYakala(&GL.usart2_st.dma_st.rxRingbuffer_st,
					        &GL.usart3_st.dma_st.txRingbuffer_st);

			LoraVeriGonder(&GL.usart3_st.dma_st.txRingbuffer_st,
					       &GL.veri_pkt_st,
						   &GL.lora_st);



			GL._50HzFlag_u8 = 0;
		}

		if(1 == GL._10HzFlag_u8)
		{

//			RingbufferDan_veri_Gonder(&Glo_st.usart3_txRingBuffer_st);

			GL._10HzFlag_u8 = 0;
		}

	}
}
