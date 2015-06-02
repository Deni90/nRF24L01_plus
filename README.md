#nRF24L01+ Single Chip 2.4GHz Transceiver Library

This library is portable and gives API for easy configuration of the RF modules.

The API provides:
- Network settings (data rate, channel, power amplifier, auto ACK controll, auto retransmission delay, auto retransmission trials, CRC controll, CRC encoding scheme, cont wave controll, payload length, address lenght, Rx/Tx address, data pipe controll)
- Power management (Rx, Standby-I, Standby-II, Power down modes)
- Status getter
- Tx/Rx functions

## Porting the library

To port the library on the target hardware the extern functions must be implemented. These functions are used for controlling NRFs IO ports and SPI. SPI can be implemented as hardware SPI or software bit banging.

##TODO list

- Implementation of dynamic payload length mode
- Usage of IRQ pin for receiving data

## References

This library is based on kehribar's nrf24L01_plus library:
	https://github.com/kehribar/nrf24L01_plus

See the transceivers datasheet: http://www.nordicsemi.com/eng/content/download/2726/34069/file/nRF24L01P_Product_Specification_1_0.pdf
