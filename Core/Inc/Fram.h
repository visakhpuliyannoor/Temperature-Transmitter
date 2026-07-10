/*
 * Fram.h
 *
 *  Created on: 13-Jan-2026
 *      Author: visak
 */

// fram.h
#ifndef FRAM_H
#define FRAM_H

#include "stm32f0xx_hal.h"
#include <stdbool.h>

// FRAM Configuration
#define FRAM_I2C_ADDR       0xA0    // 7-bit address << 1
#define FRAM_SIZE           8192    // 8KB FRAM
#define FRAM_PAGE_SIZE      32      // Bytes per page

// Memory map addresses
#define ADDR_INPUT_TYPE         0x0000
#define ADDR_INPUT_LOW          0x0004
#define ADDR_INPUT_HIGH         0x0008
#define ADDR_INPUT_DECIMAL      0x000C
#define ADDR_OUTPUT_LOW         0x0010
#define ADDR_OUTPUT_HIGH        0x0014
#define ADDR_OUTPUT_DECIMAL     0x0018
#define ADDR_OUTPUT_DIRECTION   0x001C
#define ADDR_OFFSET_INPUT       0x0020
#define ADDR_OFFSET_OUTPUT      0x0024
#define ADDR_COMM_ENABLE        0x0028
#define ADDR_SLAVE_ID           0x002C
#define ADDR_PARITY             0x0030
#define ADDR_STOP_BIT           0x0034
#define ADDR_BAUD_RATE          0x0038
#define ADDR_DATA_BIT           0x003C
#define ADDR_SENSOR_FAULT_TIME  0x0040
#define ADDR_CAL_INPUT_TYPE     0x0044
#define ADDR_CAL_INPUT_DECIMAL  0x0048
#define ADDR_CAL_INPUT_LO       0x004C
#define ADDR_CAL_INPUT_HI       0x0050
#define ADDR_CAL_OUTPUT_LO      0x0054
#define ADDR_CAL_OUTPUT_HI      0x0058

// Backup addresses for triple redundancy (offset by 0x100 and 0x200)
#define FRAM_BACKUP1_OFFSET     0x0100
#define FRAM_BACKUP2_OFFSET     0x0200

// CRC address (after all data)
#define ADDR_CRC                0x0300

// Configuration structure
typedef struct {
	uint16_t process_value;//PV
	uint16_t input_type;
	uint16_t input_low;
	uint16_t input_high;
	uint16_t input_decimal;
	uint16_t output_low;
	uint16_t output_high;
	uint16_t output_decimal;
	uint16_t output_direction;
	uint16_t offset_input;
	uint16_t offset_output;
	uint16_t comm_enable;
	uint16_t slave_id;
	uint16_t parity;
	uint16_t stop_bit;
	uint16_t baud_rate;
	uint16_t data_bit;
	uint16_t sensor_fault_time;
	uint16_t cal_input_type;
	uint16_t cal_input_decimal;
	uint16_t cal_input_lo;
	uint16_t cal_input_hi;
	uint16_t cal_output_lo;
	uint16_t cal_output_hi;
	uint16_t Inp_Cal_code1;        // ADC code at low point
	uint16_t Inp_Cal_code2;        // ADC code at high point

	uint16_t adc_cal_oe_code;      // ADC Offset Error code
	uint16_t adc_cal_ge_code;      // ADC Gain Error code
	uint16_t adc_cal_flag_code;    // ADC Flag code

} FRAM_Config_t;




// Public functions
bool FRAM_Init(I2C_HandleTypeDef *hi2c);
bool FRAM_Write_Config(FRAM_Config_t *config);
bool FRAM_Read_Config(FRAM_Config_t *config);
bool FRAM_Write_Word(uint16_t addr, uint16_t data);
bool FRAM_Read_Word(uint16_t addr, uint16_t *data);
//bool FRAM_Restore_Defaults(void);
bool FRAM_Verify_Integrity(void);
void FRAM_Process(void);
bool FRAM_Write_Config_Start(FRAM_Config_t *config);
bool FRAM_Read_Config_Start(FRAM_Config_t *config);
bool FRAM_Is_Done(void);
bool FRAM_Is_Error(void);
void FRAM_Reset(void);
void FRAM_Restore_Defaults(void);

#endif // FRAM_H
