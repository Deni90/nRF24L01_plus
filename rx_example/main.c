/*
 *  main.c
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */

#include <avr/io.h>
#include <stdio.h>
#include "../nrf24l01_plus/nrf24l01_plus.h"

#define DEBUG 1

#define PAYLOAD_LENGTH      10

#if DEBUG == 1

#define BAUD_RATE           9600
#define MYUBRR              F_CPU/16/BAUD_RATE-1

/*
 *  This function is used to initialize the USART0 at a given UBRR value.
 *  Set frame format:
 *      - Asynchronous USART
 *      - 1 start bit
 *      - 8 data bits
 *      - no parity bit
 *      - 1 stop bit
 */
void uart_init(unsigned int _ubrr) {
    UBRR0 = _ubrr;                              //Set Baud rate
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //Enable The receiver and transmitter
    UCSR0B |= (1 << RXCIE0);                    //RX Complete Interrupt Enable 0
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);    //Character Size: 8-bit
}

/*
 *  This function writes the given "data" to the USART which then transmit it via TX line
 */
int uart_putchar(char _ch, FILE *_stream) {
    if (_ch == '\n')
        uart_putchar('\r', _stream);
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    UDR0 = _ch;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#endif

int debug_msg(const char * format, ...) {
    if (DEBUG != 1)
        return 0;
    va_list args;
    va_start(args, format);
    int length = vprintf(format, args);
    va_end(args);
    return length;
}



int main(void) {
#if DEBUG == 1
    uart_init(MYUBRR);
    stdout = &mystdout;
#endif
    debug_msg("\n\n***  NRF24l01+ RX test  ***\n\n");

    uint8_t tx_address[5] = { 0xB2, 0xB2, 0xB2, 0xB2, 0xB2 };
    uint8_t rx_address[5] = { 0xA1, 0xA1, 0xA1, 0xA1, 0xA1 };

    NRF24_init();
    // Confidure radio
    NRF24_setDataRate(DATA_RATE_1MBPS);
    NRF24_setRFChannel(13);
    NRF24_setPowerAmplifier(RF_PWR_0DBM);
    NRF24_setCRCEncodingScheme(ENCODING_SCHEME_1BYTE);
    NRF24_enableCRC();
    NRF24_setAddressLength(ADDRESS_5_BYTES);
    NRF24_setTxAddress(tx_address, 5);
    NRF24_setRxAddress(rx_address, 5, DATA_PIPE_1);
    NRF24_setRxAddress(tx_address, 5, DATA_PIPE_0);
    NRF24_enableDataPipe(DATA_PIPE_0);
    NRF24_enableDataPipe(DATA_PIPE_1);
    NRF24_enableAutoACK(DATA_PIPE_0);
    NRF24_enableAutoACK(DATA_PIPE_1);
    NRF24_setPayloadLength(DATA_PIPE_1, PAYLOAD_LENGTH);
    NRF24_setAutoRetransmissionTrials(15);
    NRF24_setAutoRetransmissionDelay(4);
    NRF24_RxMode();

    uint8_t data_pipe = 0x00;
    uint8_t data[PAYLOAD_LENGTH];
    uint8_t i;

    while (1) {
        // check is some data received
        if (NRF24_getStatus() & (1 << RX_DR_BIT)) {
            NRF24_getData(&data_pipe, data);
            debug_msg("Incoming message on data pipe %d\n", data_pipe);
            debug_msg("Payload: [");
            for (i = 0; i < PAYLOAD_LENGTH; i++)
                debug_msg((i != PAYLOAD_LENGTH - 1) ? "%02X, " : "%02X",
                        data[i]);
            debug_msg("]\n");
        }
    }
    return 0;
}
