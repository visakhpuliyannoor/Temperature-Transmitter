/*
 * Modbus.c
 *
 *  Created on: 16-Jan-2026
 *      Author: visakh.p
 */

#include "Modbus.h"
#include "keyboard.h"
#include "Fram.h"
#include "ADC.h"
#include "Output.h"

// External references
extern FRAM_Config_t device_config;
extern ADC_Handle_t ADCHandler;
extern OperationMode_e current_mode;
// Private variables
static UART_HandleTypeDef *modbus_uart;
Modbus_Handle_t ModbusHandler;
static uint32_t last_rx_time = 0;

// Private function prototypes
static uint16_t CRC16(uint8_t *buffer, uint16_t length);
static bool Check_CRC(uint8_t *buffer, uint16_t length);
static void Append_CRC(uint8_t *buffer, uint16_t length);
static void Process_Request(void);
static void Handle_Read_Holding_Registers(void);
static void Handle_Write_Single_Register(void);
static void Handle_Write_Multiple_Registers(void);
static void Send_Exception(uint8_t function_code, uint8_t exception_code);
static void Send_Response(void);
static uint16_t Read_Register(uint16_t address);
static bool Write_Register(uint16_t address, uint16_t value);

void Modbus_Init(UART_HandleTypeDef *huart)
{
    modbus_uart = huart;

    // Initialize Modbus handler
    ModbusHandler.slave_id = 1;
    ModbusHandler.rx_length = 0;
    ModbusHandler.tx_length = 0;
    ModbusHandler.frame_received = false;
    ModbusHandler.last_byte_time = 0;
    ModbusHandler.comm_enabled = true;

    // Start UART receive with DMA (use idle line detection)
    __HAL_UART_ENABLE_IT(modbus_uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(modbus_uart, ModbusHandler.rx_buffer, MODBUS_BUFFER_SIZE);
}

void Modbus_Process(void)
{
    if (!ModbusHandler.comm_enabled) {
        return;
    }

    // Check if we need to write to FRAM (after Modbus response sent)
    // Handle pending FRAM write - Solution 1
//    if (ModbusHandler.pending_fram_write) {
//        ModbusHandler.pending_fram_write = false;
//
//        // Disable interrupts and write to FRAM
//        __disable_irq();
//        FRAM_Write_Config(&device_config);
//        __enable_irq();
//
//        // Debug: Toggle LED to confirm write happened
//        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
//    }

//    if (ModbusHandler.pending_fram_write) {
//           if (FRAM_Write_Config_Start(&device_config)) {
//               ModbusHandler.pending_fram_write = false;
//               // FRAM_Process() in main loop will handle the actual writing
//           }
//       }

    // Check for timeout-based frame completion (3.5 character times)
    if (ModbusHandler.rx_length > 0 && !ModbusHandler.frame_received) {
        uint32_t current_time = HAL_GetTick();
        if ((current_time - last_rx_time) > 10) {  // 10ms timeout for 9600 baud
            ModbusHandler.frame_received = true;
        }
    }

    // Check for received frame
    if (ModbusHandler.frame_received) {
        ModbusHandler.frame_received = false;

        // Check minimum frame length (Slave ID + Function + CRC = 4 bytes)
        if (ModbusHandler.rx_length < 4) {
            ModbusHandler.rx_length = 0;
            return;
        }

        // Check if message is for this slave
        if (ModbusHandler.rx_buffer[0] != ModbusHandler.slave_id) {
            ModbusHandler.rx_length = 0;
            return;
        }

        // Verify CRC
        if (!Check_CRC(ModbusHandler.rx_buffer, ModbusHandler.rx_length)) {
            ModbusHandler.rx_length = 0;
            return;  // Invalid CRC, ignore frame
        }

        // Process valid request
        Process_Request();

        // Reset for next frame
        ModbusHandler.rx_length = 0;
    }
}

static void Process_Request(void)
{
    uint8_t function_code = ModbusHandler.rx_buffer[1];

    switch (function_code) {
//        case MODBUS_FC_READ_HOLDING_REG:
        case MODBUS_FC_READ_INPUT_REG:
        case MODBUS_FC_READ_HOLDING_REG:
            Handle_Read_Holding_Registers();
            break;

        case MODBUS_FC_WRITE_SINGLE_REG:
            Handle_Write_Single_Register();
            break;

        case MODBUS_FC_WRITE_MULTI_REG:
            Handle_Write_Multiple_Registers();
            break;

        default:
            Send_Exception(function_code, MODBUS_EXCEPTION_ILLEGAL_FUNCTION);
            break;
    }
}

static void Handle_Read_Holding_Registers(void)
{
    uint16_t start_address = (ModbusHandler.rx_buffer[2] << 8) | ModbusHandler.rx_buffer[3];
    uint16_t num_registers = (ModbusHandler.rx_buffer[4] << 8) | ModbusHandler.rx_buffer[5];

    // Check register count
    if (num_registers == 0 || num_registers > 125) {
        Send_Exception(ModbusHandler.rx_buffer[1], MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Build response
    ModbusHandler.tx_buffer[0] = ModbusHandler.slave_id;
    ModbusHandler.tx_buffer[1] = ModbusHandler.rx_buffer[1];  // Function code
    ModbusHandler.tx_buffer[2] = num_registers * 2;           // Byte count

    uint16_t byte_index = 3;

    for (uint16_t i = 0; i < num_registers; i++) {
        uint16_t reg_value = Read_Register(start_address + i);
        ModbusHandler.tx_buffer[byte_index++] = (reg_value >> 8) & 0xFF;  // High byte
        ModbusHandler.tx_buffer[byte_index++] = reg_value & 0xFF;          // Low byte
    }

    ModbusHandler.tx_length = byte_index;
    Send_Response();
}

static void Handle_Write_Single_Register(void)
{
	uint16_t reg_address = (ModbusHandler.rx_buffer[2] << 8) | ModbusHandler.rx_buffer[3];
		    uint16_t reg_value = (ModbusHandler.rx_buffer[4] << 8) | ModbusHandler.rx_buffer[5];

		    // Try to write the register
		    bool write_success = Write_Register(reg_address, reg_value);

		    if (write_success) {
		        // Save to FRAM (comment out for testing if FRAM has issues)
//		        FRAM_Write_Config(&device_config);
		    	ModbusHandler.pending_fram_write = true;

		        // Echo request as response
		        ModbusHandler.tx_buffer[0] = ModbusHandler.rx_buffer[0];
		        ModbusHandler.tx_buffer[1] = ModbusHandler.rx_buffer[1];
		        ModbusHandler.tx_buffer[2] = ModbusHandler.rx_buffer[2];
		        ModbusHandler.tx_buffer[3] = ModbusHandler.rx_buffer[3];
		        ModbusHandler.tx_buffer[4] = ModbusHandler.rx_buffer[4];
		        ModbusHandler.tx_buffer[5] = ModbusHandler.rx_buffer[5];
		        ModbusHandler.tx_length = 6;
		        Send_Response();
		    } else {
		        Send_Exception(ModbusHandler.rx_buffer[1], MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
		    }
}

static void Handle_Write_Multiple_Registers(void)
{
	 uint16_t start_address = (ModbusHandler.rx_buffer[2] << 8) | ModbusHandler.rx_buffer[3];
	    uint16_t num_registers = (ModbusHandler.rx_buffer[4] << 8) | ModbusHandler.rx_buffer[5];
	    uint8_t byte_count = ModbusHandler.rx_buffer[6];

	    // Validate
	    if (byte_count != (num_registers * 2)) {
	        Send_Exception(ModbusHandler.rx_buffer[1], MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
	        return;
	    }

	    if (num_registers == 0 || num_registers > 100) {
	        Send_Exception(ModbusHandler.rx_buffer[1], MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
	        return;
	    }

	    // Write registers
	    uint16_t data_index = 7;
	    bool all_writes_success = true;

	    for (uint16_t i = 0; i < num_registers; i++) {
	        uint16_t reg_value = (ModbusHandler.rx_buffer[data_index] << 8) |
	                             ModbusHandler.rx_buffer[data_index + 1];
	        data_index += 2;

	        if (!Write_Register(start_address + i, reg_value)) {
	            all_writes_success = false;
	            Send_Exception(ModbusHandler.rx_buffer[1], MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
	            return;
	        }
	    }

	    // Save to FRAM after all successful writes
	    if (all_writes_success) {
	        FRAM_Write_Config(&device_config);

	        // Build response
	        ModbusHandler.tx_buffer[0] = ModbusHandler.slave_id;
	        ModbusHandler.tx_buffer[1] = ModbusHandler.rx_buffer[1];
	        ModbusHandler.tx_buffer[2] = ModbusHandler.rx_buffer[2];
	        ModbusHandler.tx_buffer[3] = ModbusHandler.rx_buffer[3];
	        ModbusHandler.tx_buffer[4] = ModbusHandler.rx_buffer[4];
	        ModbusHandler.tx_buffer[5] = ModbusHandler.rx_buffer[5];
	        ModbusHandler.tx_length = 6;
	        Send_Response();
	    }
}

static uint16_t Read_Register(uint16_t address)
{
	switch (address) {
	        case REG_INPUT_TYPE:
	            return device_config.input_type;
	        case REG_LOW_LIMIT_INPUT:
	            return device_config.input_low;
	        case REG_HIGH_LIMIT_INPUT:
	            return device_config.input_high;
	        case REG_DECIMAL_POINT_INPUT:
	            return device_config.input_decimal;
	        case REG_LOW_LIMIT_OUTPUT:
	            return device_config.output_low;
	        case REG_HIGH_LIMIT_OUTPUT:
	            return device_config.output_high;
	        case REG_DECIMAL_POINT_OUTPUT:
	            return device_config.output_decimal;
	        case REG_OUTPUT_DIRECTION:
	            return device_config.output_direction;
	        case REG_SENSOR_BREAK:
	            return ADC_Is_Sensor_Fault() ? 0 : 1;
	        case REG_OFFSET_INPUT:
	            return device_config.offset_input;
	        case REG_OFFSET_OUTPUT:
	            return device_config.offset_output;
	        case REG_NEGATIVE_SIGN_BIT:
	            return (ADCHandler.modbus_value < 0) ? 1 : 0;
	        case REG_COMM_ENABLE:
	            return device_config.comm_enable;
	        case REG_SLAVE_ID:
	            return device_config.slave_id;
	        case REG_PARITY:
	            return device_config.parity;
	        case REG_STOP_BIT:
	            return device_config.stop_bit;
	        case REG_BAUD_RATE:
	            return device_config.baud_rate;
	        case REG_DATA_BIT:
	            return device_config.data_bit;
	        case REG_SENSOR_FAULT_TIME:
	            return device_config.sensor_fault_time;
	        case REG_CAL_INPUT_TYPE:
	            return device_config.cal_input_type;
	        case REG_CAL_INPUT_DECIMAL:
	            return device_config.cal_input_decimal;
	        case REG_CAL_INPUT_LO:
	            return device_config.cal_input_lo;
	        case REG_CAL_INPUT_HI:
	            return device_config.cal_input_hi;
	        case REG_CAL_OUTPUT_LO:
	            return device_config.cal_output_lo;
	        case REG_CAL_OUTPUT_HI:
	            return device_config.cal_output_hi;
	        case REG_MEMORY_CORRUPTED:
	            return (Keyboard_GetMode() == MODE_MEMORY_CORRUPT) ? 0 : 1;
	        case PROCESS_VALUE:
	        	return device_config.process_value;

	      //*******For Standby Mode**********
	        case REG_PWM_TEST_VALUE:
	              // Return current PWM pulse value
	              return AOPHandler.pulse_value;

	          case REG_PWM_TEST_ENABLE:
	              // Return 1 if in PWM test mode, 0 otherwise
	              return (current_mode == MODE_PWM_TEST) ? 1 : 0;
	        default:
	            return 0;
	    }
}

static bool Write_Register(uint16_t address, uint16_t value)
{
	 bool valid = true;

	    switch (address) {
	        case REG_INPUT_TYPE:
	            if (value <= 12) {
	                device_config.input_type = value;
	                ADC_Set_Sensor_Type((SensorType_t)value);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_LOW_LIMIT_INPUT:
	            device_config.input_low = value;
	            ADC_Set_Range(device_config.input_low, device_config.input_high);
	            break;

	        case REG_HIGH_LIMIT_INPUT:
	            device_config.input_high = value;
	            ADC_Set_Range(device_config.input_low, device_config.input_high);
	            break;

	        case REG_DECIMAL_POINT_INPUT:
	            if (value <= 3) {
	                device_config.input_decimal = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_LOW_LIMIT_OUTPUT:
	            device_config.output_low = value;
	            AnalogOutput_Set_Range(device_config.output_low, device_config.output_high);
	            break;

	        case REG_HIGH_LIMIT_OUTPUT:
	            device_config.output_high = value;
	            AnalogOutput_Set_Range(device_config.output_low, device_config.output_high);
	            break;

	        case REG_DECIMAL_POINT_OUTPUT:
	            if (value <= 3) {
	                device_config.output_decimal = value;
	                AnalogOutput_Set_Decimal(value);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_OUTPUT_DIRECTION:
	            if (value <= 1) {
	                device_config.output_direction = value;
	                AnalogOutput_Set_Direction((OutputDirection_t)value);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_SENSOR_BREAK:
	            valid = false;  // Read only
	            break;

	        case REG_OFFSET_INPUT:
	            device_config.offset_input = (int16_t)value;
	            ADC_Set_Offset(device_config.offset_input);
	            break;

	        case REG_OFFSET_OUTPUT:
	            device_config.offset_output = (int16_t)value;
	            AnalogOutput_Set_Offset(device_config.offset_output);
	            break;

	        case REG_NEGATIVE_SIGN_BIT:
	            valid = false;  // Read only
	            break;

	        case 0:
	            valid = false;  // Reserved //Read only
	            break;

	        case REG_COMM_ENABLE:
	            if (value <= 1) {
	                device_config.comm_enable = value;
	                Modbus_Enable(value);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_SLAVE_ID:
	            if (value >= 1 && value <= 255) {
	                device_config.slave_id = value;
	                Modbus_Set_Slave_ID(value);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_PARITY:
	            if (value <= 2) {
	                device_config.parity = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_STOP_BIT:
	            if (value <= 1) {
	                device_config.stop_bit = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_BAUD_RATE:
	            if (value <= 3) {
	                device_config.baud_rate = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_DATA_BIT:
	            if (value <= 1) {
	                device_config.data_bit = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_SENSOR_FAULT_TIME:
	            if (value <= 4) {
	                device_config.sensor_fault_time = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case 20: case 21: case 22: case 23: case 24:
	        case 25: case 26: case 27: case 28: case 29:
	            valid = false;  // Reserved
	            break;

	        case REG_CAL_INPUT_TYPE:
	            if (value <= 12) {
	                device_config.cal_input_type = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_CAL_INPUT_DECIMAL:
	            if (value <= 3) {
	                device_config.cal_input_decimal = value;
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_CAL_INPUT_LO:
	            device_config.cal_input_lo = value;
	            break;

	        case REG_CAL_INPUT_HI:
	            device_config.cal_input_hi = value;
	            break;

	        case REG_CAL_OUTPUT_LO:
	            device_config.cal_output_lo = value;
	            break;

	        case REG_CAL_OUTPUT_HI:
		device_config.cal_output_hi = value;
	            break;

	        case REG_RESTORE_CAL:
	            if (value == 1) {
//	                FRAM_Restore_Defaults();
	                FRAM_Read_Config(&device_config);
	                ADC_Set_Sensor_Type((SensorType_t)device_config.input_type);
	                ADC_Set_Range(device_config.input_low, device_config.input_high);
	                ADC_Set_Offset(device_config.offset_input);
	                Modbus_Set_Slave_ID(device_config.slave_id);
	                AnalogOutput_Set_Range(device_config.output_low, device_config.output_high);
	                AnalogOutput_Set_Direction((OutputDirection_t)device_config.output_direction);
	                AnalogOutput_Set_Offset(device_config.offset_output);
	                AnalogOutput_Set_Decimal(device_config.output_decimal);
	                // Clear memory corrupt mode
	                       Keyboard_SetMode(MODE_NORMAL);
	            } else {
	                valid = false;
	            }
	            break;

	        case REG_MEMORY_CORRUPTED:
	            valid = false;  // Read only
	            break;

	        case 38: case 39:
	            valid = false;  // Reserved
	            break;

	        case REG_PWM_TEST_VALUE:
	                // Write PWM test value (only valid in test mode)
	                if (value > 2000/*1000*/) {
	                    return false;  // Out of range
	                }
	                if (current_mode == MODE_PWM_TEST) {
	                    AnalogOutput_Set_Raw_PWM(value);
	                }
	                return true;

	        case REG_PWM_TEST_ENABLE:
	                // Enable/disable PWM test mode
	                if (value > 1) {
	                    return false;  // Invalid value
	                }
	                if (value == 1 && current_mode != MODE_PWM_TEST) {
	                    // Enter PWM test mode
	                    Keyboard_SetMode(MODE_PWM_TEST);
	                } else if (value == 0 && current_mode == MODE_PWM_TEST) {
	                    // Exit PWM test mode
	                    Keyboard_SetMode(MODE_NORMAL);
	                }
	                return true;


	        default:
	            valid = false;
	            break;
	    }

	    return valid;
}

static void Send_Exception(uint8_t function_code, uint8_t exception_code)
{
    ModbusHandler.tx_buffer[0] = ModbusHandler.slave_id;
    ModbusHandler.tx_buffer[1] = function_code | 0x80;  // Set MSB for exception
    ModbusHandler.tx_buffer[2] = exception_code;
    ModbusHandler.tx_length = 3;
    Send_Response();
}

static void Send_Response(void)
{
    // Append CRC
    Append_CRC(ModbusHandler.tx_buffer, ModbusHandler.tx_length);
    ModbusHandler.tx_length += 2;

    // Small delay before response
    HAL_Delay(MODBUS_RESPONSE_DELAY);

    // Send via UART with DMA (DE pin handled by hardware)
    HAL_UART_Transmit_DMA(modbus_uart, ModbusHandler.tx_buffer, ModbusHandler.tx_length);
}

static uint16_t CRC16(uint8_t *buffer, uint16_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }

    return crc;
}

static bool Check_CRC(uint8_t *buffer, uint16_t length)
{
    uint16_t received_crc = buffer[length - 2] | (buffer[length - 1] << 8);
    uint16_t calculated_crc = CRC16(buffer, length - 2);

    return (received_crc == calculated_crc);
}

static void Append_CRC(uint8_t *buffer, uint16_t length)
{
    uint16_t crc = CRC16(buffer, length);
    buffer[length] = crc & 0xFF;             // CRC Low byte first
    buffer[length + 1] = (crc >> 8) & 0xFF;  // CRC High byte
}

void Modbus_Set_Slave_ID(uint8_t id)
{
    if (id >= 1 && id <= 255) {
        ModbusHandler.slave_id = id;
    }
}

void Modbus_Enable(bool enable)
{
    ModbusHandler.comm_enabled = enable;
}

// UART Idle Line Detection Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == modbus_uart) {
        last_rx_time = HAL_GetTick();
    }
}
