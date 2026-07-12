///*
// * Output.c
// *
// *  Created on: 16-Jan-2026
// *      Author: visakh.p
// */
//
//
//
//#include "Output.h"
//
///* ---------- module state -------------------------------------- */
//static TIM_HandleTypeDef *pwm_timer;
//AnalogOutput_Handle_t     AOPHandler;
//
///* Calibration bypass mode */
//static bool calibration_bypass_mode = false;
//static uint16_t bypass_pwm_value =0; //200;
//
///* ---------- private helpers ---------------------------------- */
////static uint16_t Scale_To_PWM_HighRes(float input_value, float input_low, float input_high);
//static uint16_t Scale_To_PWM(float input_value, float input_low, float input_high);
//static int16_t  Scale_To_Modbus(uint16_t pulse_value);
//static void     Set_Fault_Output(void);
//
///* ================================================================== */
///*  INIT                                                               */
///* ================================================================== */
//void AnalogOutput_Init(TIM_HandleTypeDef *htim)
//{
//    pwm_timer = htim;
//
//    /* defaults */
//    AOPHandler.pulse_value     = 0;
//    AOPHandler.virtual_value   = 0;
//    AOPHandler.output_low      = 0;
//    AOPHandler.output_high     = 2000;
//    AOPHandler.output_decimal  = 1;
//    AOPHandler.direction       = OUTPUT_UP_SCALE;
//    AOPHandler.offset          = 0;
//    AOPHandler.output_value    = 0;
//    AOPHandler.sensor_fault    = false;
//
//    /* calibration defaults - will be loaded from FRAM */
//    AOPHandler.cal_output_lo   = OUTPUT_4MA;   /* 200 */
//    AOPHandler.cal_output_hi   = OUTPUT_20MA;  /* 999 */
//
//    calibration_bypass_mode = false;
//
//    /* start PWM */
//    HAL_TIM_PWM_Start(pwm_timer, TIM_CHANNEL_1);
//    __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, AOPHandler.pulse_value);
//}
//
///* ================================================================== */
///*  CALIBRATION BYPASS MODE                                            */
///* ================================================================== */
//
//void AnalogOutput_Calibration_Bypass(bool enable)
//{
//    calibration_bypass_mode = enable;
//}
//
//void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value)
//{
//    /* During calibration, allow 200-999 only */
//    if (pwm_value < 0)  pwm_value = 0;//OUTPUT_4MA;
////    if (pwm_value > OUTPUT_20MA) pwm_value = OUTPUT_20MA;
//	if (pwm_value > 2000) pwm_value = 2000;   // Only clamp upper bound, allow full 0-1000
//
//
//    bypass_pwm_value = pwm_value;
//
//    if (calibration_bypass_mode) {
//        AOPHandler.pulse_value = pwm_value;
//        __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, pwm_value);
//    }
//}
//
///* ================================================================== */
///*  UPDATE (called every loop)                                         */
///* ================================================================== */
//void AnalogOutput_Update(float input_value, float input_low, float input_high)
//{
//    /* If in calibration bypass mode, don't update from temperature */
//    if (calibration_bypass_mode) {
//        AOPHandler.pulse_value = bypass_pwm_value;
//        __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, bypass_pwm_value);
//        AOPHandler.output_value = Scale_To_Modbus(bypass_pwm_value);
//        return;
//    }
//
//    /* Normal operation with HIGH RESOLUTION */
//    if (AOPHandler.sensor_fault) {
//        Set_Fault_Output();
//        AOPHandler.virtual_value = 0;
//    } else {
////        AOPHandler.pulse_value = Scale_To_PWM_HighRes(input_value, input_low, input_high);
//
//    	 AOPHandler.pulse_value = Scale_To_PWM(input_value, input_low, input_high);
//        /* virtual_value is set inside Scale_To_PWM_HighRes() */
//    }
//
//    __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, AOPHandler.pulse_value);
//    AOPHandler.output_value = Scale_To_Modbus(AOPHandler.pulse_value);
//}
//
///* ================================================================== */
///*  FAULT OUTPUT - ALLOWS VALUES OUTSIDE 4-20mA RANGE                 */
///*                                                                     */
///*  UPSCALE FAULT:   20.5mA → PWM 999 (max hardware allows)          */
///*  DOWNSCALE FAULT: 3.5mA  → PWM 175 (below 4mA minimum)            */
///*                                                                     */
///*  These values are NOT clamped - they indicate fault condition!     */
///* ================================================================== */
//static void Set_Fault_Output(void)
//{
//    if (AOPHandler.direction == OUTPUT_UP_SCALE) {
//        /* Upscale fault: Maximum possible (20mA at 999, hardware can't go higher) */
//        AOPHandler.pulse_value = OUTPUT_20MA;   /* 999 = ~20mA */
//    } else {
//        /* Downscale fault: Below 4mA to indicate fault */
//        AOPHandler.pulse_value = OUTPUT_3_5MA;  /* 175 = 3.5mA */
//    }
//    /* NOTE: No clamping here! Fault values are allowed outside normal range */
//}
//
///* ================================================================== */
///*  SCALING WITH 10000 VIRTUAL STEPS (HIGH RESOLUTION)                */
///*                                                                     */
///*  IMPORTANT: Normal operation clamps to 200-999 (4-20mA)           */
///*             Fault conditions bypass this clamping                  */
///* ================================================================== */
////static uint16_t Scale_To_PWM_HighRes(float input_value, float input_low, float input_high)
////{
////    float    normalized;
////    uint16_t virtual;
////    int32_t  pulse;
////
////    uint16_t cal_low  = AOPHandler.cal_output_lo;
////    uint16_t cal_high = AOPHandler.cal_output_hi;
////
////    /* calculate normalized 0.0 to 1.0 */
////    if (input_high != input_low) {
////        normalized = (input_value - input_low) / (input_high - input_low);
////    } else {
////        normalized = 0.0f;
////    }
////
////    /* clamp normalized value */
////    if (normalized < 0.0f) normalized = 0.0f;
////    if (normalized > 1.0f) normalized = 1.0f;
////
////    /* apply direction */
////    if (AOPHandler.direction == OUTPUT_DOWN_SCALE) {
////        normalized = 1.0f - normalized;
////    }
////
////    /* Calculate VIRTUAL value (0-10000) ← HIGH RESOLUTION */
////    virtual = (uint16_t)(normalized * 10000.0f);
////    if (virtual > 10000) virtual = 10000;
////
////    /* Store virtual value for Modbus reporting */
////    AOPHandler.virtual_value = virtual;
////
////    /* Map virtual 0-10000 to hardware PWM cal_low...cal_high */
////    pulse = cal_low + (int32_t)((virtual / 10000.0f) * (float)(cal_high - cal_low));
////
////    /* ============================================================ */
////    /*  CLAMP TO 4-20mA RANGE (only for normal operation)         */
////    /*  Fault conditions set PWM directly in Set_Fault_Output()   */
////    /* ============================================================ */
//////    if (pulse < OUTPUT_4MA)  pulse = OUTPUT_4MA;
//////    if (pulse > OUTPUT_20MA) pulse = OUTPUT_20MA;
////    if (pulse > 2000)        pulse = 2000;
////
////    return (uint16_t)pulse;
////}
//
//
//static uint16_t Scale_To_PWM(float input_value, float input_low, float input_high)
//{
//    float   normalized;
//    int32_t pulse;
//
//    int32_t cal_low  = (int32_t)AOPHandler.cal_output_lo;
//    int32_t cal_high = (int32_t)AOPHandler.cal_output_hi;
//
//    if (input_high != input_low)
//        normalized = (input_value - input_low) / (input_high - input_low);
//    else
//        normalized = 0.0f;
//
//    if (normalized < 0.0f) normalized = 0.0f;
//    if (normalized > 1.0f) normalized = 1.0f;
//
//    if (AOPHandler.direction == OUTPUT_DOWN_SCALE)
//        normalized = 1.0f - normalized;
//
//    pulse = cal_low + (int32_t)(normalized * (float)(cal_high - cal_low));
//
//    if (pulse < 0)    pulse = 0;     /* safe if cal_high < cal_low (downscale cal) */
//    if (pulse > 2000) pulse = 2000;
//
//    AOPHandler.virtual_value = 0;    /* virtual layer retired */
//    return (uint16_t)pulse;
//}
//
//
///* ================================================================== */
///*  MODBUS VALUE (uses virtual_value for high precision)             */
///* ================================================================== */
//static int16_t Scale_To_Modbus(uint16_t pulse_value)
//{
////    float   output_range = (float)(AOPHandler.output_high - AOPHandler.output_low);
////    int16_t modbus_value;
////
////    /* Use virtual value if available (high resolution path) */
////    if (AOPHandler.virtual_value > 0) {
////        float norm = (float)AOPHandler.virtual_value / 10000.0f;
////        modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
////    } else {
////        /* Fallback: use PWM pulse (low resolution path) */
////        float pulse_range = (float)(OUTPUT_20MA - OUTPUT_4MA);   /* 799 */
////        if (pulse_value <= OUTPUT_4MA) {
////            modbus_value = AOPHandler.output_low;
////        } else if (pulse_value >= OUTPUT_20MA) {
////            modbus_value = AOPHandler.output_high;
////        } else {
////            float norm = (float)(pulse_value - OUTPUT_4MA) / pulse_range;
////            modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
////        }
////    }
////
////    modbus_value += AOPHandler.offset;
////    return modbus_value;
//
//	 float   output_range = (float)((int32_t)AOPHandler.output_high -
//	                                   (int32_t)AOPHandler.output_low);
//	    float   pulse_range  = (float)(OUTPUT_20MA - OUTPUT_4MA);   /* 1600 */
//	    int16_t modbus_value;
//
//	    if (pulse_value <= OUTPUT_4MA) {
//	        modbus_value = AOPHandler.output_low;
//	    } else if (pulse_value >= OUTPUT_20MA) {
//	        modbus_value = AOPHandler.output_high;
//	    } else {
//	        float norm = (float)(pulse_value - OUTPUT_4MA) / pulse_range;
//	        modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
//	    }
//
//	    modbus_value += AOPHandler.offset;
//	    return modbus_value;
//}
//
///* ================================================================== */
///*  SETTERS / GETTERS                                                  */
///* ================================================================== */
//void AnalogOutput_Set_Range(uint16_t low, uint16_t high)
//{
//    AOPHandler.output_low  = low;
//    AOPHandler.output_high = high;
//}
//
//void AnalogOutput_Set_Direction(OutputDirection_t dir)
//{
//    AOPHandler.direction = dir;
//}
//
//void AnalogOutput_Set_Offset(int16_t offset)
//{
//    AOPHandler.offset = offset;
//}
//
//void AnalogOutput_Set_Decimal(uint8_t decimal)
//{
//    if (decimal <= 3)
//        AOPHandler.output_decimal = decimal;
//}
//
//void AnalogOutput_Set_Fault(bool fault)
//{
//    AOPHandler.sensor_fault = fault;
//}
//
//uint16_t AnalogOutput_Get_Pulse(void)
//{
//    return AOPHandler.pulse_value;
//}
//
//uint16_t AnalogOutput_Get_Virtual_Value(void)
//{
//    return AOPHandler.virtual_value;
//}
//
//int16_t AnalogOutput_Get_Modbus_Value(void)
//{
//    return AOPHandler.output_value;
//}
//
///* ================================================================== */
///*  CALIBRATION API                                                    */
///* ================================================================== */
//
//void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm)
//{
//    /* clamp to valid range */
////    if (low_pwm < OUTPUT_4MA)   low_pwm = OUTPUT_4MA;
////    if (low_pwm > OUTPUT_20MA)  low_pwm = OUTPUT_20MA;
////    if (high_pwm < OUTPUT_4MA)  high_pwm = OUTPUT_4MA;
////    if (high_pwm > OUTPUT_20MA) high_pwm = OUTPUT_20MA;
//
//	if (low_pwm  < 0)  low_pwm  = 0;
//	if (low_pwm  > 2000)        low_pwm  = 2000;
//	if (high_pwm < 0)  high_pwm = 0;
//	if (high_pwm > 2000)        high_pwm = 2000;
//
//    /* store */
//    AOPHandler.cal_output_lo = low_pwm;
//    AOPHandler.cal_output_hi = high_pwm;
//}


/*
 * Output.c
 *
 *  Created on: 16-Jan-2026
 *      Author: visakh.p
 */



#include "Output.h"

/* ---------- module state -------------------------------------- */
static TIM_HandleTypeDef *pwm_timer;
AnalogOutput_Handle_t     AOPHandler;

/* Calibration bypass mode */
static bool calibration_bypass_mode = false;
static uint16_t bypass_pwm_value =0; //200;

/* ---------- private helpers ---------------------------------- */
//static uint16_t Scale_To_PWM_HighRes(float input_value, float input_low, float input_high);
static uint16_t Scale_To_PWM(float input_value, float input_low, float input_high);
static int16_t  Scale_To_Modbus(uint16_t pulse_value);
static void     Set_Fault_Output(void);
static uint16_t Get_Fault_Pulse(bool upscale);

/* ================================================================== */
/*  INIT                                                               */
/* ================================================================== */
void AnalogOutput_Init(TIM_HandleTypeDef *htim)
{
    pwm_timer = htim;

    /* defaults */
//    AOPHandler.pulse_value     = 0;
//    AOPHandler.virtual_value   = 0;
//    AOPHandler.output_low      = 0;
//    AOPHandler.output_high     = 2000;
//    AOPHandler.output_decimal  = 1;
//    AOPHandler.direction       = OUTPUT_UP_SCALE;
//    AOPHandler.offset          = 0;
//    AOPHandler.output_value    = 0;
//    AOPHandler.sensor_fault    = false;
//
//    /* calibration defaults - will be loaded from FRAM */
//    AOPHandler.cal_output_lo   = OUTPUT_4MA;   /* 200 */
//    AOPHandler.cal_output_hi   = OUTPUT_20MA;  /* 999 */

    calibration_bypass_mode = false;

    /* start PWM */
    HAL_TIM_PWM_Start(pwm_timer, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, AOPHandler.pulse_value);
}

/* ================================================================== */
/*  CALIBRATION BYPASS MODE                                            */
/* ================================================================== */

void AnalogOutput_Calibration_Bypass(bool enable)
{
    calibration_bypass_mode = enable;
}

void AnalogOutput_Set_Raw_PWM(uint16_t pwm_value)
{
    /* During calibration, allow 200-999 only */
    if (pwm_value < 0)  pwm_value = 0;//OUTPUT_4MA;
//    if (pwm_value > OUTPUT_20MA) pwm_value = OUTPUT_20MA;
	if (pwm_value > 2000) pwm_value = 2000;   // Only clamp upper bound, allow full 0-1000


    bypass_pwm_value = pwm_value;

    if (calibration_bypass_mode) {
        AOPHandler.pulse_value = pwm_value;
        __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, pwm_value);
    }
}

/* ================================================================== */
/*  UPDATE (called every loop)                                         */
/* ================================================================== */
void AnalogOutput_Update(float input_value, float input_low, float input_high)
{
    /* If in calibration bypass mode, don't update from temperature */
    if (calibration_bypass_mode) {
        AOPHandler.pulse_value = bypass_pwm_value;
        __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, bypass_pwm_value);
        AOPHandler.output_value = Scale_To_Modbus(bypass_pwm_value);
        return;
    }

    /* Normal operation with HIGH RESOLUTION */
    if (AOPHandler.sensor_fault) {
        Set_Fault_Output();
        AOPHandler.virtual_value = 0;
    } else {
//        AOPHandler.pulse_value = Scale_To_PWM_HighRes(input_value, input_low, input_high);

    	 AOPHandler.pulse_value = Scale_To_PWM(input_value, input_low, input_high);
        /* virtual_value is set inside Scale_To_PWM_HighRes() */
    }

    __HAL_TIM_SET_COMPARE(pwm_timer, TIM_CHANNEL_1, AOPHandler.pulse_value);
    AOPHandler.output_value = Scale_To_Modbus(AOPHandler.pulse_value);
}

/* ================================================================== */
/*  FAULT OUTPUT - ALLOWS VALUES OUTSIDE 4-20mA RANGE                 */
/*                                                                     */
/*  UPSCALE FAULT:   20.5mA → PWM 999 (max hardware allows)          */
/*  DOWNSCALE FAULT: 3.5mA  → PWM 175 (below 4mA minimum)            */
/*                                                                     */
/*  These values are NOT clamped - they indicate fault condition!     */
/* ================================================================== */
static void Set_Fault_Output(void)
{
    if (AOPHandler.direction == OUTPUT_UP_SCALE) {
        /* Upscale fault: 0.5mA above the CALIBRATED 20mA point -> ~20.5mA */
        AOPHandler.pulse_value = Get_Fault_Pulse(true);
    } else {
        /* Downscale fault: 0.5mA below the CALIBRATED 4mA point -> ~3.5mA */
        AOPHandler.pulse_value = Get_Fault_Pulse(false);
    }
    /* NOTE: No clamping to the normal 4-20mA band here - fault values are
     * meant to sit outside it. Only the absolute hardware ceiling applies,
     * enforced inside Get_Fault_Pulse(). */
}

/* ================================================================== */
/*  FAULT PULSE - CALCULATED RELATIVE TO ACTUAL CALIBRATION            */
/*                                                                      */
/*  The real 4mA/20mA points are wherever cal_output_lo/cal_output_hi  */
/*  were set during calibration - NOT necessarily 400/2000. So the     */
/*  fault pulse must be derived from the calibrated slope, not a       */
/*  fixed constant, or it may not actually reach ~3.5mA/~20.5mA once   */
/*  calibration has been applied.                                      */
/* ================================================================== */
static uint16_t Get_Fault_Pulse(bool upscale)
{
    int32_t cal_low  = (int32_t)AOPHandler.cal_output_lo;   /* actual 4mA point  */
    int32_t cal_high = (int32_t)AOPHandler.cal_output_hi;   /* actual 20mA point */

    /* counts per mA, derived from the CALIBRATED span, not the nominal one */
    float counts_per_mA = (float)(cal_high - cal_low) / 16.0f;  /* 4-20mA = 16mA span */

    int32_t pulse;
    if (upscale) {
        pulse = cal_high + (int32_t)(counts_per_mA * 0.5f);  /* +0.5mA beyond calibrated 20mA */
    } else {
        pulse = cal_low  - (int32_t)(counts_per_mA * 0.5f);  /* -0.5mA below calibrated 4mA */
    }

    /* Only clamp to the absolute hardware limit (TIM3 ARR) - deliberately
     * NOT clamped to the normal 4-20mA band, since the whole point is to
     * sit outside it. */
    if (pulse < 0) pulse = 0;
    if (pulse > OUTPUT_PWM_ABS_MAX) pulse = OUTPUT_PWM_ABS_MAX;

    return (uint16_t)pulse;
}

/* ================================================================== */
/*  SCALING WITH 10000 VIRTUAL STEPS (HIGH RESOLUTION)                */
/*                                                                     */
/*  IMPORTANT: Normal operation clamps to 200-999 (4-20mA)           */
/*             Fault conditions bypass this clamping                  */
/* ================================================================== */
//static uint16_t Scale_To_PWM_HighRes(float input_value, float input_low, float input_high)
//{
//    float    normalized;
//    uint16_t virtual;
//    int32_t  pulse;
//
//    uint16_t cal_low  = AOPHandler.cal_output_lo;
//    uint16_t cal_high = AOPHandler.cal_output_hi;
//
//    /* calculate normalized 0.0 to 1.0 */
//    if (input_high != input_low) {
//        normalized = (input_value - input_low) / (input_high - input_low);
//    } else {
//        normalized = 0.0f;
//    }
//
//    /* clamp normalized value */
//    if (normalized < 0.0f) normalized = 0.0f;
//    if (normalized > 1.0f) normalized = 1.0f;
//
//    /* apply direction */
//    if (AOPHandler.direction == OUTPUT_DOWN_SCALE) {
//        normalized = 1.0f - normalized;
//    }
//
//    /* Calculate VIRTUAL value (0-10000) ← HIGH RESOLUTION */
//    virtual = (uint16_t)(normalized * 10000.0f);
//    if (virtual > 10000) virtual = 10000;
//
//    /* Store virtual value for Modbus reporting */
//    AOPHandler.virtual_value = virtual;
//
//    /* Map virtual 0-10000 to hardware PWM cal_low...cal_high */
//    pulse = cal_low + (int32_t)((virtual / 10000.0f) * (float)(cal_high - cal_low));
//
//    /* ============================================================ */
//    /*  CLAMP TO 4-20mA RANGE (only for normal operation)         */
//    /*  Fault conditions set PWM directly in Set_Fault_Output()   */
//    /* ============================================================ */
////    if (pulse < OUTPUT_4MA)  pulse = OUTPUT_4MA;
////    if (pulse > OUTPUT_20MA) pulse = OUTPUT_20MA;
//    if (pulse > 2000)        pulse = 2000;
//
//    return (uint16_t)pulse;
//}


/* Normal-mode scaling: direction does NOT flip this mapping anymore.
 * The relationship between input and output is fixed by whatever is
 * calibrated into cal_output_lo (4mA point) and cal_output_hi (20mA
 * point) - same approach as the THP-3003 code. AOPHandler.direction is
 * now used ONLY to select the fault target (see Get_Fault_Pulse). */
static uint16_t Scale_To_PWM(float input_value, float input_low, float input_high)
{
    float   normalized;
    int32_t pulse;

    int32_t cal_low  = (int32_t)AOPHandler.cal_output_lo;
    int32_t cal_high = (int32_t)AOPHandler.cal_output_hi;

    if (input_high != input_low)
        normalized = (input_value - input_low) / (input_high - input_low);
    else
        normalized = 0.0f;

    if (normalized < 0.0f) normalized = 0.0f;
    if (normalized > 1.0f) normalized = 1.0f;

    pulse = cal_low + (int32_t)(normalized * (float)(cal_high - cal_low));

    if (pulse < 0)    pulse = 0;     /* safe if cal_high < cal_low */
    if (pulse > 2000) pulse = 2000;

    AOPHandler.virtual_value = 0;    /* virtual layer retired */
    return (uint16_t)pulse;
}


/* ================================================================== */
/*  MODBUS VALUE (uses virtual_value for high precision)             */
/* ================================================================== */
static int16_t Scale_To_Modbus(uint16_t pulse_value)
{
//    float   output_range = (float)(AOPHandler.output_high - AOPHandler.output_low);
//    int16_t modbus_value;
//
//    /* Use virtual value if available (high resolution path) */
//    if (AOPHandler.virtual_value > 0) {
//        float norm = (float)AOPHandler.virtual_value / 10000.0f;
//        modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
//    } else {
//        /* Fallback: use PWM pulse (low resolution path) */
//        float pulse_range = (float)(OUTPUT_20MA - OUTPUT_4MA);   /* 799 */
//        if (pulse_value <= OUTPUT_4MA) {
//            modbus_value = AOPHandler.output_low;
//        } else if (pulse_value >= OUTPUT_20MA) {
//            modbus_value = AOPHandler.output_high;
//        } else {
//            float norm = (float)(pulse_value - OUTPUT_4MA) / pulse_range;
//            modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
//        }
//    }
//
//    modbus_value += AOPHandler.offset;
//    return modbus_value;

	 float   output_range = (float)((int32_t)AOPHandler.output_high -
	                                   (int32_t)AOPHandler.output_low);
	    float   pulse_range  = (float)(OUTPUT_20MA - OUTPUT_4MA);   /* 1600 */
	    int16_t modbus_value;

	    if (pulse_value <= OUTPUT_4MA) {
	        modbus_value = AOPHandler.output_low;
	    } else if (pulse_value >= OUTPUT_20MA) {
	        modbus_value = AOPHandler.output_high;
	    } else {
	        float norm = (float)(pulse_value - OUTPUT_4MA) / pulse_range;
	        modbus_value = AOPHandler.output_low + (int16_t)(norm * output_range);
	    }

	    modbus_value += AOPHandler.offset;
	    return modbus_value;
}

/* ================================================================== */
/*  SETTERS / GETTERS                                                  */
/* ================================================================== */
void AnalogOutput_Set_Range(uint16_t low, uint16_t high)
{
    AOPHandler.output_low  = low;
    AOPHandler.output_high = high;
}

void AnalogOutput_Set_Direction(OutputDirection_t dir)
{
    AOPHandler.direction = dir;
}

void AnalogOutput_Set_Offset(int16_t offset)
{
    AOPHandler.offset = offset;
}

void AnalogOutput_Set_Decimal(uint8_t decimal)
{
    if (decimal <= 3)
        AOPHandler.output_decimal = decimal;
}

void AnalogOutput_Set_Fault(bool fault)
{
    AOPHandler.sensor_fault = fault;
}

uint16_t AnalogOutput_Get_Pulse(void)
{
    return AOPHandler.pulse_value;
}

uint16_t AnalogOutput_Get_Virtual_Value(void)
{
    return AOPHandler.virtual_value;
}

int16_t AnalogOutput_Get_Modbus_Value(void)
{
    return AOPHandler.output_value;
}

/* ================================================================== */
/*  CALIBRATION API                                                    */
/* ================================================================== */

void AnalogOutput_Set_Calibration(uint16_t low_pwm, uint16_t high_pwm)
{
    /* clamp to valid range */
//    if (low_pwm < OUTPUT_4MA)   low_pwm = OUTPUT_4MA;
//    if (low_pwm > OUTPUT_20MA)  low_pwm = OUTPUT_20MA;
//    if (high_pwm < OUTPUT_4MA)  high_pwm = OUTPUT_4MA;
//    if (high_pwm > OUTPUT_20MA) high_pwm = OUTPUT_20MA;

	if (low_pwm  < 0)  low_pwm  = 0;
	if (low_pwm  > 2000)        low_pwm  = 2000;
	if (high_pwm < 0)  high_pwm = 0;
	if (high_pwm > 2000)        high_pwm = 2000;

    /* store */
    AOPHandler.cal_output_lo = low_pwm;
    AOPHandler.cal_output_hi = high_pwm;
}

