/*
 *  nrf24l01_plus.c
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */

#include "nrf24l01_plus.h"
#include <stdlib.h>

static uint8_t payload_legth[6];

static void NRF24_readRegister(uint8_t reg, uint8_t *value, uint8_t length);
static void NRF24_writeRegister(uint8_t reg, uint8_t *value, uint8_t length);
static void NRF24_writeCommand(uint8_t reg);

void NRF24_init(void) {
    NRF24_ioSetup();
    SPI_init();
    NRF24_setCE(LOW);
    NRF24_setCSN(HIGH);
}

void NRF24_setDataRate(uint8_t data_rate) {
    uint8_t rf_setup_register;

    NRF24_readRegister(RF_SETUP, &rf_setup_register, 1);
    if (data_rate == DATA_RATE_250KBPS) {
        rf_setup_register |= (1 << RF_DR_LOW_BIT);
    } else if (data_rate == DATA_RATE_1MBPS) {
        rf_setup_register &= ~(1 << RF_DR_LOW_BIT);
        rf_setup_register &= ~(1 << RF_DR_HIGH_BIT);
    } else if (data_rate == DATA_RATE_2MBPS) {
        rf_setup_register &= ~(1 << RF_DR_LOW_BIT);
        rf_setup_register |= (1 << RF_DR_HIGH_BIT);
    }
    NRF24_writeRegister(RF_SETUP, &rf_setup_register, 1);
}

void NRF24_setRFChannel(uint8_t channel) {
    if (channel > MAX_RF_CHANNEL)
        return;
    uint8_t rf_ch_register = RF_CH_MASK & channel;
    NRF24_writeRegister(RF_CH, &rf_ch_register, 1);
}

void NRF24_setPowerAmplifier(uint8_t rf_power) {
    uint8_t rf_setup_register;

    NRF24_readRegister(RF_SETUP, &rf_setup_register, 1);
    rf_setup_register &= ~RF_PWR_MASK;
    rf_setup_register |= RF_PWR_MASK & (rf_power << RF_PWR_BIT);
    NRF24_writeRegister(RF_SETUP, &rf_setup_register, 1);
}

void NRF24_disableAutoACK(uint8_t data_pipe) {
    uint8_t en_aa_register;

    if (data_pipe > 5)
        return;
    NRF24_readRegister(EN_AA, &en_aa_register, 1);
    en_aa_register &= ~(1 << data_pipe);
    NRF24_writeRegister(EN_AA, &en_aa_register, 1);
}

void NRF24_enableAutoACK(uint8_t data_pipe) {
    uint8_t en_aa_register;

    if (data_pipe > 5)
        return;
    NRF24_readRegister(EN_AA, &en_aa_register, 1);
    en_aa_register |= (1 << data_pipe);
    NRF24_writeRegister(EN_AA, &en_aa_register, 1);
}

void NRF24_setAutoRetransmissionDelay(uint8_t delay) {
    uint8_t setup_retr_register;

    if (delay > 15)
        return;
    NRF24_readRegister(SETUP_RETR, &setup_retr_register, 1);
    setup_retr_register &= ~ARD_MASK;   //clear value
    setup_retr_register |= ARD_MASK & (delay << ARD_BIT);   //set new value
    NRF24_writeRegister(SETUP_RETR, &setup_retr_register, 1);
}

void NRF24_setAutoRetransmissionTrials(uint8_t retransmit_count) {
    uint8_t setup_retr_register;

    if (retransmit_count > 15)
        return;
    NRF24_readRegister(SETUP_RETR, &setup_retr_register, 1);
    setup_retr_register &= ~ARC_MASK;
    setup_retr_register |= ARC_MASK & (retransmit_count << ARC_BIT);
    NRF24_writeRegister(SETUP_RETR, &setup_retr_register, 1);
}

void NRF24_enableCRC(void) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register |= (1 << EN_CRC_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
}

void NRF24_disableCRC(void) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register &= ~(1 << EN_CRC_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
}

void NRF24_setCRCEncodingScheme(uint8_t encoding_scheme) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    if (encoding_scheme)
        config_register |= (1 << CRCO_BIT);
    else
        config_register &= ~(1 << CRCO_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
}

void NRF24_enableContWave(void) {
    uint8_t rf_setup_register;

    NRF24_readRegister(RF_SETUP, &rf_setup_register, 1);
    rf_setup_register |= (1 << CONT_WAVE_BIT);
    NRF24_writeRegister(RF_SETUP, &rf_setup_register, 1);
}

void NRF24_disableContWave(void) {
    uint8_t rf_setup_register;

    NRF24_readRegister(RF_SETUP, &rf_setup_register, 1);
    rf_setup_register &= ~(1 << CONT_WAVE_BIT);
    NRF24_writeRegister(RF_SETUP, &rf_setup_register, 1);
}

void NRF24_setAddressLength(uint8_t address_length) {
    uint8_t aw_register = address_length;

    NRF24_writeRegister(SETUP_AW, &aw_register, 1);
}

void NRF24_setRxAddress(uint8_t* address, uint8_t address_length,
        uint8_t data_pipe) {
    uint8_t i, *reverse_address;

    reverse_address = (uint8_t*) malloc(address_length);
    // reverse the array, NRF24L01+ expects LSB first
    for (i = 0; i < address_length; i++) {
        reverse_address[i] = address[address_length - 1 - i];
    }
    switch (data_pipe) {
    case DATA_PIPE_0:
        NRF24_writeRegister(RX_ADDR_P0, reverse_address, address_length);
        break;
    case DATA_PIPE_1:
        NRF24_writeRegister(RX_ADDR_P1, reverse_address, address_length);
        break;
    case DATA_PIPE_2:
        // Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
        NRF24_writeRegister(RX_ADDR_P2, reverse_address, 1);
        break;
    case DATA_PIPE_3:
        // Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
        NRF24_writeRegister(RX_ADDR_P3, reverse_address, 1);
        break;
    case DATA_PIPE_4:
        // Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
        NRF24_writeRegister(RX_ADDR_P4, reverse_address, 1);
        break;
    case DATA_PIPE_5:
        // Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
        NRF24_writeRegister(RX_ADDR_P5, reverse_address, 1);
        break;
    }
    free(reverse_address);
}

void NRF24_setTxAddress(uint8_t* address, uint8_t address_length) {
    uint8_t i, *reverse_address;

    reverse_address = (uint8_t*) malloc(address_length);
    // reverse the array, NRF24L01+ expects LSB first
    for (i = 0; i < address_length; i++) {
        reverse_address[i] = address[address_length - 1 - i];
    }
    NRF24_writeRegister(TX_ADDR, reverse_address, address_length);
    free(reverse_address);
}

void NRF24_enableDataPipe(uint8_t data_pipe) {
    uint8_t en_rxaddr_register;

    NRF24_readRegister(EN_RXADDR, &en_rxaddr_register, 1);
    en_rxaddr_register |= (1 << data_pipe);
    NRF24_writeRegister(EN_RXADDR, &en_rxaddr_register, 1);

}

void NRF24_disableDataPipe(uint8_t data_pipe) {
    uint8_t en_rxaddr_register;

    NRF24_readRegister(EN_RXADDR, &en_rxaddr_register, 1);
    en_rxaddr_register &= ~(1 << data_pipe);
    NRF24_writeRegister(EN_RXADDR, &en_rxaddr_register, 1);
}

void NRF24_setPayloadLength(uint8_t data_pipe, uint8_t length) {
    if (length > 32)
        return;
    payload_legth[data_pipe] = length;
    switch (data_pipe) {
    case DATA_PIPE_0:
        NRF24_writeRegister(RX_PW_P0, &length, 1);
        break;
    case DATA_PIPE_1:
        NRF24_writeRegister(RX_PW_P1, &length, 1);
        break;
    case DATA_PIPE_2:
        NRF24_writeRegister(RX_PW_P2, &length, 1);
        break;
    case DATA_PIPE_3:
        NRF24_writeRegister(RX_PW_P3, &length, 1);
        break;
    case DATA_PIPE_4:
        NRF24_writeRegister(RX_PW_P4, &length, 1);
        break;
    case DATA_PIPE_5:
        NRF24_writeRegister(RX_PW_P5, &length, 1);
        break;
    }
}

void NRF24_RxMode(void) {
    uint8_t temp_register;

    NRF24_writeCommand(FLUSH_RX);
    NRF24_readRegister(CONFIG, &temp_register, 1);
    temp_register |= (1 << PWR_UP_BIT) | (1 << PRIM_RX_BIT);
    NRF24_writeRegister(CONFIG, &temp_register, 1);
    NRF24_setCE(HIGH);

    // reset RX_DR_BIT in status register
    NRF24_readRegister(STATUS, &temp_register, 1);
    temp_register |= (1 << RX_DR_BIT);
    NRF24_writeRegister(STATUS, &temp_register, 1);
}

void NRF24_standbyI(void) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register |= (1 << PWR_UP_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
    NRF24_setCE(LOW);
}

void NRF24_standbyII(void) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register |= (1 << PWR_UP_BIT);
    config_register &= (1 << PRIM_RX_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
    NRF24_setCE(HIGH);
}

void NRF24_powerDown(void) {
    uint8_t config_register;

    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register &= ~(1 << PWR_UP_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
}

uint8_t NRF24_getStatus(void) {
    uint8_t status_register;

    NRF24_readRegister(STATUS, &status_register, 1);
    return status_register;
}

void NRF24_sendData(uint8_t *data, uint8_t length) {
    uint8_t config_register, status_register, i;

    if (length > 32)
        return;

    NRF24_setCE(LOW);
    //go to Tx mode
    NRF24_readRegister(CONFIG, &config_register, 1);
    config_register |= (1 << PWR_UP_BIT);
    config_register &= ~(1 << PRIM_RX_BIT);
    NRF24_writeRegister(CONFIG, &config_register, 1);
    NRF24_writeCommand(FLUSH_TX);
    status_register = NRF24_getStatus();
    // clear status bits
    status_register |= (1 << MAX_RT_BIT) | (1 << TX_DS_BIT) | (1 << RX_DR_BIT);
    NRF24_writeRegister(STATUS, &status_register, 1);
    NRF24_setCSN(LOW);
    // Send data to nrf
    SPI_readWrite(W_TX_PAYLOAD);
    for (i = 0; i < length; i++) {
        SPI_readWrite(data[i]); // Send data to nrf
    }
    NRF24_setCSN(HIGH);
    // start transmission
    NRF24_setCE(HIGH);
}

void NRF24_getData(uint8_t *data_pipe, uint8_t *data) {
    uint8_t status_register, pipe, i;

    status_register = NRF24_getStatus();
    if (status_register & (1 << RX_DR_BIT)) {
        //get the pipe number
        pipe = (status_register >> RX_P_NO_BIT) & RX_P_NO_MASK;
        if (pipe <= 0x05) {
            *data_pipe = pipe;
            //there is some data on pipe, get it
            NRF24_setCSN(LOW);
            // Send data to nrf
            SPI_readWrite(R_RX_PAYLOAD);
            for (i = 0; i < payload_legth[pipe]; i++) {
                data[i] = SPI_readWrite(NOP);
            }
            NRF24_setCSN(HIGH);
            //reset data ready RX FIFO interrupt
            status_register |= (1 << RX_DR_BIT);
            NRF24_writeRegister(STATUS, &status_register, 1);

            //handle ack payload receipt
            if (status_register & (1 << TX_DS_BIT)) {
                status_register |= (1 << TX_DS_BIT);
                NRF24_writeRegister(STATUS, &status_register, 1);
            }
        }
    }
}

/*
 * ****************************************************************************
 * ********************  Implementation of static functions  *******************
 * ****************************************************************************
 */

void NRF24_readRegister(uint8_t reg, uint8_t *value, uint8_t length) {
    uint8_t i;
    NRF24_setCSN(LOW);
    // Send data to nrf
    SPI_readWrite(R_REGISTER | (reg & RW_REGISTER_MASK));
    for (i = 0; i < length; i++) {
        value[i] = SPI_readWrite(NOP);
    }
    NRF24_setCSN(HIGH);
}

void NRF24_writeRegister(uint8_t reg, uint8_t *value, uint8_t length) {
    uint8_t i;

    NRF24_setCSN(LOW);
    // Send data to nrf
    SPI_readWrite(W_REGISTER | (reg & RW_REGISTER_MASK));
    for (i = 0; i < length; i++) {
        SPI_readWrite(value[i]);
    }
    NRF24_setCSN(HIGH);
}

void NRF24_writeCommand(uint8_t command) {
    NRF24_setCSN(LOW);
    SPI_readWrite(command);
    NRF24_setCSN(HIGH);
}
