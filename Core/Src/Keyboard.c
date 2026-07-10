///*
// * Keyboard.c
// *
// *  Created on: Jan 5, 2026
// *      Author: visak
// */
//
//#include "keyboard.h"
//
//
//
////static uint32_t systemTickCounter = 0;
//
//
//// Timing constants
//#define DEBOUNCE_TIME       50      // ms
//#define LONG_PRESS_5SEC     5000    // ms
//#define LONG_PRESS_10SEC    10000   // ms
//#define LED_FLASH_PERIOD    500     // ms
//
//// Private variables
//static OperationMode_e current_mode = MODE_NORMAL;
//static uint8_t key_state = 0;
//static uint8_t last_key_state = 0;
//static uint32_t key_press_start = 0;
//static bool power_on_flag = true;
//static LED_State_t led_state;
//
//// Private functions
//static uint8_t Read_Keys(void);
//static void Process_Key_Combination(uint8_t keys, uint32_t duration);
////static void Update_LED_Pattern(void);
//static void Set_LED_State_For_Mode(OperationMode_e mode);
//static void LED_Control(void);
//
//void Keyboard_Init(void)
//{
//	// Keys are already configured in CubeMX as GPIO inputs with pull-up
//	// LEDs are already configured in CubeMX as GPIO outputs
//
//	current_mode = MODE_NORMAL;
//	key_state = 0;
//	last_key_state = 0;
//	key_press_start = 0;
//	power_on_flag = true;
//
//	// Initialize LED state
//	led_state.op_on = true;
//	led_state.hi_on = true;
//	led_state.lo_on = true;
//	led_state.inp_on = true;
//	led_state.led_1_on = true;
//	led_state.led_2_on = true;
//	led_state.op_flash = false;
//	led_state.hi_flash = false;
//	led_state.lo_flash = false;
//	led_state.inp_flash = false;
//	led_state.led_1_flash = false;
//	led_state.led_2_flash = false;
//}
//
//static uint8_t Read_Keys(void)
//{
//	uint8_t keys = 0;
//
//	// Read all keys (active low with pull-up)
//	if (HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET)
//		keys |= KEY_SET;
//	if (HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET)
//		keys |= KEY_UP;
//	if (HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET)
//		keys |= KEY_DOWN;
//	if (HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin) == GPIO_PIN_RESET)
//		keys |= KEY_ENTER;
//
//	return keys;
//}
//
//void Keyboard_Scan(void)
//{
//	static uint32_t last_scan = 0;
//	uint32_t current_time = HAL_GetTick();
//
//	// Debounce
//	if (current_time - last_scan < DEBOUNCE_TIME)
//		return;
//
//	last_scan = current_time;
//	key_state = Read_Keys();
//
//	// Check for power-on key combinations (K3+K4 for Store Cal)
//	if (power_on_flag && (key_state == (KEY_DOWN | KEY_ENTER))) {
//		current_mode = MODE_STORE_CAL;
//		Set_LED_State_For_Mode(current_mode);
//		power_on_flag = false;
//		return;
//	}
//	power_on_flag = false;
//
//	// Detect key press start
//	if (key_state != 0 && last_key_state == 0) {
//		key_press_start = current_time;
//	}
//
//	// Process long press combinations
//	if (key_state != 0) {
//		uint32_t press_duration = current_time - key_press_start;
//		Process_Key_Combination(key_state, press_duration);
//	}
//
//	last_key_state = key_state;
//}
//
//static void Process_Key_Combination(uint8_t keys, uint32_t duration)
//{
//	static bool mode_changed = false;
//
//	// K1+K3 for 5 sec = Input Calibration
//	if (keys == (KEY_SET | KEY_DOWN) && duration >= LONG_PRESS_5SEC) {
//		if (current_mode == MODE_NORMAL && !mode_changed) {
//			current_mode = MODE_INPUT_CAL;
//			Set_LED_State_For_Mode(current_mode);
//			mode_changed = true;
//		}
//	}
//
//	// K1+K2 for 5 sec = Output Calibration
//	else if (keys == (KEY_SET | KEY_UP) && duration >= LONG_PRESS_5SEC) {
//		if (current_mode == MODE_NORMAL && !mode_changed) {
//			current_mode = MODE_OUTPUT_CAL;
//			Set_LED_State_For_Mode(current_mode);
//			mode_changed = true;
//		}
//	}
//
//	// K2+K4 for 10 sec = Restore Calibration
//	else if (keys == (KEY_UP | KEY_ENTER) && duration >= LONG_PRESS_10SEC) {
//		if (current_mode == MODE_NORMAL && !mode_changed) {
//			current_mode = MODE_RESTORE_CAL;
//			Set_LED_State_For_Mode(current_mode);
//			mode_changed = true;
//		}
//	}
//
//	// K4 = Mode transitions (on key release)
//	else if (/*keys == 0 &&*/ last_key_state == KEY_ENTER) {
//		mode_changed = false;
//
//		switch (current_mode) {
//		case MODE_INPUT_CAL:
//			current_mode = MODE_INPUT_CAL_LOW;
//			break;
//		case MODE_INPUT_CAL_LOW:
//			current_mode = MODE_INPUT_CAL_HIGH;
//			break;
//		case MODE_INPUT_CAL_HIGH:
//			current_mode = MODE_INPUT_CAL_DONE;
//			break;
//		case MODE_INPUT_CAL_DONE:
//			current_mode = MODE_NORMAL;
//			break;
//
//		case MODE_OUTPUT_CAL:
//			current_mode = MODE_OUTPUT_CAL_LOW;
//			break;
//		case MODE_OUTPUT_CAL_LOW:
//			current_mode = MODE_OUTPUT_CAL_HIGH;
//			break;
//		case MODE_OUTPUT_CAL_HIGH:
//			current_mode = MODE_OUTPUT_CAL_DONE;
//			break;
//		case MODE_OUTPUT_CAL_DONE:
//			current_mode = MODE_NORMAL;
//			break;
//
//		case MODE_STORE_CAL:
//			current_mode = MODE_NORMAL;
//			break;
//		case MODE_RESTORE_CAL:
//			current_mode = MODE_NORMAL;
//			break;
//
//		default:
//			break;
//		}
//
//		Set_LED_State_For_Mode(current_mode);
//	}
//	else {
//		mode_changed = false;
//	}
//}
//
//static void Set_LED_State_For_Mode(OperationMode_e mode)
//{
//	// Reset all LED states
//	led_state.op_on = true;
//	led_state.hi_on = true;
//	led_state.lo_on = true;
//	led_state.inp_on = true;
//	led_state.led_1_on = true;
//	led_state.led_2_on = true;
//	led_state.op_flash = false;
//	led_state.hi_flash = false;
//	led_state.lo_flash = false;
//	led_state.inp_flash = false;
//	led_state.led_1_flash = false;
//	led_state.led_2_flash = false;
//
//	switch (mode) {
//	case MODE_NORMAL:
//		// All LEDs OFF
//		break;
//
//	case MODE_INPUT_CAL:
//		// INP ON, others OFF
//		led_state.inp_on = false;
//		break;
//
//	case MODE_INPUT_CAL_LOW:
//		// INP ON, LO FLASH, others OFF
//		led_state.inp_on = false;
//		led_state.lo_flash = true;
//		break;
//
//	case MODE_INPUT_CAL_HIGH:
//		// INP ON, LO ON, HI FLASH, OP OFF
//		led_state.inp_on = false;
//		led_state.lo_on = false;
//		led_state.hi_flash = true;
//		break;
//
//	case MODE_INPUT_CAL_DONE:
//		// INP ON, LO ON, HI ON, OP OFF
//		led_state.inp_on = false;
//		led_state.lo_on = false;
//		led_state.hi_on = false;
//		break;
//
//	case MODE_OUTPUT_CAL:
//		// OP ON, others OFF
//		led_state.op_on = false;
//		break;
//
//	case MODE_OUTPUT_CAL_LOW:
//		// OP ON, LO FLASH, others OFF
//		led_state.op_on = false;
//		led_state.lo_flash = true;
//		break;
//
//	case MODE_OUTPUT_CAL_HIGH:
//		// OP ON, LO ON, HI FLASH, INP OFF
//		led_state.op_on = false;
//		led_state.lo_on = false;
//		led_state.hi_flash = true;
//		break;
//
//	case MODE_OUTPUT_CAL_DONE:
//		// OP ON, LO ON, HI ON, INP OFF
//		led_state.op_on = false;
//		led_state.lo_on = false;
//		led_state.hi_on = false;
//		break;
//
//	case MODE_STORE_CAL:
//		// HI FLASH, LO FLASH, others OFF
//		led_state.hi_flash = true;
//		led_state.lo_flash = true;
//		break;
//	case MODE_STORE_CAL_DONE:
//		// HI FLASH, LO FLASH, others OFF
//		led_state.hi_on = false;
//		led_state.lo_on = false;
//		break;
//
//	case MODE_RESTORE_CAL:
//		// OP FLASH, INP FLASH, others OFF
//		led_state.op_flash = true;
//		led_state.inp_flash = true;
//		break;
//	case MODE_RESTORE_CAL_DONE:
//		// OP FLASH, INP FLASH, others OFF
//		led_state.op_on = false;
//		led_state.inp_on = false;
//		break;
//
//	case MODE_SENSOR_BROKE:
//		// LD1 (assuming INP) FLASH
//		led_state.led_1_flash = true;
//		break;
//
//	case MODE_COMM_FAILED:
//		// LD1 ON, LD2 ON (assuming INP and HI)
//		led_state.led_1_on = false;
//		led_state.led_2_on = false;
//		break;
//
//	case MODE_MEMORY_CORRUPT:
//		// LD1 FLASH, LD2 FLASH (assuming INP and HI)
//		led_state.led_1_flash = true;
//		led_state.led_2_flash = true;
//		break;
//	}
//}
//
//static void LED_Control(void)
//{
//	static uint32_t last_flash_time = 0;
//	static bool flash_state = false;
//	uint32_t current_time = HAL_GetTick();
//
//	// Update flash state
//	if (current_time - last_flash_time >= LED_FLASH_PERIOD) {
//		flash_state = !flash_state;
//		last_flash_time = current_time;
//	}
//
//	// OP LED
//	if (led_state.op_flash) {
//		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, led_state.op_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//	// HI LED
//	if (led_state.hi_flash) {
//		HAL_GPIO_WritePin(HI_INP_GPIO_Port, HI_INP_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(HI_INP_GPIO_Port, HI_INP_Pin, led_state.hi_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//	// LO LED
//	if (led_state.lo_flash) {
//		HAL_GPIO_WritePin(LO_INP_GPIO_Port, LO_INP_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(LO_INP_GPIO_Port, LO_INP_Pin, led_state.lo_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//	// INP LED
//	if (led_state.inp_flash) {
//		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, led_state.inp_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//	// LED_1
//	if (led_state.led_1_flash) {
//		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, led_state.led_1_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//	// LED_2
//	if (led_state.led_2_flash) {
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	} else {
//		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, led_state.led_2_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	}
//
//
//}
//
//void Keyboard_LED_Update(void)
//{
//	LED_Control();
//}
//
//OperationMode_e Keyboard_GetMode(void)
//{
//	return current_mode;
//}
//
//void Keyboard_SetMode(OperationMode_e mode)
//{
//	current_mode = mode;
//	Set_LED_State_For_Mode(mode);
//}
//
//uint8_t Keyboard_GetKeyPressed(void)
//{
//	// Return current single key press (for UP/DOWN in calibration)
//	if (key_state == KEY_UP || key_state == KEY_DOWN) {
//		return key_state;
//	}
//	return 0;
//}



/*
 * Keyboard.c
 *
 *  Created on: Jan 5, 2026
 *      Author: visak
 */

#include "keyboard.h"
#include "Fram.h"
#include "ADC.h"

// Timing constants
#define DEBOUNCE_TIME       100     // ms (matches reference debounce delay)
#define LONG_KEY_COUNT      50      // For 5 second press (50 * 100ms)
#define LONG_KEY_COUNT_10S  100     // For 10 second press (100 * 100ms)
#define LED_FLASH_PERIOD    500     // ms

// CAL_Flag definitions (reference pattern)
#define NORMAL_MODE  0
#define CAL_MODE     1

// NO_KEY definition
#define NO_KEY       0x00

// Private variables
OperationMode_e current_mode = MODE_NORMAL;
static bool power_on_flag = true;
static LED_State_t led_state;
extern FRAM_Config_t device_config;//Visakh
extern ADC_Handle_t ADCHandler;//Visakh on 23/01/2026
// Public CAL flag (reference pattern)
uint8_t CAL_Flag = NORMAL_MODE;

// Keyboard structure (reference pattern)
Keyboard_t Keyboard = {0};

// Private functions
uint8_t Read_Keys(void);
static void Set_LED_State_For_Mode(OperationMode_e mode);
static void LED_Control(void);

// Key combination functions (reference pattern)
static void SetDownKeyPressed(void);      // K1+K3 for 5 sec = Input Cal
static void SetUpKeyPressed(void);        // K1+K2 for 5 sec = Output Cal
static void UpEnterKeyPressed(void);      // K2+K4 for 10 sec = Restore Cal
static void DownEnterKeyPressed(void);    // K3+K4 at power-on = Store Cal

// Public key processing functions
static void UPKeyPressed(void);
static void DownKeyPressed(void);
static void EnterKeyPressed(void);
static void DownEnterPWMTestKeyPressed(void);  // K3+K4 for 5 sec = PWM Test
void Keyboard_Init(void)
{
	// Keys are already configured in CubeMX as GPIO inputs with pull-up
	// LEDs are already configured in CubeMX as GPIO outputs

	current_mode = MODE_NORMAL;
	power_on_flag = true;
	CAL_Flag = NORMAL_MODE;

	// Initialize Keyboard structure
	Keyboard.Fast_Value_Change = 0;
	Keyboard.New_Val = 0;
	Keyboard.Key_Stop_Counter = 0;
	Keyboard.KeyPressed = NO_KEY;

	// Initialize LED state
	led_state.op_on = true;
	led_state.hi_on = true;
	led_state.lo_on = true;
	led_state.inp_on = true;
	led_state.led_1_on = true;
	led_state.led_2_on = true;
	led_state.op_flash = false;
	led_state.hi_flash = false;
	led_state.lo_flash = false;
	led_state.inp_flash = false;
	led_state.led_1_flash = false;
	led_state.led_2_flash = false;
}

uint8_t Read_Keys(void)
{
	uint8_t keys = 0;

	// Read all keys (active low with pull-up)
	if (HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET)
		keys |= KEY_SET;
	if (HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET)
		keys |= KEY_UP;
	if (HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET)
		keys |= KEY_DOWN;
	if (HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin) == GPIO_PIN_RESET)
		keys |= KEY_ENTER;

	return keys;
}

// Reference pattern: Read key with debounce
uint8_t Read_key(void)
{
	uint8_t read_port;

	read_port = Read_Keys();

	// If key detected, apply debounce (reference pattern)
	if (read_port != NO_KEY) {
		HAL_Delay(100);  // Debounce delay (from reference)
		read_port = Read_Keys();
	}

	return read_port;
}

void Keyboard_Scan(void)
{
	// Simple scan - just checks for power-on combination
	// Long-press detection happens in Keycheck_Normal via combination functions

	// Check for power-on key combinations (K3+K4 for Store Cal)
	if (power_on_flag && (Keyboard.KeyPressed == (KEY_DOWN | KEY_ENTER))) {
		DownEnterKeyPressed();
		power_on_flag = false;
		return;
	}
	power_on_flag = false;
}

// Reference pattern: K1+K3 for 5 seconds = Input Calibration
static void SetDownKeyPressed(void)
{
	//	// Wait for 5 seconds while keys held
	//	Keyboard.Key_Stop_Counter = 0;
	//
	//	while ((Keyboard.KeyPressed == (KEY_SET | KEY_DOWN)) &&
	//	       (Keyboard.Key_Stop_Counter < LONG_KEY_COUNT)) {
	//		Keyboard.KeyPressed = Read_key();
	//		Keyboard.Key_Stop_Counter++;
	////		HAL_Delay(100);  // 100ms delay
	//	}
	//
	//	// Check if held for full duration
	//	if (Keyboard.Key_Stop_Counter >= LONG_KEY_COUNT) {
	//		current_mode = MODE_INPUT_CAL;
	//		CAL_Flag = CAL_MODE;
	//		Set_LED_State_For_Mode(current_mode);
	//	}
	//
	//	Keyboard.KeyPressed = NO_KEY;

	uint32_t start_time = HAL_GetTick();

	// Wait 5 seconds while checking if keys still held
	while ((HAL_GetTick() - start_time) < 5000) {
		// Direct GPIO read (no delay!)
		if (Read_Keys() != (KEY_SET | KEY_DOWN)) {
			Keyboard.KeyPressed = NO_KEY;
			return;  // Keys released early
		}
		HAL_Delay(100);  // Only one delay per loop
	}

	// Keys held for full 5 seconds
	current_mode = MODE_INPUT_CAL;
	CAL_Flag = CAL_MODE;
	Set_LED_State_For_Mode(current_mode);
	Keyboard.KeyPressed = NO_KEY;

}

// Reference pattern: K1+K2 for 5 seconds = Output Calibration
static void SetUpKeyPressed(void)
{
	//	// Wait for 5 seconds while keys held
	//	Keyboard.Key_Stop_Counter = 0;
	//
	//	while ((Keyboard.KeyPressed == (KEY_SET | KEY_UP)) &&
	//	       (Keyboard.Key_Stop_Counter < LONG_KEY_COUNT)) {
	//		Keyboard.KeyPressed = Read_key();
	//		Keyboard.Key_Stop_Counter++;
	////		HAL_Delay(100);  // 100ms delay
	//	}
	//
	//	// Check if held for full duration
	//	if (Keyboard.Key_Stop_Counter >= LONG_KEY_COUNT) {
	//		current_mode = MODE_OUTPUT_CAL;
	//		CAL_Flag = CAL_MODE;
	//		Set_LED_State_For_Mode(current_mode);
	//	}
	//
	//	Keyboard.KeyPressed = NO_KEY;

	uint32_t start_time = HAL_GetTick();

	while ((HAL_GetTick() - start_time) < 5000) {
		if (Read_Keys() != (KEY_SET | KEY_UP)) {
			Keyboard.KeyPressed = NO_KEY;
			return;
		}
		HAL_Delay(100);
	}

	current_mode = MODE_OUTPUT_CAL;
	CAL_Flag = CAL_MODE;
	Set_LED_State_For_Mode(current_mode);
	Keyboard.KeyPressed = NO_KEY;

}

// Reference pattern: K2+K4 for 10 seconds = Restore Calibration
static void UpEnterKeyPressed(void)
{
	//	// Wait for 10 seconds while keys held
	//	Keyboard.Key_Stop_Counter = 0;
	//
	//	while ((Keyboard.KeyPressed == (KEY_UP | KEY_ENTER)) &&
	//	       (Keyboard.Key_Stop_Counter < LONG_KEY_COUNT_10S)) {
	//		Keyboard.KeyPressed = Read_key();
	//		Keyboard.Key_Stop_Counter++;
	//		HAL_Delay(100);  // 100ms delay
	//	}
	//
	//	// Check if held for full duration
	//	if (Keyboard.Key_Stop_Counter >= LONG_KEY_COUNT_10S) {
	//		current_mode = MODE_RESTORE_CAL;
	//		CAL_Flag = NORMAL_MODE;
	//		Set_LED_State_For_Mode(current_mode);
	//	}
	//
	//	Keyboard.KeyPressed = NO_KEY;

	uint32_t start_time = HAL_GetTick();

	while ((HAL_GetTick() - start_time) < 10000) {  // 10 seconds
		if (Read_Keys() != (KEY_UP | KEY_ENTER)) {
			Keyboard.KeyPressed = NO_KEY;
			return;
		}
		HAL_Delay(100);
	}

	current_mode = MODE_RESTORE_CAL;
	CAL_Flag = NORMAL_MODE;
	Set_LED_State_For_Mode(current_mode);
	Keyboard.KeyPressed = NO_KEY;


}

// Reference pattern: K3+K4 at power-on = Store Calibration
static void DownEnterKeyPressed(void)
{
	current_mode = MODE_STORE_CAL;
	CAL_Flag = NORMAL_MODE;
	Set_LED_State_For_Mode(current_mode);
	Keyboard.KeyPressed = NO_KEY;
}

// Reference pattern: Normal mode key processing (PUBLIC - called from main loop)
void Keycheck_Normal(void)
{
	switch (Keyboard.KeyPressed) {
	case KEY_UP:
		UPKeyPressed();
		Keyboard.KeyPressed = NO_KEY;
		break;

	case KEY_DOWN:
		DownKeyPressed();
		Keyboard.KeyPressed = NO_KEY;
		break;

	case KEY_ENTER:
		EnterKeyPressed();
		Keyboard.KeyPressed = NO_KEY;
		break;

		// Key combinations for mode entry
	case (KEY_SET | KEY_DOWN):   // K1+K3 = Input Cal (5 sec)
				SetDownKeyPressed();
	break;

	case (KEY_SET | KEY_UP):     // K1+K2 = Output Cal (5 sec)
				SetUpKeyPressed();
	break;

	case (KEY_UP | KEY_ENTER):   // K2+K4 = Restore Cal (10 sec)
				UpEnterKeyPressed();
	break;

	// In the switch statement, add:
	case (KEY_DOWN | KEY_ENTER):  // K3+K4 = PWM Test (5 sec)
				  DownEnterPWMTestKeyPressed();
	break;

	default:
		break;
	}
}

// Reference pattern: Calibration mode key processing (PUBLIC - called from main loop)
void Keycheck_Calibration(void)
{
	switch (Keyboard.KeyPressed) {
	case KEY_UP:
		UPKeyPressed();
		break;

	case KEY_DOWN:
		DownKeyPressed();
		break;

	case KEY_ENTER:
		EnterKeyPressed();
		Keyboard.KeyPressed = NO_KEY;
		break;

	default:
		break;
	}
}

//************Visakh on 13/02/2026

static void DownEnterPWMTestKeyPressed(void)
{
	//    if (power_on_flag) {
	//        return;  // Prevent conflict with Store Cal at power-on
	//    }
	//
	//    Keyboard.Key_Stop_Counter = 0;
	//    while ((Keyboard.KeyPressed == (KEY_DOWN | KEY_ENTER)) &&
	//           (Keyboard.Key_Stop_Counter < LONG_KEY_COUNT)) {
	//        Keyboard.KeyPressed = Read_key();
	//        Keyboard.Key_Stop_Counter++;
	//        HAL_Delay(100);
	//    }
	//
	//    if (Keyboard.Key_Stop_Counter >= LONG_KEY_COUNT) {
	//        current_mode = MODE_PWM_TEST;
	//        CAL_Flag = CAL_MODE;
	//        Set_LED_State_For_Mode(current_mode);
	//    }
	//    Keyboard.KeyPressed = NO_KEY;

	if (power_on_flag) {
		return;
	}

	uint32_t start_time = HAL_GetTick();

	while ((HAL_GetTick() - start_time) < 5000) {
		if (Read_Keys() != (KEY_DOWN | KEY_ENTER)) {
			Keyboard.KeyPressed = NO_KEY;
			return;
		}
		HAL_Delay(100);
	}

	current_mode = MODE_PWM_TEST;
	CAL_Flag = CAL_MODE;
	Set_LED_State_For_Mode(current_mode);
	Keyboard.KeyPressed = NO_KEY;

}

//************Visakh on 13/02/2026

static void UPKeyPressed(void)
{
	// Implement UP key logic based on current mode
	// This is called from your main loop switch statement
}

static void DownKeyPressed(void)
{
	// Implement DOWN key logic based on current mode
	// This is called from your main loop switch statement
}

static void EnterKeyPressed(void)
{
	while (Read_Keys() & KEY_ENTER) {
		HAL_Delay(10);
	}
	HAL_Delay(100);
	// Handle ENTER key for mode transitions
	switch (current_mode) {
	case MODE_INPUT_CAL:
		current_mode = MODE_INPUT_CAL_LOW;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_INPUT_CAL_LOW:
		current_mode = MODE_INPUT_CAL_HIGH;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_INPUT_CAL_HIGH:
		//		device_config.Inp_Cal_code1 = ADCHandler.adc_raw;
		current_mode = MODE_INPUT_CAL_DONE;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_INPUT_CAL_DONE:
		//		l.Inp_Cal_code2 = ADCHandler.adc_raw;
		current_mode = MODE_NORMAL;
		CAL_Flag = NORMAL_MODE;
		break;

	case MODE_OUTPUT_CAL:
		current_mode = MODE_OUTPUT_CAL_LOW;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_OUTPUT_CAL_LOW:
		current_mode = MODE_OUTPUT_CAL_HIGH;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_OUTPUT_CAL_HIGH:
		current_mode = MODE_OUTPUT_CAL_DONE;
		CAL_Flag = CAL_MODE;
		break;
	case MODE_OUTPUT_CAL_DONE:
		current_mode = MODE_NORMAL;
		CAL_Flag = NORMAL_MODE;
		break;

	case MODE_STORE_CAL:
		current_mode = MODE_NORMAL;
		CAL_Flag = NORMAL_MODE;
		break;
	case MODE_RESTORE_CAL:
		current_mode = MODE_NORMAL;
		CAL_Flag = NORMAL_MODE;
		break;
	case MODE_PWM_TEST:
		current_mode = MODE_NORMAL;
		CAL_Flag = NORMAL_MODE;
		break;

	default:
		break;
	}

	Set_LED_State_For_Mode(current_mode);

	//   while (Read_Keys() & KEY_ENTER) {
	//      HAL_Delay(10);
	//    }
	//    HAL_Delay(50);
}

static void Set_LED_State_For_Mode(OperationMode_e mode)
{
	// Reset all LED states
	led_state.op_on = true;
	led_state.hi_on = true;
	led_state.lo_on = true;
	led_state.inp_on = true;
	led_state.led_1_on = true;
	led_state.led_2_on = true;
	led_state.op_flash = false;
	led_state.hi_flash = false;
	led_state.lo_flash = false;
	led_state.inp_flash = false;
	led_state.led_1_flash = false;
	led_state.led_2_flash = false;

	switch (mode) {
	case MODE_NORMAL:
		// All LEDs OFF
		break;

	case MODE_INPUT_CAL:
		// INP ON, others OFF
		led_state.inp_on = false;
		break;

	case MODE_INPUT_CAL_LOW:
		// INP ON, LO FLASH, others OFF
		led_state.inp_on = false;
		led_state.lo_flash = true;
		break;

	case MODE_INPUT_CAL_HIGH:
		// INP ON, LO ON, HI FLASH, OP OFF
		led_state.inp_on = false;
		led_state.lo_on = false;
		led_state.hi_flash = true;
		break;

	case MODE_INPUT_CAL_DONE:
		// INP ON, LO ON, HI ON, OP OFF
		led_state.inp_on = false;
		led_state.lo_on = false;
		led_state.hi_on = false;
		break;

	case MODE_OUTPUT_CAL:
		// OP ON, others OFF
		led_state.op_on = false;
		break;

	case MODE_OUTPUT_CAL_LOW:
		// OP ON, LO FLASH, others OFF
		led_state.op_on = false;
		led_state.lo_flash = true;
		break;

	case MODE_OUTPUT_CAL_HIGH:
		// OP ON, LO ON, HI FLASH, INP OFF
		led_state.op_on = false;
		led_state.lo_on = false;
		led_state.hi_flash = true;
		break;

	case MODE_OUTPUT_CAL_DONE:
		// OP ON, LO ON, HI ON, INP OFF
		led_state.op_on = false;
		led_state.lo_on = false;
		led_state.hi_on = false;
		break;

	case MODE_STORE_CAL:
		// HI FLASH, LO FLASH, others OFF
		led_state.hi_flash = true;
		led_state.lo_flash = true;
		break;
	case MODE_STORE_CAL_DONE:
		// HI ON, LO ON, others OFF
		led_state.hi_on = false;
		led_state.lo_on = false;
		break;

	case MODE_RESTORE_CAL:
		// OP FLASH, INP FLASH, others OFF
		led_state.op_flash = true;
		led_state.inp_flash = true;
		break;
	case MODE_RESTORE_CAL_DONE:
		// OP ON, INP ON, others OFF
		led_state.op_on = false;
		led_state.inp_on = false;
		break;

	case MODE_SENSOR_BROKE:
		// LD1 FLASH
		led_state.led_1_flash = true;
		break;

	case MODE_COMM_FAILED:
		// LD1 ON, LD2 ON
		led_state.led_1_on = false;
		led_state.led_2_on = false;
		break;

	case MODE_MEMORY_CORRUPT:
		// LD1 FLASH, LD2 FLASH
		led_state.led_1_flash = true;
		led_state.led_2_flash = true;
		break;
		//*********Visakh on12/02/2026
	case MODE_PWM_TEST:
		// All LEDs flash to indicate test mode
		led_state.op_on = false;
		led_state.hi_on = false;
		led_state.lo_on = false;
		led_state.inp_on = false;
		led_state.op_flash = true;
		led_state.hi_flash = true;
		led_state.lo_flash = true;
		led_state.inp_flash = true;
		break;
	}
}

static void LED_Control(void)
{
	static uint32_t last_flash_time = 0;
	static bool flash_state = false;
	uint32_t current_time = HAL_GetTick();

	// Update flash state
	if (current_time - last_flash_time >= LED_FLASH_PERIOD) {
		flash_state = !flash_state;
		last_flash_time = current_time;
	}

	// OP LED
	if (led_state.op_flash) {
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, led_state.op_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	// HI LED
	if (led_state.hi_flash) {
		HAL_GPIO_WritePin(HI_INP_GPIO_Port, HI_INP_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(HI_INP_GPIO_Port, HI_INP_Pin, led_state.hi_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	// LO LED
	if (led_state.lo_flash) {
		HAL_GPIO_WritePin(LO_INP_GPIO_Port, LO_INP_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(LO_INP_GPIO_Port, LO_INP_Pin, led_state.lo_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	// INP LED
	if (led_state.inp_flash) {
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, led_state.inp_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	// LED_1
	if (led_state.led_1_flash) {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, led_state.led_1_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	// LED_2
	if (led_state.led_2_flash) {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, led_state.led_2_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

void Keyboard_LED_Update(void)
{
	LED_Control();
}

OperationMode_e Keyboard_GetMode(void)
{
	return current_mode;
}

void Keyboard_SetMode(OperationMode_e mode)
{
	current_mode = mode;
	Set_LED_State_For_Mode(mode);

	// Automatically set CAL_Flag based on mode (reference pattern)
	if ((mode >= MODE_INPUT_CAL && mode <= MODE_INPUT_CAL_DONE) ||
			(mode >= MODE_OUTPUT_CAL && mode <= MODE_OUTPUT_CAL_DONE)) {
		CAL_Flag = CAL_MODE;
	} else {
		CAL_Flag = NORMAL_MODE;
	}
}

uint8_t Keyboard_GetKeyPressed(void)
{
	// Return current single key press (for UP/DOWN in calibration)
	// This maintains compatibility with your main loop
	if (Keyboard.KeyPressed == KEY_UP || Keyboard.KeyPressed == KEY_DOWN) {
		return Keyboard.KeyPressed;
	}
	return 0;
}

// Reference pattern: Random delay function
void random_delay(char delay)
{
	HAL_Delay(delay);
}

// Reference pattern: Fast value increment
void Fast_value_Inc(void)
{
	if (Keyboard.Fast_Value_Change) {
		Keyboard.New_Val++;
		if (Keyboard.New_Val > 10) {
			Keyboard.New_Val = 10;  // Cap at maximum acceleration
		}
	}
}

// Reference pattern: Fast value decrement
void Fast_value_Dec(void)
{
	if (Keyboard.Fast_Value_Change) {
		Keyboard.New_Val++;
		if (Keyboard.New_Val > 10) {
			Keyboard.New_Val = 10;  // Cap at maximum acceleration
		}
	}
}
