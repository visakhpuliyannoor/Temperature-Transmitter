/*
 * ADC.h
 *
 *  Created on: 15-Jan-2026
 *      Author: visak
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32f0xx_hal.h"
#include <stdbool.h>
#include <math.h>
#include "keyboard.h"
//#include "lookup_tables.h"  // We'll create this separately


// Decimal point options
typedef enum {
	DP_NONE = 0,    // 0
	DP_1,           // 0.X
	DP_2,           // 0.0X
	DP_3            // 0.00X
} DecimalPoint_t;

// ADC Configuration
#define ADC_BUFFER_SIZE     2       // Input channel + CJC channel
#define VREF                3300    // mV
#define ADC_MAX_VALUE       4095.0f
#define CJC_SLOPE           0.9869f
#define AMB_KELVIN          273.0f

// Calibration structure
typedef struct {
	uint16_t oe_code;       // Offset error code
	uint16_t ge_code;       // Gain error code
	uint8_t flag_code;      // 0=negative offset, 1=positive offset
} ADC_Cal_t;


//typedef struct FRAM_Config_t FRAM_Config_t;//Visakh on 248/01/2026


// ADC Handler - Single Input Channel
typedef struct {
	// Raw ADC values
	uint16_t adc_buffer[ADC_BUFFER_SIZE];   // [0]=Input, [1]=CJC
	uint16_t adc_raw;                        // Input channel raw value
	uint16_t cjc_raw;                        // CJC channel raw value

	// Calibration (for input channel only)
	ADC_Cal_t cal;

	// Processing variables
	float mv_result;
	float mv_temp;
	float mv_cjc;
	float temp;
	float sensitivity;
	float table_mv_input;

	// Final output
	float lin_value;
	int16_t modbus_value;

	// Configuration (for input channel)
	SensorType_t sensor_type;
	DecimalPoint_t decimal_point;
	int16_t input_low;
	int16_t input_high;
	int16_t offset;

} ADC_Handle_t;

// Public functions
void ADC_Init(ADC_HandleTypeDef *hadc);
void ADC_Process(void);
bool ADC_Is_Sensor_Fault(void);
void ADC_Set_Sensor_Type(SensorType_t type);
void ADC_Set_Range(int16_t low, int16_t high);
void ADC_Set_Offset(int16_t offset);
void ADC_Calibrate(float cal_low_mv, float cal_high_mv,
		float actual_low, float actual_high);
int16_t ADC_Get_Value(void);
float ADC_Get_Float_Value(void);


//New

//extern void ADC_Save_Calibration_To_Config(FRAM_Config_t *config);
//extern void ADC_Restore_Calibration_From_Config(FRAM_Config_t *config);
extern void ADC_Get_Calibration(uint16_t *oe_code, uint16_t *ge_code, uint16_t *flag_code);
extern void ADC_Set_Calibration(uint16_t oe_code, uint16_t ge_code, uint16_t flag_code);


#endif /* INC_ADC_H_ */
