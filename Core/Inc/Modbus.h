/*
 * Modbus.h
 *
 *  Created on: 16-Jan-2026
 *      Author: visakh.p
 */

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include "stm32f0xx_hal.h"
#include <stdbool.h>

// Modbus Configuration
#define MODBUS_SLAVE_ID_DEFAULT     1
#define MODBUS_BUFFER_SIZE          256
#define MODBUS_TIMEOUT              100     // ms
#define MODBUS_RESPONSE_DELAY       5       // ms

// Modbus Function Codes
#define MODBUS_FC_READ_HOLDING_REG  0x03
#define MODBUS_FC_READ_INPUT_REG    0x04
#define MODBUS_FC_WRITE_SINGLE_REG  0x06
#define MODBUS_FC_WRITE_MULTI_REG   0x10

// Modbus Exception Codes
#define MODBUS_EXCEPTION_ILLEGAL_FUNCTION       0x01
#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS   0x02
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE     0x03

// Modbus Register Map (from your PDF)
//#define REG_INPUT_TYPE              0   // 40001
//#define REG_LOW_LIMIT_INPUT         1   // 40002
//#define REG_HIGH_LIMIT_INPUT        2   // 40003
//#define REG_DECIMAL_POINT_INPUT     3   // 40004
//#define REG_LOW_LIMIT_OUTPUT        4   // 40005
//#define REG_HIGH_LIMIT_OUTPUT       5   // 40006
//#define REG_DECIMAL_POINT_OUTPUT    6   // 40007
//#define REG_OUTPUT_DIRECTION        7   // 40008
//#define REG_SENSOR_BREAK            8   // 40009
//#define REG_OFFSET_INPUT            9   // 40010
//#define REG_OFFSET_OUTPUT           10  // 40011
//#define REG_NEGATIVE_SIGN_BIT       11  // 40012
//// Address 12 (40013) - Reserved


// Modbus Register Map (from your PDF)
#define PROCESS_VALUE				0
#define REG_INPUT_TYPE              1   // 40001
#define REG_LOW_LIMIT_INPUT         2   // 40002
#define REG_HIGH_LIMIT_INPUT        3   // 40003
#define REG_DECIMAL_POINT_INPUT     4   // 40004
#define REG_LOW_LIMIT_OUTPUT        5   // 40005
#define REG_HIGH_LIMIT_OUTPUT       6   // 40006
#define REG_DECIMAL_POINT_OUTPUT    7   // 40007
#define REG_OUTPUT_DIRECTION        8   // 40008
#define REG_SENSOR_BREAK            9   // 40009
#define REG_OFFSET_INPUT            10   // 40010
#define REG_OFFSET_OUTPUT           11  // 40011
#define REG_NEGATIVE_SIGN_BIT       12  // 40012
// Address 12 (40013) - Reserved
#define REG_COMM_ENABLE             13  // 40014
#define REG_SLAVE_ID                14  // 40015
#define REG_PARITY                  15  // 40016
#define REG_STOP_BIT                16  // 40017
#define REG_BAUD_RATE               17  // 40018
#define REG_DATA_BIT                18  // 40019
#define REG_SENSOR_FAULT_TIME       19  // 40020
// Addresses 20-29 (40021-40030) - Reserved
#define REG_CAL_INPUT_TYPE          30  // 40031
#define REG_CAL_INPUT_DECIMAL       31  // 40032
#define REG_CAL_INPUT_LO            32  // 40033
#define REG_CAL_INPUT_HI            33  // 40034
#define REG_CAL_OUTPUT_LO           34  // 40035
#define REG_CAL_OUTPUT_HI           35  // 40036
#define REG_RESTORE_CAL             36  // 40037
#define REG_MEMORY_CORRUPTED        37  // 40038
// Addresses 38-39 (40039-40040) - Reserved
#define REG_PWM_TEST_VALUE   		40 //new modbus address
#define REG_PWM_TEST_ENABLE  		41 //new modbus address

// Baud rate options
typedef enum {
    BAUD_9600 = 0,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600
} ModbusBaudRate_t;

// Parity options
typedef enum {
    PARITY_NONE = 0,
    PARITY_ODD,
    PARITY_EVEN
} ModbusParity_t;

// Modbus Handler
typedef struct {
    uint8_t slave_id;
    uint8_t rx_buffer[MODBUS_BUFFER_SIZE];
    uint8_t tx_buffer[MODBUS_BUFFER_SIZE];
    uint16_t rx_length;
    uint16_t tx_length;
    bool frame_received;
    uint32_t last_byte_time;
    bool comm_enabled;
    bool pending_fram_write;//flag for FRAM
} Modbus_Handle_t;

extern Modbus_Handle_t ModbusHandler;

// Public functions
void Modbus_Init(UART_HandleTypeDef *huart);
void Modbus_Process(void);
void Modbus_Set_Slave_ID(uint8_t id);
void Modbus_Enable(bool enable);
void Modbus_Update_Config(uint8_t baud, uint8_t parity, uint8_t stop_bit);


#endif /* INC_MODBUS_H_ */
