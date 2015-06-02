/*
 *  config.h
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */


#ifndef CONFIG__H_
#define CONFIG__H_

#include <avr/io.h>

#define CE_DDR      DDRC
#define CE_PORT     PORTC
#define CE_IO       0

#define CSN_DDR     DDRC
#define CSN_PORT    PORTC
#define CSN_IO      1

#define SCK_DDR     DDRC
#define SCK_PORT    PORTC
#define SCK_IO      2

#define MOSI_DDR    DDRC
#define MOSI_PORT   PORTC
#define MOSI_IO     3

#define MISO_DDR    DDRC
#define MISO_PIN    PINC
#define MISO_IO     4

//#define IRQ_DDR    DDRC
//#define IRQ_PORT   PORTC
//#define IRQ_PIN    PINC
//#define IRQ_IO     5

#endif /* CONFIG__H_ */
