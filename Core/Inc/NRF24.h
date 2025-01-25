#ifndef NRF24_H_
#define NRF24_H_
//------------------------------------------------
#include "stm32f1xx_hal.h"
#include "app_lev_stend.h"
#include <stdio.h>
#include <string.h>

union  un_field_struct{
  
uint16_t rx_result_word;
  
  struct{
    
    unsigned a0:1;
    unsigned a1:1;
    unsigned a2:1;
    unsigned a3:1;
    unsigned a4:1;
    unsigned a5:1;
    unsigned a6:1;
    unsigned a7:1;
    unsigned a8:1;
    unsigned a9:1;
    unsigned a10:1;
    unsigned a11:1;
    unsigned a12:1;
    unsigned a13:1;
    unsigned a14:1;
    unsigned a15:1;
    
  }bits_fld;
  
}; 


#define TX_ADR_WIDTH 3
#define TX_PLOAD_WIDTH 2

#define	START_TIM_DELAY_NRF		HAL_TIM_Base_Start(&htim2) 


//------------Gpio config spi-------------
#define SPI_DATA_TYPE uint8_t

#define	SPI_SOFT_SCK_Pin	GPIO_PIN_5	
#define	SPI_SOFT_SDI_Pin	GPIO_PIN_6
#define	SPI_SOFT_SDO_Pin	GPIO_PIN_7

#define	SPI_SOFT_SDI_GPIO_Port	GPIOA
#define	SPI_SOFT_SCK_GPIO_Port	GPIOA
#define	SPI_SOFT_SDO_GPIO_Port 	GPIOA

//------------------Gpio config nrf---------------------------
#define CS_GPIO_PORT GPIOA
#define CS_PIN GPIO_PIN_4
#define CS_ON HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET)
#define CS_OFF HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET)
#define CE_GPIO_PORT GPIOA
#define CE_PIN GPIO_PIN_2
#define CE_RESET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_RESET)
#define CE_SET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_SET)
#define IRQ_GPIO_PORT GPIOA
#define IRQ_PIN_NRF GPIO_PIN_3
#define IRQ HAL_GPIO_ReadPin(IRQ_GPIO_PORT, IRQ_PIN_NRF)
#define LED_GPIO_PORT GPIOB
#define LED_PIN GPIO_PIN_0
#define LED_ON HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define EXTI_IRQ_NRF EXTI3_IRQn


#define ENABLE_IT_EXTI_IRQ_NRF	HAL_NVIC_EnableIRQ(EXTI_IRQ_NRF) 
#define	DISABLE_IT_EXTI_IRQ_NRF	HAL_NVIC_DisableIRQ(EXTI_IRQ_NRF) 

//-----------------NRF24 REGISTERS-------------------------
#define ACTIVATE 0x50 //
#define RD_RX_PLOAD 0x61 // Define RX payload register address
#define WR_TX_PLOAD 0xA0 // Define TX payload register address
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
//------------------------------------------------
#define CONFIG 0x00 //'Config' register address
#define EN_AA 0x01 //'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02 //'Enabled RX addresses' register address
#define SETUP_AW 0x03 //'Setup address width' register address
#define SETUP_RETR 0x04 //'Setup Auto. Retrans' register address
#define RF_CH 0x05 //'RF channel' register address
#define RF_SETUP 0x06 //'RF setup' register address
#define STATUS 0x07 //'Status' register address
#define OBSERVE_TX 0x08 //'Transmit observe' register
#define RX_ADDR_P0 0x0A //'RX address pipe0' register address
#define RX_ADDR_P1 0x0B //'RX address pipe1' register address
#define TX_ADDR 0x10 //'TX address' register address
#define RX_PW_P0 0x11 //'RX payload width, pipe0' register address
#define RX_PW_P1 0x12 //'RX payload width, pipe1' register address
#define FIFO_STATUS 0x17 //'FIFO Status Register' register address
#define DYNPD 0x1C
#define FEATURE 0x1D
//------------------------------------------------
#define PRIM_RX 0x00 //RX/TX control (1: PRX, 0: PTX)
#define PWR_UP 0x01 //1: POWER UP, 0:POWER DOWN
#define RX_DR 0x40 //Data Ready RX FIFO interrupt
#define TX_DS 0x20 //Data Sent TX FIFO interrupt
#define MAX_RT 0x10 //Maximum number of TX retransmits interrupt
//------------------------------------------------
#define W_REGISTER 0x20 //запись в регистр
//------------------------------------------------

void Nrf24Init(void);
uint8_t NRF24_ReadReg(uint8_t addr);
void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes);
void NrfRxCallbackHandler(void);
void DelayMicro(__IO uint32_t us);
void NRF24_WriteReg(uint8_t addr, uint8_t dt); 
extern uint8_t first_show_maim_menu ; 
extern uint8_t RX_BUF[] ;
//------------------------------------------------
#endif /* NRF24_H_ */
