/*
 *  nrf24l01_plus.h
 *
 *  "THE DON'T-CARE LICENCE"
 *  I don't care, do whatever you want with this file.
 *
 *  Author: Daniel Knezevic
 *  E-mail: knezevic_daniel@hotmail.com
 */

#ifndef NRF24L01_PLUS__H_
#define NRF24L01_PLUS__H_

#include <stdint.h>
#include "nrf24l01_plus_registers.h"

// IO pin states
#define LOW     0
#define HIGH    1

// boolean values
#define TRUE    1
#define FALSE   0

// NRF24 config defines
#define DATA_RATE_1MBPS         0
#define DATA_RATE_2MBPS         1
#define DATA_RATE_250KBPS       2

#define RF_PWR_MINUS_18DBM      0
#define RF_PWR_MINUS_12DBM      1
#define RF_PWR_MINUS_6DBM       2
#define RF_PWR_0DBM             3

#define ADDRESS_3_BYTES         1
#define ADDRESS_4_BYTES         2
#define ADDRESS_5_BYTES         3

#define DATA_PIPE_0             0
#define DATA_PIPE_1             1
#define DATA_PIPE_2             2
#define DATA_PIPE_3             3
#define DATA_PIPE_4             4
#define DATA_PIPE_5             5

#define ENCODING_SCHEME_1BYTE   0
#define ENCODING_SCHEME_2BYTES  1

#define MAX_RF_CHANNEL          125

/*
 * Initializes nRF24l01+ module.
 */
void NRF24_init(void);


/*
 * ****************************************************************************
 * ****************************  Network settings  ****************************
 * ****************************************************************************
 */

/*
 * Sets the data rate. The air data rate is the modulated signaling rate the
 * nRF24L01+ uses when transmitting and receiving data. It can be 250kbps,
 * 1Mbps or 2Mbps. Using lower air data rate gives better receiver sensitivity
 * than higher air data rate. But, high air data rate gives lower average
 * current consumption and reduced probability of on-air collisions.
 * @param data_rate:
 *                   DATA_RATE_1MBPS     0
 *                   DATA_RATE_2MBPS     1
 *                   DATA_RATE_250KBPS   2
 */
void NRF24_setDataRate(uint8_t data_rate);

/*
 * Sets the RF channel. The RF channel frequency determines the center of the
 * channel used by the nRF24L01+.
 * @param channel Desired channel number. Range between 0 - 125.
 */
void NRF24_setRFChannel(uint8_t channel);

/*
 * In RX mode a Received Power Detector (RPD) signal is available. The RPD is a
 * signal that is set high when a RF signal higher than -64 dBm is detected
 * inside the receiving frequency channel.
 */
void NRF24_getRPD(void);

/*
 * The PA (Power Amplifier) control is used to set the output power from the
 * nRF24L01+ power amplifier.
 * @param rf_power RF output power:
 *                                  RF_PWR_MINUS_18DBM  0
 *                                  RF_PWR_MINUS_12DBM  1
 *                                  RF_PWR_MINUS_6DBM   2
 *                                  RF_PWR_0DBM         3
 */
void NRF24_setPowerAmplifier(uint8_t rf_power);

/*
 * Disables auto Acknowledgment (ACK) on selected data pipe.
 * @param data_pipe Data pipe on witch would be auto ACK disabled:
 *      DATA_PIPE_0     0
 *      DATA_PIPE_1     1
 *      DATA_PIPE_2     2
 *      DATA_PIPE_3     3
 *      DATA_PIPE_4     4
 *      DATA_PIPE_5     5
 */
void NRF24_disableAutoACK(uint8_t data_pipe);

/*
 * Enables auto Acknowledgment (ACK) on selected data pipe.
 * @param data_pipe Data pipe on witch would be auto ACK enabled:
 *      DATA_PIPE_0     0
 *      DATA_PIPE_1     1
 *      DATA_PIPE_2     2
 *      DATA_PIPE_3     3
 *      DATA_PIPE_4     4
 *      DATA_PIPE_5     5
 */
void NRF24_enableAutoACK(uint8_t data_pipe);

/*
 * Sets the auto retransmission delay in 250uS steps. Range 0 (250uS) - 15
 * (4000uS).
 * @param delay Value for auto retransmit delay.
 */
void NRF24_setAutoRetransmissionDelay(uint8_t delay);

/*
 * Sets the number of retransmission trials. Range 0 - 15.
 * @param retransmit_count Number of retransmission trials.
 */
void NRF24_setAutoRetransmissionTrials(uint8_t retransmit_count);

/*
 * Enables Cyclic Redundancy Check (CRC).
 */
void NRF24_enableCRC(void);

/*
 * Disables Cyclic Redundancy Check (CRC).
 */
void NRF24_disableCRC(void);

/*
 * Sets the CRC encoding scheme.
 * @param encoding_scheme:
 *                         ENCODING_SCHEME_1BYTE   0
 *                         ENCODING_SCHEME_2BYTES  1
 */
void NRF24_setCRCEncodingScheme(uint8_t encoding_scheme);

/*
 *  Enables continuous carrier transmit.
 */
void NRF24_enableContWave(void);

/*
 * Disables continuous carrier transmit.
 */
void NRF24_disableContWave(void);

/*
 * Sets the payload length on selected data pipe.
 * @param data_pipe Data pipe:
 *                             DATA_PIPE_0  0
 *                             DATA_PIPE_1  1
 *                             DATA_PIPE_2  2
 *                             DATA_PIPE_3  3
 *                             DATA_PIPE_4  4
 *                             DATA_PIPE_5  5
 * @param length Length of the payload. Range 0 - 31.
 */
void NRF24_setPayloadLength(uint8_t data_pipe, uint8_t length);

//void NRF24_enableDynamicPayloadLengthRx(data_pipe_e data_pipe);
//void NRF24_disableDynamicPayloadLengthRx(data_pipe_e data_pipe);

/*
 * Sets the address length of TX and RX addresses.
 * @param address_length Address length:
 *                                       ADDRESS_3_BYTES    1
 *                                       ADDRESS_4_BYTES    2
 *                                       ADDRESS_5_BYTES    3
 */
void NRF24_setAddressLength(uint8_t address_length);

/*
 * Sets the receivers address on selected data pipe.
 * @param address Receivers address.
 * @param address_length Length of receivers address:
 *      ADDRESS_3_BYTES    1
 *      ADDRESS_4_BYTES    2
 *      ADDRESS_5_BYTES    3
 * @param data_pipe Data pipe on which would be the address set:
 *      DATA_PIPE_0  0
 *      DATA_PIPE_1  1
 *      DATA_PIPE_2  2
 *      DATA_PIPE_3  3
 *      DATA_PIPE_4  4
 *      DATA_PIPE_5  5
 */
void NRF24_setRxAddress(uint8_t* address, uint8_t address_length,
        uint8_t data_pipe);

/*
 * Sets the transmitters address.
 * @param address Transmitters address.
 * @param address_length Length of transmitters address:
 *      ADDRESS_3_BYTES    1
 *      ADDRESS_4_BYTES    2
 *      ADDRESS_5_BYTES    3
 */
void NRF24_setTxAddress(uint8_t* address, uint8_t address_length);

/*
 * Enables selected data pipe.
 * @param data_pipe Data pipe:
 *                             DATA_PIPE_0  0
 *                             DATA_PIPE_1  1
 *                             DATA_PIPE_2  2
 *                             DATA_PIPE_3  3
 *                             DATA_PIPE_4  4
 *                             DATA_PIPE_5  5
 */
void NRF24_enableDataPipe(uint8_t data_pipe);

/*
 * Disables selected data pipe.
 * @param data_pipe Data pipe:
 *                             DATA_PIPE_0  0
 *                             DATA_PIPE_1  1
 *                             DATA_PIPE_2  2
 *                             DATA_PIPE_3  3
 *                             DATA_PIPE_4  4
 *                             DATA_PIPE_5  5
 */
void NRF24_disableDataPipe(uint8_t data_pipe);

/*
 * ****************************************************************************
 * ****************************  Power management  ****************************
 * ****************************************************************************
 */

/*
 * Goes to RX mode. The RX mode is an active mode where the nRF24L01+ radio is
 * used as a receiver.
 */
void NRF24_RxMode(void);

/*
 * Goes to Standby-I mode. Standby-I mode is used to minimize average current
 * consumption while maintaining short start up times. In this mode only part
 * of the crystal oscillator is active.
 */
void NRF24_standbyI(void);

/*
 * Goes to Standby-II mode. In standby-II mode extra clock buffers are active and
 * more current is used compared to standby-I mode.
 */
void NRF24_standbyII(void);

/*
 * Goes to Power Down mode. In power down mode nRF24L01+ is disabled using
 * minimal current consumption. All register values avail-able are maintained
 * and the SPI is kept active, enabling change of configuration and the
 * uploading/down-loading of data registers.
 */
void NRF24_powerDown(void);

/*
 * ****************************************************************************
 * ******************************  State checkers  *****************************
 * ****************************************************************************
 */

/*
 * Gets STATUS register of the nRF24l01+ module.
 * @return Status register value.
 */
uint8_t NRF24_getStatus(void);

/*
 * ****************************************************************************
 * *****************************  TX/RX functions  ****************************
 * ****************************************************************************
 */

/*
 * Send data to other device defined with TX_ADDRESS.
 * @param data Data to be transmit.
 * @param length Length (Size) of the data.
 */
void NRF24_sendData(uint8_t *data, uint8_t length);

/*
 * Function checks if there is available data to be received and receives it
 * from actual data pipe.
 * @param data_pipe
 * @param data
 */
void NRF24_getData(uint8_t *data_pipe, uint8_t *data);

/*
 * ****************************************************************************
 * ******************  Extern (platform specific) functions  ******************
 * ****************************************************************************
 */

/*
 * In this function the basic NRF24 IO setup should be implemented.
 * Set CE pin as output
 * Set CSN pin as output
 * (Set IRQ pin as input)
 */
extern void NRF24_ioSetup(void);

/*
 * NRF24 CE pin control function
 * @param state State of the pin:
 *                                0 - LOW
 *                                1 - HIGH
 */
extern void NRF24_setCE(uint8_t state);

/*
 * NRF24 CSN pin control function
 * @param state State of the pin:
 *                                0 - LOW
 *                                1 - HIGH
 */
extern void NRF24_setCSN(uint8_t state);

//TODO add functions for IRQ pin

/*
 * In this function the initialization of SPI protocol should be implemented.
 * SPI pin configuration and other SPI related stuff.
 */
extern void SPI_init(void);

/*
 * SPI read/write should be implemented. It can be hardware SPI or software
 * bit banging.
 */
extern uint8_t SPI_readWrite(uint8_t data);

#endif /* NRF24L01_PLUS__H_ */
