/*
 *  nrf24l01_plus_registers.h
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */

#ifndef NRF24L01_PLUS_REGISTERS_H_
#define NRF24L01_PLUS_REGISTERS_H_

// Command set for the nRF24L01+ SPI
#define R_REGISTER          0x00
#define W_REGISTER          0x20
#define	R_RX_PAYLOAD        0x61
#define W_TX_PAYLOAD        0xA0
#define FLUSH_TX            0xE1
#define FLUSH_RX            0xE2
#define REUSE_TX_PL         0xE3
#define R_RX_PL_WID         0x60
#define W_ACK_PAYLOAD       0xA8
#define W_TX_PAYLOAD_NOACK  0xB0
#define NOP                 0xFF

#define RW_REGISTER_MASK    0x1F
#define W_ACK_PAYLOAD_MASK  0x07

// Register map of nRF24L01+
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD         0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

// Configuration Register
#define MASK_RX_DR_BIT  6
#define MASK_TX_DS_BIT  5
#define MASK_MAX_RT_BIT 4
#define EN_CRC_BIT      3
#define CRCO_BIT        2
#define PWR_UP_BIT      1
#define PRIM_RX_BIT     0

// Enable 'Auto Acknowledgment'
#define ENAA_P5_BIT 5
#define ENAA_P4_BIT 4
#define ENAA_P3_BIT 3
#define ENAA_P2_BIT 2
#define ENAA_P1_BIT 1
#define ENAA_P0_BIT 0

// Enabled RX Addresses
#define ERX_P5_BIT  5
#define ERX_P4_BIT  4
#define ERX_P3_BIT  3
#define ERX_P2_BIT  2
#define ERX_P1_BIT  1
#define ERX_P0_BIT  0

// Setup of Address Widths (common for all data pipes)
#define AW_BIT  0 // 2 bits

// Setup of Automatic Retransmission
#define ARD_BIT 4 // 4 bits
#define ARC_BIT 0 // 4 bits

#define ARD_MASK    0xF0
#define ARC_MASK    0x0F

// RF Channel
#define RF_CH_MASK 0x7F

// RF Setup Register
#define CONT_WAVE_BIT   7
#define RF_DR_LOW_BIT   5
#define PLL_LOCK_BIT    4
#define RF_DR_HIGH_BIT  3
#define RF_PWR_BIT      1   // 2 bits
#define RF_PWR_MASK     0x06

// Status Register
#define RX_DR_BIT   6
#define TX_DS_BIT   5
#define MAX_RT_BIT  4
#define RX_P_NO_BIT 1   // 3 bits
#define TX_FULL_BIT 0

#define RX_P_NO_MASK    0x07

// Transmit observe register
#define PLOS_CNT_BIT    4   // 4 bits
#define ARC_CNT_BIT	    0   // 4 bits

// FIFO Status Register
#define TX_REUSE_BIT    6
#define TX_FULL_BIT_    5
#define TX_EMPTY_BIT    4
#define RX_FULL_BIT     1
#define RX_EMPTY_BIT    0

// Enable dynamic payload length
#define DPL_P5_BIT  5
#define DPL_P4_BIT  4
#define DPL_P3_BIT  3
#define DPL_P2_BIT  2
#define DPL_P1_BIT  1
#define DPL_P0_BIT  0

// Feature Register
#define EN_DPL_BIT      2
#define EN_ACK_PAY_BIT  1
#define EN_DYN_ACK_BIT  0

#endif /* NRF24L01_PLUS_REGISTERS_H_ */
