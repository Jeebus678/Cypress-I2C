/*
 * CY8CMBR3xxx_CRC.h
 *
 *  Created on: 5 αιεμι 2020
 *      Author: Michael
 */

#ifndef SRC_CY8CMBR3XXX_CRC_H_
#define SRC_CY8CMBR3XXX_CRC_H_

uint16_t CY8CMBR3xxx_Calc4BitsCRC(uint8_t value, uint16_t remainder);
uint16_t CY8CMBR3xxx_CalculateCrc(uint8_t *configuration);

#endif /* SRC_CY8CMBR3XXX_CRC_H_ */
