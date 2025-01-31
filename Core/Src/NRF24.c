/*******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Technology OFS.
 * All rights reserved.
 *
 * The information contained herein is confidential
 * property of Technology OFS.
 * The use, copying, transfer or disclosure of such
 * information is prohibited except by express written
 * agreement with Technology OFS.
 ******************************************************************************
 * @file    NRF24.c
 * @brief   pulse generation module for stepper motor.
 * @author  Karimov S.
 * @date    2025.01.25
 * 
 * @details
 * 
 * Module Description:
 * 	The library for controlling the nRF24l01 radio module contains all the 
 * 	necessary functions for configuration, management and initialization
 * 
 * 
 * Need to perform :
 *     TODO 
 * 
 * Log: 
 *     Version 1.0.0.0: Initial version.  
 *         
 * 
 * Known Bugs:
 *     This module is sensitive to power supply 
 * 
 * Fixed Bugs:
 *     BUG FIXED dd.mm.yyyy 431 Description
 *
 ******************************************************************************/
 
/************************************
 * INCLUDES
 ************************************/
#include "NRF24.h"
 
 
 
/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};






/************************************
 * GLOBAL VARIABLES
 ************************************/
uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};
union un_field_struct nrf_bits_field_rxdata;
uint16_t dt=0; 





/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
/**
  * @brief Software spi
  * @param data data to write to spi
  * @retval return value by spi
  */
static SPI_DATA_TYPE SPI_send_soft(SPI_DATA_TYPE data)
{
  
  SPI_DATA_TYPE result = 0;
  uint8_t i = sizeof(SPI_DATA_TYPE) * 8;                    
   
  while(i--){
          
    result <<= 1;
    if(data & (0x80)) {       
        SPI_SOFT_SDO_GPIO_Port->BSRR = SPI_SOFT_SDO_Pin;
    }else {
        SPI_SOFT_SDO_GPIO_Port->BRR = SPI_SOFT_SDO_Pin;
    }  
    
    DelayMicro(1);
    SPI_SOFT_SCK_GPIO_Port->BSRR = SPI_SOFT_SCK_Pin;
		if(SPI_SOFT_SDI_GPIO_Port->IDR & SPI_SOFT_SDI_Pin)
			result |= 1;
    DelayMicro(1);   
    SPI_SOFT_SCK_GPIO_Port->BRR = SPI_SOFT_SCK_Pin; 
		DelayMicro(1);
     
    data <<= 1;
		
  }  
  
   
  return result;
  
}



/**
  * @brief reading the register of the radio module
  * @param addr the address of the register to read
  * @retvalthe reading value
  */
static uint8_t NRF24_ReadReg(uint8_t addr)
{
  uint8_t dt=0, cmd;
  CS_ON;
  dt = SPI_send_soft(addr);
  if (addr!=STATUS) 
  {
    cmd = 0xFF;
    dt = SPI_send_soft(cmd);
  }
  CS_OFF;
  
  return dt;
}



/**
  * @brief writing a byte to the register of the radio module
  * @param addr the address of the register for recording
  * @param dt recording data
  * @retval None 
  */
static void NRF24_WriteReg(uint8_t addr, uint8_t dt)
{
  addr |= W_REGISTER; 
  CS_ON;
  SPI_send_soft(addr); 
  SPI_send_soft(dt); 
  CS_OFF;
}




/**
  * @brief activating commands R_RX_PL_WID, W_ACK_PAYLOAD and W_TX_PAYLOAD_NOACK
  * @param None
  * @retval None 
  */
static void NRF24_ToggleFeatures(void)
{
  uint8_t dt[1] = {ACTIVATE};
  CS_ON;
  SPI_send_soft(dt[0]);
  DelayMicro(1);
  dt[0] = 0x73;
  SPI_send_soft(dt[0]);
  CS_OFF;
}




/**
  * @brief reading data from a radio module
  * @param addr address of the register to read
  * @param pBuf pointer to the buffer
  * @param bytes number of bytes to read
  * @retval None
  */
static void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  CS_ON;
  SPI_send_soft( addr ); 
	for(uint8_t i=0; i<bytes; ++i){
		pBuf[i] = SPI_send_soft(0xff); 
	}
  
  CS_OFF;
}





/**
  * @brief recording data in a radio module
  * @param addr address of the register for recording
  * @param pBuf pointer to the buffer of written data
  * @param bytes number of bytes to write
  * @retval None
  */
static void NRF24_Write_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  addr |= W_REGISTER; 
  CS_ON;
  SPI_send_soft(addr); 
  DelayMicro(1);	
	for(uint8_t i=0; i<bytes; ++i){
		SPI_send_soft(pBuf[i]); 
	}
	
  CS_OFF;
}





/**
  * @brief Command to clear the receiving buffer
  * @param None
  * @retval None 
  */
static void NRF24_FlushRX(void)
{
  uint8_t dt[1] = {FLUSH_RX};
  CS_ON;
  SPI_send_soft(dt[0]);
  DelayMicro(1);
  CS_OFF;
}





/**
  * @brief Command to clear the transmiter buffer
  * @param None
  * @retval None 
  */
static void NRF24_FlushTX(void)
{
  uint8_t dt[1] = {FLUSH_TX};
  CS_ON;
  SPI_send_soft(dt[0]);
  DelayMicro(1);
  CS_OFF;
}



/**
  * @brief Setting the radio module to read mode
  * @param None  
  * @retval None
  */
static void NRF24L01_RX_Mode(void)
{
  uint8_t regval=0x00;
  regval = NRF24_ReadReg(CONFIG);
  
  regval |= (1<<PWR_UP)|(1<<PRIM_RX);
  NRF24_WriteReg(CONFIG,regval);
  CE_SET;
  DelayMicro(150);  
  // Flush buffers
  NRF24_FlushRX();
  NRF24_FlushTX();
}

 
 
 
/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
  * @brief Microsecond delay function
  * @param us number of microseconds  
  * @retval None
  */
void DelayMicro(__IO uint32_t us)
{
		TIM2->CNT = 0;	
    while(TIM2->CNT < us);
}



/**
  * @brief Handler for interrupting the reception of a radio packet
  * @param None   
  * @retval None
  */
void NrfRxCallbackHandler(void){
	
	uint8_t status = NRF24_ReadReg(STATUS);
	DelayMicro(10);
	status = NRF24_ReadReg(STATUS);
		
	if(status & RX_DR)
	{
			
			NRF24_WriteReg(STATUS, RX_DR);
			NRF24_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
			nrf_bits_field_rxdata.rx_result_word = (uint16_t)(*((uint16_t*)RX_BUF));
			
				 
			dt = nrf_bits_field_rxdata.rx_result_word & 0x01ff;
			dt = ((float)((float)dt / 512) * 360); 		 
			

			SynchrAngle();       	
			ShowAngleBitStat(); 	
  
	}	 
	
}




/**
  * @brief Initializing the radio module
  * @param None   
  * @retval None
  */
void Nrf24Init(void)
{
	
	CE_RESET;
	DelayMicro(5000);
	NRF24_WriteReg(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	DelayMicro(5000);
	NRF24_WriteReg(EN_AA, 0x02); // Enable Pipe1
	NRF24_WriteReg(EN_RXADDR, 0x02); // Enable Pipe1
	NRF24_WriteReg(SETUP_AW, 0x01); // Setup address width=3 bytes
	NRF24_WriteReg(SETUP_RETR, 0x5F); // // 1500us, 15 retrans
	NRF24_ToggleFeatures();
	NRF24_WriteReg(FEATURE, 0);
	NRF24_WriteReg(DYNPD, 0);
	NRF24_WriteReg(STATUS, 0x70); //Reset flags for IRQ
	NRF24_WriteReg(RF_CH, 76); //   2476 MHz
	NRF24_WriteReg(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_WriteReg(RX_PW_P1, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 1
  
	NRF24L01_RX_Mode();
	LED_OFF;
	
}
