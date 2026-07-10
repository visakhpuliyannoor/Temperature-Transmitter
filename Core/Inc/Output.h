///*
// * Output.h
// *
// *  Created on: 16-Jan-2026
// *      Author: visakh.p
// */
//
//#ifndef INC_OUTPUT_H_
//#define INC_OUTPUT_H_
//
////#include "stm32f0xx_hal.h"
////#include <stdbool.h>
////
////// PWM Configuration
////#define PWM_PERIOD          999     // 0-999 = 1000 steps
////#define PWM_FREQUENCY       2000    // 2kHz
////#define PWM_MIN_PULSE       0       // 0mA
////#define PWM_MAX_PULSE       999     // 20mA
////
////// Output range in pulse counts
////#define OUTPUT_0MA          0       // 0mA = 0 pulse
////#define OUTPUT_3_5MA        175     // 3.5mA = 175 pulse (17.5% of 1000)
////#define OUTPUT_4MA          200     // 4mA = 200 pulse (20% of 1000)
////#define OUTPUT_20MA         999     // 20mA = 999 pulse
////#define OUTPUT_20_5MA       1024    // 20.5mA = 1024 pulse (clamped to 999)
////
////// Output direction
////typedef enum {
////    OUTPUT_DOWN_SCALE = 0,  // Fault = 3.5mA
////    OUTPUT_UP_SCALE = 1     // Fault = 20.5mA
////} OutputDirection_t;
////
////// Analog Output Handler
////typedef struct {
////    uint16_t pulse_value;           // Current PWM pulse value (0-999)
////    int16_t output_value;           // Scaled output value for Modbus
////    uint16_t output_low;            // Low range limit
////    uint16_t output_high;           // High range limit
////    uint8_t output_decimal;         // Decimal point (0-3)
////    OutputDirection_t direction;    // Scale direction
////    int16_t offset;                 // Output offset
////    bool sensor_fault;              // Sensor fault flag
////} AnalogOutput_Handle_t;
////
////// Public functions
////void AnalogOutput_Init(TIM_HandleTypeDef *htim);
////void AnalogOutput_Update(float input_value, float input_low, float input_high);
////void AnalogOutput_Set_Range(uint16_t low, uint16_t high);
////void AnalogOutput_Set_Direction(OutputDirection_t dir);
////void AnalogOutput_Set_Offset(int16_t offset);
////void AnalogOutput_Set_Decimal(uint8_t decimal);
////void AnalogOutput_Set_Fault(bool fault);
////uint16_t AnalogOutput_Get_Pulse(void);
////int16_t AnalogOutput_Get_Modbus_Value(void);
//
////#include "stm32f0xx_hal.h"
////#include <stdbool.h>
////
/////* ---------- PWM pulse limits (TIM3 Period = 999) ---------- */
////#define OUTPUT_4MA      200     /* 4mA   - minimum */
////#define OUTPUT_20MA     999     /* 20mA  - maximum */
////#define OUTPUT_3_5MA    175     /* 3.5mA - downscale fault */
////
/////* ---------- Output direction ----------------------------------------- */
////typedef enum {
////    OUTPUT_UP_SCALE   = 0,
////    OUTPUT_DOWN_SCALE = 1
////} OutputDirection_t;
////
/////* ---------- Main output handler --------------------------------------- */
////typedef struct {
////    uint16_t          pulse_value;    /* current PWM (200-999) */
////    uint16_t          virtual_value;  /* high-res value (0-10000) ← NEW */
////    uint16_t          output_low;     /* Modbus range low */
////    uint16_t          output_high;    /* Modbus range high */
////    uint8_t           output_decimal; /* decimal point */
////    OutputDirection_t direction;      /* up/down */
////    int16_t           offset;         /* offset */
////    int16_t           output_value;   /* Modbus value */
////    bool              sensor_fault;   /* fault flag */
////
////    /* Calibration values - loaded from device_config */
////    uint16_t          cal_output_lo;  /* PWM at 0°C (from device_config) */
////    uint16_t          cal_output_hi;  /* PWM at full scale (from device_config) */
////} AnalogOutput_Handle_t;
////
////extern AnalogOutput_Handle_t AOPHandler;
////
/////* ---------- Core API ------------------------------------------------- */
////void     AnalogOutput_Init(TIM_HandleTypeDef *htim);
////void     AnalogOutput_Update(float input_value, float input_low, float input_high);
////void     AnalogOutput_Set_Range(uint16_t low, uint16_t high);
////void     AnalogOutput_Set_Direction(OutputDirection_t dir);
////void     AnalogOutput_Set_Offset(int16_t offset);
////void     AnalogOutput_Set_Decimal(uint8_t decimal);
////void     AnalogOutput_Set_Fault(bool fault);
////uint16_t AnalogOutput_Get_Pulse(void);
////uint16_t AnalogOutput_Get_Virtual_Value(void);  /* ← NEW: get high-res value */
////int16_t  AnalogOutput_Get_Modbus_Value(void);
////
/////* ---------- Calibration API ------------------------------------------ */
////void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm);
////
/////* ---------- Calibration Bypass (prevents fluctuation) --------------- */
////void AnalogOutput_Calibration_Bypass(bool enable);
////void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value);
//
//
//
//#include "stm32f0xx_hal.h"
//#include <stdbool.h>
//
///* ---------- PWM pulse limits (TIM3 Period = 999) ---------- */
//#define OUTPUT_4MA      400     /* 4mA   - minimum */
//#define OUTPUT_20MA     2000     /* 20mA  - maximum */
//#define OUTPUT_3_5MA    350     /* 3.5mA - downscale fault */
//
///* ---------- Output direction ----------------------------------------- */
//typedef enum {
//    OUTPUT_UP_SCALE   = 0,
//    OUTPUT_DOWN_SCALE = 1
//} OutputDirection_t;
//
///* ---------- Main output handler --------------------------------------- */
//typedef struct {
//    uint16_t          pulse_value;    /* current PWM (200-999) */
//    uint16_t          virtual_value;  /* high-res value (0-10000) */
//    uint16_t          output_low;     /* Modbus range low */
//    uint16_t          output_high;    /* Modbus range high */
//    uint8_t           output_decimal; /* decimal point */
//    OutputDirection_t direction;      /* up/down */
//    int16_t           offset;         /* offset */
//    int16_t           output_value;   /* Modbus value */
//    bool              sensor_fault;   /* fault flag */
//
//    /* Calibration values - loaded from device_config */
//    uint16_t          cal_output_lo;  /* PWM at 0°C */
//    uint16_t          cal_output_hi;  /* PWM at full scale */
//} AnalogOutput_Handle_t;
//
//extern AnalogOutput_Handle_t AOPHandler;
//
///* ---------- Core API ------------------------------------------------- */
//void     AnalogOutput_Init(TIM_HandleTypeDef *htim);
//void     AnalogOutput_Update(float input_value, float input_low, float input_high);
//void     AnalogOutput_Set_Range(uint16_t low, uint16_t high);
//void     AnalogOutput_Set_Direction(OutputDirection_t dir);
//void     AnalogOutput_Set_Offset(int16_t offset);
//void     AnalogOutput_Set_Decimal(uint8_t decimal);
//void     AnalogOutput_Set_Fault(bool fault);
//uint16_t AnalogOutput_Get_Pulse(void);
//uint16_t AnalogOutput_Get_Virtual_Value(void);
//int16_t  AnalogOutput_Get_Modbus_Value(void);
//
///* ---------- Calibration API ------------------------------------------ */
//void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm);
//
///* ---------- Calibration Bypass (prevents fluctuation) --------------- */
//void AnalogOutput_Calibration_Bypass(bool enable);
//void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value);
//
//
//#endif /* INC_OUTPUT_H_ */


/*
 * Output.h
 *
 *  Created on: 16-Jan-2026
 *      Author: visakh.p
 */

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

//#include "stm32f0xx_hal.h"
//#include <stdbool.h>
//
//// PWM Configuration
//#define PWM_PERIOD          999     // 0-999 = 1000 steps
//#define PWM_FREQUENCY       2000    // 2kHz
//#define PWM_MIN_PULSE       0       // 0mA
//#define PWM_MAX_PULSE       999     // 20mA
//
//// Output range in pulse counts
//#define OUTPUT_0MA          0       // 0mA = 0 pulse
//#define OUTPUT_3_5MA        175     // 3.5mA = 175 pulse (17.5% of 1000)
//#define OUTPUT_4MA          200     // 4mA = 200 pulse (20% of 1000)
//#define OUTPUT_20MA         999     // 20mA = 999 pulse
//#define OUTPUT_20_5MA       1024    // 20.5mA = 1024 pulse (clamped to 999)
//
//// Output direction
//typedef enum {
//    OUTPUT_DOWN_SCALE = 0,  // Fault = 3.5mA
//    OUTPUT_UP_SCALE = 1     // Fault = 20.5mA
//} OutputDirection_t;
//
//// Analog Output Handler
//typedef struct {
//    uint16_t pulse_value;           // Current PWM pulse value (0-999)
//    int16_t output_value;           // Scaled output value for Modbus
//    uint16_t output_low;            // Low range limit
//    uint16_t output_high;           // High range limit
//    uint8_t output_decimal;         // Decimal point (0-3)
//    OutputDirection_t direction;    // Scale direction
//    int16_t offset;                 // Output offset
//    bool sensor_fault;              // Sensor fault flag
//} AnalogOutput_Handle_t;
//
//// Public functions
//void AnalogOutput_Init(TIM_HandleTypeDef *htim);
//void AnalogOutput_Update(float input_value, float input_low, float input_high);
//void AnalogOutput_Set_Range(uint16_t low, uint16_t high);
//void AnalogOutput_Set_Direction(OutputDirection_t dir);
//void AnalogOutput_Set_Offset(int16_t offset);
//void AnalogOutput_Set_Decimal(uint8_t decimal);
//void AnalogOutput_Set_Fault(bool fault);
//uint16_t AnalogOutput_Get_Pulse(void);
//int16_t AnalogOutput_Get_Modbus_Value(void);

//#include "stm32f0xx_hal.h"
//#include <stdbool.h>
//
///* ---------- PWM pulse limits (TIM3 Period = 999) ---------- */
//#define OUTPUT_4MA      200     /* 4mA   - minimum */
//#define OUTPUT_20MA     999     /* 20mA  - maximum */
//#define OUTPUT_3_5MA    175     /* 3.5mA - downscale fault */
//
///* ---------- Output direction ----------------------------------------- */
//typedef enum {
//    OUTPUT_UP_SCALE   = 0,
//    OUTPUT_DOWN_SCALE = 1
//} OutputDirection_t;
//
///* ---------- Main output handler --------------------------------------- */
//typedef struct {
//    uint16_t          pulse_value;    /* current PWM (200-999) */
//    uint16_t          virtual_value;  /* high-res value (0-10000) ← NEW */
//    uint16_t          output_low;     /* Modbus range low */
//    uint16_t          output_high;    /* Modbus range high */
//    uint8_t           output_decimal; /* decimal point */
//    OutputDirection_t direction;      /* up/down */
//    int16_t           offset;         /* offset */
//    int16_t           output_value;   /* Modbus value */
//    bool              sensor_fault;   /* fault flag */
//
//    /* Calibration values - loaded from device_config */
//    uint16_t          cal_output_lo;  /* PWM at 0°C (from device_config) */
//    uint16_t          cal_output_hi;  /* PWM at full scale (from device_config) */
//} AnalogOutput_Handle_t;
//
//extern AnalogOutput_Handle_t AOPHandler;
//
///* ---------- Core API ------------------------------------------------- */
//void     AnalogOutput_Init(TIM_HandleTypeDef *htim);
//void     AnalogOutput_Update(float input_value, float input_low, float input_high);
//void     AnalogOutput_Set_Range(uint16_t low, uint16_t high);
//void     AnalogOutput_Set_Direction(OutputDirection_t dir);
//void     AnalogOutput_Set_Offset(int16_t offset);
//void     AnalogOutput_Set_Decimal(uint8_t decimal);
//void     AnalogOutput_Set_Fault(bool fault);
//uint16_t AnalogOutput_Get_Pulse(void);
//uint16_t AnalogOutput_Get_Virtual_Value(void);  /* ← NEW: get high-res value */
//int16_t  AnalogOutput_Get_Modbus_Value(void);
//
///* ---------- Calibration API ------------------------------------------ */
//void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm);
//
///* ---------- Calibration Bypass (prevents fluctuation) --------------- */
//void AnalogOutput_Calibration_Bypass(bool enable);
//void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value);



#include "stm32f0xx_hal.h"
#include <stdbool.h>

/* ---------- PWM pulse limits (TIM3 Period = 2000, see main.c) ------ */
#define OUTPUT_4MA      400     /* 4mA   - nominal minimum (before calibration) */
#define OUTPUT_20MA     2000     /* 20mA  - nominal maximum (before calibration) */
#define OUTPUT_3_5MA    350     /* 3.5mA - downscale fault (legacy, unused) */

/* Absolute hardware ceiling = TIM3 ARR (Period = 2000). Fault pulses must
 * never exceed this. The actual 4mA/20mA points (cal_output_lo/hi) are
 * found via the output calibration menu with a meter, and will normally
 * land somewhere BELOW 2000 (and above 0) - that gap is what gives the
 * fault pulse room to sit above/below the real calibrated span. */
#define OUTPUT_PWM_ABS_MAX  2000

/* NOTE: There is no fixed "20.5mA" or "0.5mA" pulse count anymore.
 * The real 4mA/20mA points move with calibration (cal_output_lo/hi), so
 * the fault pulse is calculated at runtime, 0.5mA beyond whatever those
 * calibrated points actually are - see Get_Fault_Pulse() in Output.c. */

/* ---------- Output direction ----------------------------------------- */
typedef enum {
    OUTPUT_UP_SCALE   = 0,
    OUTPUT_DOWN_SCALE = 1
} OutputDirection_t;

/* ---------- Main output handler --------------------------------------- */
typedef struct {
    uint16_t          pulse_value;    /* current PWM (200-999) */
    uint16_t          virtual_value;  /* high-res value (0-10000) */
    uint16_t          output_low;     /* Modbus range low */
    uint16_t          output_high;    /* Modbus range high */
    uint8_t           output_decimal; /* decimal point */
    OutputDirection_t direction;      /* fault direction ONLY: UP_SCALE -> ~20.5mA fault,
                                          DOWN_SCALE -> ~3.5mA fault. Does NOT affect
                                          normal-mode scaling - that is purely determined
                                          by cal_output_lo/cal_output_hi (same as THP-3003). */
    int16_t           offset;         /* offset */
    int16_t           output_value;   /* Modbus value */
    bool              sensor_fault;   /* fault flag */

    /* Calibration values - loaded from device_config */
    uint16_t          cal_output_lo;  /* PWM at 0°C */
    uint16_t          cal_output_hi;  /* PWM at full scale */
} AnalogOutput_Handle_t;

extern AnalogOutput_Handle_t AOPHandler;

/* ---------- Core API ------------------------------------------------- */
void     AnalogOutput_Init(TIM_HandleTypeDef *htim);
void     AnalogOutput_Update(float input_value, float input_low, float input_high);
void     AnalogOutput_Set_Range(uint16_t low, uint16_t high);
void     AnalogOutput_Set_Direction(OutputDirection_t dir);
void     AnalogOutput_Set_Offset(int16_t offset);
void     AnalogOutput_Set_Decimal(uint8_t decimal);
void     AnalogOutput_Set_Fault(bool fault);
uint16_t AnalogOutput_Get_Pulse(void);
uint16_t AnalogOutput_Get_Virtual_Value(void);
int16_t  AnalogOutput_Get_Modbus_Value(void);

/* ---------- Calibration API ------------------------------------------ */
void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm);

/* ---------- Calibration Bypass (prevents fluctuation) --------------- */
void AnalogOutput_Calibration_Bypass(bool enable);
void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value);


#endif /* INC_OUTPUT_H_ */
