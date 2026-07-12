/*
 * keyboard.h
 *
 *  Created on: Jan 5, 2026
 *      Author: Visakh Puliyannoor
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "stm32f0xx_hal.h"
#include <stdbool.h>
#include "main.h"
#include "lookup_tables.h"  // We'll create this separately


/* Key IDs */
//typedef enum {
//    KEY_K1 = 0,
//    KEY_K2 = 1,
//    KEY_K3 = 2,
//    KEY_K4 = 3,
//    KEY_COUNT = 4
//} KeyID_e;

// Key definitions
#define KEY_SET     0x01    // K1
#define KEY_UP      0x02    // K2
#define KEY_DOWN    0x04    // K3
#define KEY_ENTER   0x08    // K4


///* LED States */
//typedef enum {
//    LED_OFF = 0,
//    LED_ON = 1,
//    LED_FLASH = 2
//} LEDState_e;

// LED state structure
typedef struct {
    bool op_on;
    bool hi_on;
    bool lo_on;
    bool inp_on;
    bool led_1_on;
    bool led_2_on;
    bool op_flash;
    bool hi_flash;
    bool lo_flash;
    bool inp_flash;
    bool led_1_flash;
    bool led_2_flash;
} LED_State_t;

/* Operation Modes */
typedef enum {
	MODE_NORMAL = 0,
	MODE_INPUT_CAL,
	MODE_INPUT_CAL_LOW,
	MODE_INPUT_CAL_HIGH,
	MODE_INPUT_CAL_DONE,
	MODE_OUTPUT_CAL,
	MODE_OUTPUT_CAL_LOW,
	MODE_OUTPUT_CAL_HIGH,
	MODE_OUTPUT_CAL_DONE,
	MODE_STORE_CAL,
	MODE_STORE_CAL_DONE,
	MODE_RESTORE_CAL,
	MODE_RESTORE_CAL_DONE,
	MODE_SENSOR_BROKE,
	MODE_COMM_FAILED,
	MODE_MEMORY_CORRUPT,
	MODE_PWM_TEST//New menu
} OperationMode_e;//OK

/*Calibration Parameters*/
typedef struct
{
	float    OECode,GECode;
	uint16_t FLAGCode;
	uint16_t Hiop_Count,Loop_Count;
	float    cjc_value;
}Calibration_parameter;

typedef struct
{
	uint16_t Input_type;										//0
	uint16_t Low_Limit_input;									//1
	uint16_t High_Limit_input;									//2
	uint16_t Decimal_point_input;								//3
	uint16_t Low_Limit_output;									//4
	uint16_t High_Limit_output;									//5
	uint16_t Decimal_point_output;								//6
	uint16_t Output_Direction;  // upscale/Downscale seltion		//7
	uint16_t Sensor_break;										//8
	uint16_t offset_input;										//9
	uint16_t offset_output;										//10
	uint16_t Negative_Sign_Bit;									//11
	uint16_t reserved_1;										//12
	uint16_t Communication_EN_DIS;  								//13
	uint16_t SlaveID;											//14
	uint16_t Parity; 											//15
	uint16_t Stop; 												//16
	uint16_t BaudRate;											//17
	uint16_t Databit;	//n of 8n1								//18
	uint16_t Sensor_fault_detection_time;						//19
	uint16_t Reserved_2;										//20
	uint16_t Reserved_3;										//21
	uint16_t Reserved_4;										//22
	uint16_t Reserved_5;										//23
	uint16_t Reserved_6;										//24
	uint16_t Reserved_7;										//25
	uint16_t Reserved_8;										//26
	uint16_t Reserved_9;										//27
	uint16_t Reserved_10;										//28
	uint16_t Reserved_11;										//29
	uint16_t Input_Type_Selection_for_CAL;						//30
	uint16_t Input_Cal_Decimal_Point;							//31
	uint16_t Input_Cal_LO;										//32
	uint16_t Input_Cal_HI;										//33
	uint16_t Output_Cal_LO;										//34
	uint16_t Output_Cal_HI;										//35
	uint16_t Restore_Cal;										//36
	uint16_t Memory_Corrupted;									//37
	uint16_t Reserved_12;										//38
	uint16_t Reserved_13;										//39
}General_Parameter;


/*Input types*/

//typedef enum {
//	RTD = 	0,		//0
//	mA_4_20,		//1
//	V_0_1,			//2
//	V_0_5,			//3
//	V_0_10,			//4
//	K_TC,			//5
//	J_TC,			//6
//	R_TC,			//7
//	S_TC,			//8
//	B_TC			//9
//} SensorType_t;

/*Decimal Point*/
typedef enum {
	res_0 = 0,		//0
	res_0_1,		//1
	res_0_01,		//2
	res_0_001		//3
} Decimal_t;

/*Output Direction*/

typedef enum {
	upscale = 0,	//0
	downscale		//1
} output_direction;

/*Sensor Break*/
typedef enum {
	Break = 0,		//0
	Found_OK		//1
} Sensor_status;

/*Communication Enable Disable*/
typedef enum {
	Enable = 0,		//0
	Disable			//1
} Communication;


/*Parity*/
typedef enum {
	None = 0,		//0
	Odd,			//1
	Even			//2
} Parity_bit;

/*Stop Bit*/
typedef enum {
	Bit_1 = 0,		//0
	Bit_2,			//1
} Stop_bit;

/*Baud Rate*/
typedef enum {
	BR_9600 = 0,	//0
	BR_19200,		//1
	BR_38400,		//2
	BR_57600		//3
} Baud_Rate;

/*Memory Corruption*/
typedef enum {
	Corrupted = 0,	//0
	Healthy,		//1
} Memory_Health_Status;






//
///**
// * @brief  Initialize GPIO control system
// * @param  None
// * @retval None
// */
//void GPIO_Control_Init(void);
//
///**
// * @brief  Update tick counter - call from 10ms timer interrupt
// * @param  None
// * @retval None
// */
//void Keys_UpdateTick(void);
//
///**
// * @brief  Get current tick count
// * @param  None
// * @retval Tick count (10ms resolution)
// */
//uint32_t Keys_GetTick(void);
//
///**
// * @brief  Scan all keys - call every 10ms
// * @param  None
// * @retval None
// */
//void Keys_Scan(void);
//
///**
// * @brief  Check if key is currently pressed
// * @param  key: Key ID
// * @retval 1 if pressed, 0 if released
// */
//uint8_t Keys_IsPressed(KeyID_e key);
//
///**
// * @brief  Check if key was just pressed (edge detection)
// * @param  key: Key ID
// * @retval 1 on rising edge, 0 otherwise
// */
//uint8_t Keys_WasPressed(KeyID_e key);
//
///**
// * @brief  Check if key held for duration
// * @param  key: Key ID
// * @param  durationMs: Duration in milliseconds
// * @retval 1 if held, 0 otherwise
// */
//uint8_t Keys_pressed_Duration(KeyID_e key, uint32_t durationMs);
//
///**
// * @brief  Check if 5 second long press detected
// * @param  key: Key ID
// * @retval 1 if detected, 0 otherwise
// */
//uint8_t Keys_LongPress_5s(KeyID_e key);
//
///**
// * @brief  Check if 10 second long press detected
// * @param  key: Key ID
// * @retval 1 if detected, 0 otherwise
// */
//uint8_t Keys_LongPress_10s(KeyID_e key);
//
///**
// * @brief  Check if two keys pressed together
// * @param  key1: First key
// * @param  key2: Second key
// * @retval 1 if both pressed, 0 otherwise
// */
//uint8_t Keys_AreBothPressed(KeyID_e key1, KeyID_e key2);
//
///**
// * @brief  Clear long press flags
// * @param  key: Key ID
// * @retval None
// */
//void Keys_ClearLongPress(KeyID_e key);
//
///**
// * @brief  Set LED state
// * @param  ledIndex: LED index (0-3)
// * @param  state: LED_OFF, LED_ON, or LED_FLASH
// * @retval None
// */
//void LED_SetState(uint8_t ledIndex, LEDState_e state);
//
///**
// * @brief  Update all LEDs (for flashing)
// * @param  None
// * @retval None
// */
//void LED_UpdateAll(void);
//
///**
// * @brief  Set LED pattern for mode
// * @param  mode: Operation mode
// * @retval None
// */
//void LED_SetModePattern(OperationMode_e mode);
//
///**
// * @brief  Check power-on key combinations
// * @param  None
// * @retval None
// */
//void CheckPowerOnKeyCombinations(void);
//



// Public functions
// Public functions
void Keyboard_Init(void);
void Keyboard_Scan(void);
void Keyboard_LED_Update(void);
OperationMode_e Keyboard_GetMode(void);
void Keyboard_SetMode(OperationMode_e mode);
uint8_t Keyboard_GetKeyPressed(void);

// Reference pattern functions (called from main loop)
uint8_t Read_Keys(void);
//Visakh on 12/07/2026***********starts
uint8_t Read_Keys_Stable(void);
uint8_t Keys_GetEvent(void);
//Visakh on 12/07/2026***********ends
void Keycheck_Normal(void);
void Keycheck_Calibration(void);
void Fast_value_Inc(void);
void Fast_value_Dec(void);
void random_delay(char delay);

// Keyboard structure (reference pattern)
typedef struct {
	uint8_t Fast_Value_Change;   // Enable/disable fast value change
	int16_t New_Val;              // Counter for acceleration
	uint32_t Key_Stop_Counter;    // Counter for long press detection
	uint8_t KeyPressed;           // Current key state
} Keyboard_t;

// Long press timing (reference pattern)
#define LONG_KEY_COUNT      50      // 5 seconds (50 * 100ms)
#define LONG_KEY_COUNT_10S  100     // 10 seconds (100 * 100ms)

// CAL_Flag for mode tracking (reference pattern)
#define NORMAL_MODE  0
#define CAL_MODE     1
extern uint8_t CAL_Flag;

// External keyboard structure
extern Keyboard_t Keyboard;


#endif /* INC_KEYBOARD_H_ */
