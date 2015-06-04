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
#include <util/delay.h>

#define BAUD 4800
#define MYUBRR F_CPU/16/BAUD-1

#define DEBUG 0

#define PAYLOAD_LENGTH      10

#if DEBUG == 1
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
    UBRRH = (_ubrr >> 8);
    UBRRL = _ubrr;                        // Set Baud rate
    UCSRB |= (1 << RXEN) | (1 << TXEN);// Enable The receiver and transmitter
    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);// Character Size: 8-bit
}

/*
 *  This function writes the given "data" to the USART which then transmit it
 *  via TX line
 */
int uart_putchar(char _ch, FILE *_stream) {
    if (_ch == '\n')
    uart_putchar('\r', _stream);
    while (!(UCSRA & (1 << UDRE)))
    ;
    UDR = _ch;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);
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
    debug_msg("\n\n***  NRF24l01+ TX test  ***\n\n");

    uint8_t tx_address_m1[5] = { 0xA2, 0xA0, 0xA0, 0xA0, 0xA0 };
    uint8_t tx_address_m2[5] = { 0xA1, 0xA0, 0xA0, 0xA0, 0xA0 };
    uint8_t rx_address[5] = { 0xA0, 0xA0, 0xA0, 0xA0, 0xA0 };

    NRF24_init();
    // Confidure radio
    NRF24_setDataRate(DATA_RATE_1MBPS);
    NRF24_setRFChannel(13);
    NRF24_setPowerAmplifier(RF_PWR_0DBM);
    NRF24_setCRCEncodingScheme(ENCODING_SCHEME_1BYTE);
    NRF24_enableCRC();
    NRF24_setAddressLength(ADDRESS_5_BYTES);
    NRF24_setRxAddress(rx_address, 5, DATA_PIPE_1);
    NRF24_enableDataPipe(DATA_PIPE_0);
    NRF24_enableDataPipe(DATA_PIPE_1);
    NRF24_enableAutoACK(DATA_PIPE_0);
    NRF24_enableAutoACK(DATA_PIPE_1);
    NRF24_setPayloadLength(DATA_PIPE_1, PAYLOAD_LENGTH);
    NRF24_setAutoRetransmissionTrials(15);
    NRF24_setAutoRetransmissionDelay(4);
    NRF24_RxMode();

    DDRA |= (1 << PA0) | (1 << PA1);
    PORTA &= ~(1 << PA0) & ~(1 << PA1);
    _delay_ms(500);
    PORTA |= (1 << PA0) | (1 << PA1);
    _delay_ms(500);
    PORTA &= ~(1 << PA0) & ~(1 << PA1);
    _delay_ms(500);
    PORTA |= (1 << PA0) | (1 << PA1);

    uint8_t data[PAYLOAD_LENGTH];
    uint8_t i;

    for (i = 0; i < PAYLOAD_LENGTH; i++)
        data[i] = 0x00;

    while (1) {
        // send data to M1 address
        NRF24_setTxAddress(tx_address_m1, 5);
        NRF24_setRxAddress(tx_address_m1, 5, DATA_PIPE_0); // for auto ACK
        PORTA |= (1 << PA0) | (1 << PA1);
        // send data
        data[0] = tx_address_m1[0];
        NRF24_sendData(data, PAYLOAD_LENGTH);
        // wait for status update
        while (!(NRF24_getStatus() & ((1 << MAX_RT_BIT) | (1 << TX_DS_BIT))))
            ;
        // check transmission status
        if (NRF24_getStatus() & (1 << TX_DS_BIT)) {
            debug_msg("> Transmission went OK\r\n");
            PORTA &= ~(1 << PA0);
        } else if (NRF24_getStatus() & (1 << MAX_RT_BIT)) {
            debug_msg("> Message is lost ...\r\n");
            PORTA &= ~(1 << PA1);
        }

        NRF24_RxMode();
        _delay_ms(1000);

        // send data to M2 address
        NRF24_setTxAddress(tx_address_m2, 5);
        NRF24_setRxAddress(tx_address_m2, 5, DATA_PIPE_0); // for auto ACK
        PORTA |= (1 << PA0) | (1 << PA1);
        // send data
        data[0] = tx_address_m2[0];
        NRF24_sendData(data, PAYLOAD_LENGTH);
        // wait for status update
        while (!(NRF24_getStatus() & ((1 << MAX_RT_BIT) | (1 << TX_DS_BIT))))
            ;
        // check transmission status
        if (NRF24_getStatus() & (1 << TX_DS_BIT)) {
            debug_msg("> Transmission went OK\r\n");
            PORTA &= ~(1 << PA0);
        } else if (NRF24_getStatus() & (1 << MAX_RT_BIT)) {
            debug_msg("> Message is lost ...\r\n");
            PORTA &= ~(1 << PA1);
        }

        NRF24_RxMode();
        _delay_ms(1000);

        data[PAYLOAD_LENGTH -1]++;
    }
    return 0;
}
