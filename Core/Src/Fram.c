/*
 * Fram.c - State machine based FRAM driver
 */

#include "Fram.h"

I2C_HandleTypeDef *fram_i2c;

typedef enum {
    FRAM_IDLE,
    FRAM_WRITING,
    FRAM_READING,
    FRAM_DONE,
    FRAM_ERROR
} FRAM_State_t;

typedef struct {
    FRAM_State_t state;
    uint16_t current_index;
    uint16_t total_words;
    uint16_t *data_ptr;
    bool is_write;
} FRAM_Handle_t;

static FRAM_Handle_t fram_handle = {FRAM_IDLE, 0, 0, NULL, false};

bool FRAM_Init(I2C_HandleTypeDef *hi2c)
{
    fram_i2c = hi2c;
    HAL_Delay(20);
    return (HAL_I2C_IsDeviceReady(fram_i2c, FRAM_I2C_ADDR, 3, 100) == HAL_OK);
}

// Non-blocking write word
static bool FRAM_Write_Word_NB(uint16_t addr, uint16_t data)
{
    uint8_t buffer[2];
    buffer[0] = data & 0xFF;
    buffer[1] = (data >> 8) & 0xFF;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(
        fram_i2c, FRAM_I2C_ADDR, addr,
        I2C_MEMADD_SIZE_16BIT, buffer, 2, 100
    );

    return (status == HAL_OK);
}

// Non-blocking read word
static bool FRAM_Read_Word_NB(uint16_t addr, uint16_t *data)
{
    uint8_t buffer[2];

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
        fram_i2c, FRAM_I2C_ADDR, addr,
        I2C_MEMADD_SIZE_16BIT, buffer, 2, 100
    );

    if (status == HAL_OK) {
        *data = buffer[0] | (buffer[1] << 8);
        return true;
    }
    return false;
}

// Start write operation
bool FRAM_Write_Config_Start(FRAM_Config_t *config)
{
    if (fram_handle.state != FRAM_IDLE) {
        return false;  // Busy
    }

    fram_handle.data_ptr = (uint16_t*)config;
    fram_handle.total_words = sizeof(FRAM_Config_t) / sizeof(uint16_t);
    fram_handle.current_index = 0;
    fram_handle.is_write = true;
    fram_handle.state = FRAM_WRITING;

    return true;
}

// Start read operation
bool FRAM_Read_Config_Start(FRAM_Config_t *config)
{
    if (fram_handle.state != FRAM_IDLE) {
        return false;  // Busy
    }

    fram_handle.data_ptr = (uint16_t*)config;
    fram_handle.total_words = sizeof(FRAM_Config_t) / sizeof(uint16_t);
    fram_handle.current_index = 0;
    fram_handle.is_write = false;
    fram_handle.state = FRAM_READING;

    return true;
}

// Process FRAM operations - call this in main loop
void FRAM_Process(void)
{
    if (fram_handle.state == FRAM_WRITING) {

        if (fram_handle.current_index < fram_handle.total_words) {

            uint16_t addr = fram_handle.current_index * 2;
            uint16_t data = fram_handle.data_ptr[fram_handle.current_index];

            if (FRAM_Write_Word_NB(addr, data)) {
                fram_handle.current_index++;
            } else {
                fram_handle.state = FRAM_ERROR;
            }

        } else {
            fram_handle.state = FRAM_DONE;
        }

    } else if (fram_handle.state == FRAM_READING) {

        if (fram_handle.current_index < fram_handle.total_words) {

            uint16_t addr = fram_handle.current_index * 2;
            uint16_t data;

            if (FRAM_Read_Word_NB(addr, &data)) {
                fram_handle.data_ptr[fram_handle.current_index] = data;
                fram_handle.current_index++;
            } else {
                fram_handle.state = FRAM_ERROR;
            }

        } else {
            fram_handle.state = FRAM_DONE;
        }
    }
}

// Check if operation is complete
bool FRAM_Is_Done(void)
{
    return (fram_handle.state == FRAM_DONE);
}

// Check if operation failed
bool FRAM_Is_Error(void)
{
    return (fram_handle.state == FRAM_ERROR);
}

// Reset state machine
void FRAM_Reset(void)
{
    fram_handle.state = FRAM_IDLE;
    fram_handle.current_index = 0;
}

// Blocking write (for compatibility)
bool FRAM_Write_Config(FRAM_Config_t *config)
{
    FRAM_Write_Config_Start(config);

    while (!FRAM_Is_Done() && !FRAM_Is_Error()) {
        FRAM_Process();
        HAL_Delay(1);
    }

    bool success = FRAM_Is_Done();
    FRAM_Reset();
    return success;
}

// Blocking read (for compatibility)
bool FRAM_Read_Config(FRAM_Config_t *config)
{
    FRAM_Read_Config_Start(config);

    while (!FRAM_Is_Done() && !FRAM_Is_Error()) {
        FRAM_Process();
        HAL_Delay(1);
    }

    bool success = FRAM_Is_Done();
    FRAM_Reset();
    return success;
}

void FRAM_Restore_Defaults(void)
{
    FRAM_Config_t default_config = {
        .input_type = 0,
        .input_low = 0,
        .input_high = 1000,
        .offset_input = 0,
        .output_low = 0,
        .output_high = 2000,
        .output_direction = 0,
        .output_decimal = 1,
        .offset_output = 0,
        .comm_enable = 1,
        .slave_id = 1,
        .cal_input_lo = 0,
        .cal_input_hi = 1000,
        .cal_output_lo = 0,
        .cal_output_hi = 2000,
        .Inp_Cal_code1 = 0,
        .Inp_Cal_code2 = 1000,
        .adc_cal_oe_code = 0,
        .adc_cal_ge_code = 256,
        .adc_cal_flag_code = 0,
//        .reserved = {0}
    };
    FRAM_Write_Config(&default_config);
}
