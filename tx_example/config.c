/*
 *  config.c
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */

#include "config.h"

void NRF24_ioSetup(void) {
    CE_DDR |= (1 << CE_IO);
    CSN_DDR |= (1 << CSN_IO);
}

void NRF24_setCE(uint8_t state) {
    if (state) {
        CE_PORT |= (1 << CE_IO);
    } else {
        CE_PORT &= ~(1 << CE_IO);
    }
}

void NRF24_setCSN(uint8_t state) {
    if (state) {
        CSN_PORT |= (1 << CSN_IO);
    } else {
        CSN_PORT &= ~(1 << CSN_IO);
    }
}

void SPI_init(void) {
    SCK_DDR |= (1 << SCK_IO);
    MOSI_DDR |= (1 << MOSI_IO);
    MISO_DDR &= ~(1 << MISO_IO);
}

/*
 * Bit banging (Software SPI)
 */
uint8_t SPI_readWrite(uint8_t data) {
    uint8_t i = 0;
    uint8_t ret_val = 0;

    SCK_PORT &= ~(1 << SCK_IO);

    for (i = 0; i < 8; i++) {

        if (data & (1 << (7 - i))) {
            MOSI_PORT |= (1 << MOSI_IO);
        } else {
            MOSI_PORT &= ~(1 << MOSI_IO);
        }

        SCK_PORT |= (1 << SCK_IO);

        ret_val = ret_val << 1;
        if ((MISO_PIN & (1 << MISO_IO))) {
            ret_val |= 0x01;
        }

        SCK_PORT &= ~(1 << SCK_IO);
    }

    return ret_val;
}
