///* USER CODE BEGIN Header */
///**
// ******************************************************************************
// * @file           : main.c
// * @brief          : Main program body
// ******************************************************************************
// * @attention
// *
// * Copyright (c) 2026 STMicroelectronics.
// * All rights reserved.
// *
// * This software is licensed under terms that can be found in the LICENSE file
// * in the root directory of this software component.
// * If no LICENSE file comes with this software, it is provided AS-IS.
// *
// ******************************************************************************
// */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "keyboard.h"
//#include "Fram.h"
//#include "ADC.h"
//#include "modbus.h"
//#include "Output.h"
//
///* Private includes ----------------------------------------------------------*/
///* USER CODE BEGIN Includes */
//#define NO_KEY 0x00
///* USER CODE END Includes */
//
///* Private typedef -----------------------------------------------------------*/
///* USER CODE BEGIN PTD */
//
///* USER CODE END PTD */
//
///* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN PD */
//
///* USER CODE END PD */
//
///* Private macro -------------------------------------------------------------*/
///* USER CODE BEGIN PM */
//
///* USER CODE END PM */
//
///* Private variables ---------------------------------------------------------*/
//ADC_HandleTypeDef hadc;
//DMA_HandleTypeDef hdma_adc;
//
//CRC_HandleTypeDef hcrc;
//
//I2C_HandleTypeDef hi2c1;
//
//TIM_HandleTypeDef htim3;
//TIM_HandleTypeDef htim14;
//
//UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_usart1_tx;
//DMA_HandleTypeDef hdma_usart1_rx;
//
///* USER CODE BEGIN PV */
////extern I2C_HandleTypeDef hi2c1;  // From CubeMX
//FRAM_Config_t device_config;
//extern ADC_Handle_t ADCHandler;
////extern
///* USER CODE END PV */
//
///* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_DMA_Init(void);
//static void MX_ADC_Init(void);
//static void MX_CRC_Init(void);
//static void MX_I2C1_Init(void);
//static void MX_TIM3_Init(void);
//static void MX_TIM14_Init(void);
//static void MX_USART1_UART_Init(void);
//
//extern void ADC_Save_Calibration_To_Config(FRAM_Config_t *config);
//extern void ADC_Restore_Calibration_From_Config(FRAM_Config_t *config);
///* USER CODE BEGIN PFP */
//
///* USER CODE END PFP */
//
///* Private user code ---------------------------------------------------------*/
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//
///**
// * @brief  The application entry point.
// * @retval int
// */
//int main(void)
//{
//
//	/* USER CODE BEGIN 1 */
//
//	/* USER CODE END 1 */
//
//	/* MCU Configuration--------------------------------------------------------*/
//
//	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//	HAL_Init();
//
//	/* USER CODE BEGIN Init */
//
//	/* USER CODE END Init */
//
//	/* Configure the system clock */
//	SystemClock_Config();
//
//	/* USER CODE BEGIN SysInit */
//
//	/* USER CODE END SysInit */
//
//	/* Initialize all configured peripherals */
//	MX_GPIO_Init();
//	MX_DMA_Init();
//	MX_ADC_Init();
//	MX_CRC_Init();
//	MX_I2C1_Init();
//	MX_TIM3_Init();
//	MX_TIM14_Init();
//	MX_USART1_UART_Init();
//	/* USER CODE BEGIN 2 */
//	//	void PowerON_Initilization(void);
//	Keyboard_Init();
//	//	FRAM_Init(&hi2c1);
//	//	ADC_Init(&hadc);
//	// Try to initialize FRAM hardware
//	bool fram_ok = FRAM_Init(&hi2c1);
//
//	if (fram_ok) {
//		// FRAM hardware OK - try to read saved config
//		if (FRAM_Read_Config(&device_config)) {
//			// Config loaded from FRAM successfully
//			ADC_Restore_Calibration_From_Config(&device_config);
//			AnalogOutput_Set_Calibration(device_config.cal_output_lo,
//					device_config.cal_output_hi);
//			Keyboard_SetMode(MODE_NORMAL);
//		} else {
//			// FRAM read failed - config is corrupted or empty
//			// DON'T auto-restore - just set error mode
//			Keyboard_SetMode(MODE_MEMORY_CORRUPT);
//			// Device will use whatever values are in device_config structure
//		}
//	} else {
//		// FRAM hardware not responding
//		Keyboard_SetMode(MODE_MEMORY_CORRUPT);
//	}
//
//
//
//	//	HAL_StatusTypeDef ret = FRAM_Init(&hi2c1);
//	ADC_Init(&hadc);
//	Modbus_Init(&huart1);
//	AnalogOutput_Init(&htim3);
//
//	// Configure ADC from FRAM
//	ADC_Set_Sensor_Type(device_config.input_type);
//	ADC_Set_Range(device_config.input_low, device_config.input_high);
//	ADC_Set_Offset(device_config.offset_input);
//	//	Modbus_Set_Slave_ID(device_config.slave_id);
//	Modbus_Set_Slave_ID(1);
//	Modbus_Enable(1);
//	//	Modbus_Enable(device_config.comm_enable);
//
//
//	AnalogOutput_Set_Range(device_config.output_low, device_config.output_high);
//	AnalogOutput_Set_Direction(device_config.output_direction);
//	AnalogOutput_Set_Offset(device_config.offset_output);
//	AnalogOutput_Set_Decimal(device_config.output_decimal);
//	/* USER CODE END 2 */
//
//	/* Infinite loop */
//	/* USER CODE BEGIN WHILE */
//	while (1)
//	{
//		/* USER CODE END WHILE */
//
//		/* USER CODE BEGIN 3 */
//
//		ADC_Process();  // Process ADC readings
//		Modbus_Process();
//
//		// ===== REFERENCE PATTERN START =====
//		Keyboard.KeyPressed = Read_Keys();
//
//		if (Keyboard.KeyPressed == NO_KEY) {
//			// Reset fast value change counter when no key pressed
//			Keyboard.New_Val = 0;
//		}
//		else {
//			// Key is pressed
//
//			// Check for power-on combination
//			Keyboard_Scan();  // Handles K3+K4 at power-on
//
//			// Process based on calibration mode
//			if (CAL_Flag == NORMAL_MODE) {
//				Keycheck_Normal();       // Process normal mode keys (includes long-press combos)
//			}
//			else if (CAL_Flag == CAL_MODE) {
//				Keycheck_Calibration();  // Process calibration mode keys
//			}
//
//			// Wait for key release (except for UP/DOWN which can repeat)
////			if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
////				while (Read_key() != NO_KEY);
////			}
//
////			if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
////			    Keyboard.KeyPressed = NO_KEY;  // ✓ Just clear it, don't block
////			}//05/03/2026
//
//
//			/*if (Keyboard.KeyPressed == KEY_ENTER) {
//				while (Read_Keys() & KEY_ENTER) {
//					HAL_Delay(10);
//				}
//				HAL_Delay(50);  // debounce after release
//				Keyboard.KeyPressed = NO_KEY;
//			}
//			else*/ if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
//			    Keyboard.KeyPressed = NO_KEY;
//			}
//		}
//
//		Keyboard_LED_Update();  // Update LED patterns
//		// ===== REFERENCE PATTERN END =====
//
//		// FRAM write handling
//		if (ModbusHandler.pending_fram_write) {
//			HAL_Delay(50);
//			bool write_ok = FRAM_Write_Config(&device_config);
//			ModbusHandler.pending_fram_write = false;
//			if (write_ok) {
//				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//			}
//		}
//
//		// Check for sensor fault
//		bool sensor_fault = ADC_Is_Sensor_Fault();
//		AnalogOutput_Set_Fault(sensor_fault);
//
////		if (sensor_fault) {
////			Keyboard_SetMode(MODE_SENSOR_BROKE);
////		}
//
//		// Get current mode
//		    OperationMode_e current_mode = Keyboard_GetMode();
//
//		    if (sensor_fault) {
//		        // Fault detected - go to fault mode (but preserve calibration modes!)
////		        if (current_mode != MODE_INPUT_CAL_LOW &&
////		            current_mode != MODE_INPUT_CAL_HIGH &&
////		            current_mode != MODE_OUTPUT_CAL_LOW &&
////		            current_mode != MODE_OUTPUT_CAL_HIGH)
//		    	if (current_mode != MODE_INPUT_CAL      &&
//		    	        current_mode != MODE_INPUT_CAL_LOW  &&
//		    	        current_mode != MODE_INPUT_CAL_HIGH &&
//		    	        current_mode != MODE_INPUT_CAL_DONE &&
//		    	        current_mode != MODE_OUTPUT_CAL     &&
//		    	        current_mode != MODE_OUTPUT_CAL_LOW &&
//		    	        current_mode != MODE_OUTPUT_CAL_HIGH&&
//		    	        current_mode != MODE_OUTPUT_CAL_DONE){
//		            Keyboard_SetMode(MODE_SENSOR_BROKE);
//		        }
//		    } else {
//		        // Fault cleared - AUTO-RECOVER to normal mode!
//		        if (current_mode == MODE_SENSOR_BROKE) {
//		            Keyboard_SetMode(MODE_NORMAL);  // ← AUTO-RECOVERY!
//		        }
//		    }
//
//
//
//		// Update analog output
//		float input_value = ADC_Get_Float_Value();
//		AnalogOutput_Update(input_value,
//				(float)device_config.input_low,
//				(float)device_config.input_high);
//
//		// Handle calibration value changes
//		OperationMode_e mode = Keyboard_GetMode();
//		uint8_t key = Keyboard.KeyPressed;  // From structure now
//
//
//
//		switch (mode) {
//		case MODE_INPUT_CAL_LOW:
//			if (key == KEY_UP) {
//			        if (device_config.cal_input_lo < 65535) device_config.cal_input_lo++;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Inc();
//			            if (device_config.cal_input_lo > (65535 - Keyboard.New_Val))
//			                device_config.cal_input_lo = 65535;
//			            else
//			                device_config.cal_input_lo += Keyboard.New_Val;
//			        }
//			    }
//			    if (key == KEY_DOWN) {
//			        if (device_config.cal_input_lo > 0) device_config.cal_input_lo--;   // guard underflow
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Dec();
//			            if (device_config.cal_input_lo >= Keyboard.New_Val)
//			                device_config.cal_input_lo -= Keyboard.New_Val;
//			            else
//			                device_config.cal_input_lo = 0;
//			        }
//			    }
//			    device_config.Inp_Cal_code1 = ADCHandler.adc_raw;
//			    break;
//
//		case MODE_INPUT_CAL_HIGH:
//			 if (key == KEY_UP) {
//			        if (device_config.cal_input_hi < 65535) device_config.cal_input_hi++;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Inc();
//			            if (device_config.cal_input_hi > (65535 - Keyboard.New_Val))
//			                device_config.cal_input_hi = 65535;
//			            else
//			                device_config.cal_input_hi += Keyboard.New_Val;
//			        }
//			    }
//			    if (key == KEY_DOWN) {
//			        if (device_config.cal_input_hi > 0) device_config.cal_input_hi--;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Dec();
//			            if (device_config.cal_input_hi >= Keyboard.New_Val)
//			                device_config.cal_input_hi -= Keyboard.New_Val;
//			            else
//			                device_config.cal_input_hi = 0;
//			        }
//			    }
//			    device_config.Inp_Cal_code2 = ADCHandler.adc_raw;
//			    break;
//		case MODE_OUTPUT_CAL_LOW:
//
//			AnalogOutput_Calibration_Bypass(true);
//
//			    if (key == KEY_UP) {
//			        if (device_config.cal_output_lo < 2000) device_config.cal_output_lo++;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Inc();
//			            device_config.cal_output_lo += Keyboard.New_Val;
//			            if (device_config.cal_output_lo > 2000) device_config.cal_output_lo = 2000;
//			        }
//			    }
//			    if (key == KEY_DOWN) {
//			        if (device_config.cal_output_lo > 0) device_config.cal_output_lo--;   // guard underflow
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Dec();
//			            if (device_config.cal_output_lo >= Keyboard.New_Val)
//			                device_config.cal_output_lo -= Keyboard.New_Val;
//			            else
//			                device_config.cal_output_lo = 0;                              // floor, don't wrap
//			        }
//			    }
//
//			    AnalogOutput_Set_Raw_PWM(device_config.cal_output_lo);
//			    break;
//
//		case MODE_OUTPUT_CAL_HIGH:
//
//			AnalogOutput_Calibration_Bypass(true);
//
//			    if (key == KEY_UP) {
//			        if (device_config.cal_output_hi < 2000) device_config.cal_output_hi++;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Inc();
//			            device_config.cal_output_hi += Keyboard.New_Val;
//			            if (device_config.cal_output_hi > 2000) device_config.cal_output_hi = 2000;
//			        }
//			    }
//			    if (key == KEY_DOWN) {
//			        if (device_config.cal_output_hi > 0) device_config.cal_output_hi--;
//			        if (Keyboard.Fast_Value_Change) {
//			            Fast_value_Dec();
//			            if (device_config.cal_output_hi >= Keyboard.New_Val)
//			                device_config.cal_output_hi -= Keyboard.New_Val;
//			            else
//			                device_config.cal_output_hi = 0;
//			        }
//			    }
//
//			    AnalogOutput_Set_Raw_PWM(device_config.cal_output_hi);
//			    break;
//
//		case MODE_INPUT_CAL_DONE:
//		    ADC_Calibrate((float)device_config.cal_input_lo,
//		            (float)device_config.cal_input_hi,
//		            (float)device_config.Inp_Cal_code1,
//		            (float)device_config.Inp_Cal_code2);
//
//		    ADC_Save_Calibration_To_Config(&device_config);
//		    FRAM_Write_Config(&device_config);
//		    break;
//
//		case MODE_OUTPUT_CAL_DONE:
//		    // Disable bypass mode - return to normal temperature scaling
//		    AnalogOutput_Calibration_Bypass(false);
//
//		    // Apply the final calibration values
//		    AnalogOutput_Set_Calibration(device_config.cal_output_lo,
//		                                 device_config.cal_output_hi);
//
//		    // Save to FRAM
//		    FRAM_Write_Config(&device_config);
//		    break;
//
//		case MODE_RESTORE_CAL:
//		    // Restore factory defaults
//		    FRAM_Restore_Defaults();
//
//		    // Read back the defaults
//		    FRAM_Read_Config(&device_config);
//
//		    // Restore ADC calibration
//		    ADC_Restore_Calibration_From_Config(&device_config);
//
//		    // Restore PWM calibration
//		    AnalogOutput_Set_Calibration(device_config.cal_output_lo,
//		                                 device_config.cal_output_hi);
//		    break;
//
//
//		case MODE_PWM_TEST:
//		{
//		    static uint16_t pwm_test_value = 0;   // Start at 0, not 500
//
//		    AnalogOutput_Calibration_Bypass(true);
//
//		    if (key == KEY_UP) {
//		        if (pwm_test_value < 2000) pwm_test_value++;   // Guard before increment
//		        if (Keyboard.Fast_Value_Change) {
//		            Fast_value_Inc();
//		            pwm_test_value += Keyboard.New_Val;
//		            if (pwm_test_value > 2000) pwm_test_value = 2000;
//		        }
//		    }
//		    if (key == KEY_DOWN) {
//		        if (pwm_test_value > 0) pwm_test_value--;      // Guard: uint16_t can't go negative
//		        if (Keyboard.Fast_Value_Change) {
//		            Fast_value_Dec();
//		            if (pwm_test_value >= Keyboard.New_Val)
//		                pwm_test_value -= Keyboard.New_Val;
//		            else
//		                pwm_test_value = 0;
//		        }
//		    }
//
//		    AnalogOutput_Set_Raw_PWM(pwm_test_value);
//
//		    if (key == KEY_ENTER) {
//		        pwm_test_value = 0;                            // Reset for next entry
//		        AnalogOutput_Calibration_Bypass(false);
//		        Keyboard_SetMode(MODE_NORMAL);
//		    }
//		}
//		break;
//
//
//
//
//		default:
//		    break;
//		}
//
//
//
//		HAL_Delay(10);
//	}
//	/* USER CODE END 3 */
//}
//
///**
// * @brief System Clock Configuration
// * @retval None
// */
//void SystemClock_Config(void)
//{
//	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//	/** Initializes the RCC Oscillators according to the specified parameters
//	 * in the RCC_OscInitTypeDef structure.
//	 */
//	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
//	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
//	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Initializes the CPU, AHB and APB buses clocks
//	 */
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//			|RCC_CLOCKTYPE_PCLK1;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//
//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
//	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
//	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
//	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//	{
//		Error_Handler();
//	}
//}
//
///**
// * @brief ADC Initialization Function
// * @param None
// * @retval None
// */
//static void MX_ADC_Init(void)
//{
//
//	/* USER CODE BEGIN ADC_Init 0 */
//
//	/* USER CODE END ADC_Init 0 */
//
//	ADC_ChannelConfTypeDef sConfig = {0};
//
//	/* USER CODE BEGIN ADC_Init 1 */
//
//	/* USER CODE END ADC_Init 1 */
//
//	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//	 */
//	hadc.Instance = ADC1;
//	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//	hadc.Init.Resolution = ADC_RESOLUTION_12B;
//	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
//	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//	hadc.Init.LowPowerAutoWait = DISABLE;
//	hadc.Init.LowPowerAutoPowerOff = DISABLE;
//	hadc.Init.ContinuousConvMode = ENABLE;
//	hadc.Init.DiscontinuousConvMode = DISABLE;
//	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//	hadc.Init.DMAContinuousRequests = ENABLE;
//	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//	if (HAL_ADC_Init(&hadc) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Configure for the selected ADC regular channel to be converted.
//	 */
//	sConfig.Channel = ADC_CHANNEL_0;
//	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
//	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Configure for the selected ADC regular channel to be converted.
//	 */
//	sConfig.Channel = ADC_CHANNEL_1;
//	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN ADC_Init 2 */
//
//	/* USER CODE END ADC_Init 2 */
//
//}
//
///**
// * @brief CRC Initialization Function
// * @param None
// * @retval None
// */
//static void MX_CRC_Init(void)
//{
//
//	/* USER CODE BEGIN CRC_Init 0 */
//
//	/* USER CODE END CRC_Init 0 */
//
//	/* USER CODE BEGIN CRC_Init 1 */
//
//	/* USER CODE END CRC_Init 1 */
//	hcrc.Instance = CRC;
//	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
//	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
//	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
//	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
//	if (HAL_CRC_Init(&hcrc) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN CRC_Init 2 */
//
//	/* USER CODE END CRC_Init 2 */
//
//}
//
///**
// * @brief I2C1 Initialization Function
// * @param None
// * @retval None
// */
//static void MX_I2C1_Init(void)
//{
//
//	/* USER CODE BEGIN I2C1_Init 0 */
//
//	/* USER CODE END I2C1_Init 0 */
//
//	/* USER CODE BEGIN I2C1_Init 1 */
//
//	/* USER CODE END I2C1_Init 1 */
//	hi2c1.Instance = I2C1;
//	hi2c1.Init.Timing = 0x00201D2B;
//	hi2c1.Init.OwnAddress1 = 0;
//	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	hi2c1.Init.OwnAddress2 = 0;
//	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Configure Analogue filter
//	 */
//	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Configure Digital filter
//	 */
//	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN I2C1_Init 2 */
//
//	/* USER CODE END I2C1_Init 2 */
//
//}
//
///**
// * @brief TIM3 Initialization Function
// * @param None
// * @retval None
// */
//static void MX_TIM3_Init(void)
//{
//
//	/* USER CODE BEGIN TIM3_Init 0 */
//
//	/* USER CODE END TIM3_Init 0 */
//
//	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//	TIM_MasterConfigTypeDef sMasterConfig = {0};
//	TIM_OC_InitTypeDef sConfigOC = {0};
//
//	/* USER CODE BEGIN TIM3_Init 1 */
//
//	/* USER CODE END TIM3_Init 1 */
//	htim3.Instance = TIM3;
//	htim3.Init.Prescaler = 11;//23;
//	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htim3.Init.Period = 2000;
//			//1000;
//	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	sConfigOC.OCMode = TIM_OCMODE_PWM1;
//	sConfigOC.Pulse = 0;
//	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN TIM3_Init 2 */
//
//	/* USER CODE END TIM3_Init 2 */
//	HAL_TIM_MspPostInit(&htim3);
//
//}
//
///**
// * @brief TIM14 Initialization Function
// * @param None
// * @retval None
// */
//static void MX_TIM14_Init(void)
//{
//
//	/* USER CODE BEGIN TIM14_Init 0 */
//
//	/* USER CODE END TIM14_Init 0 */
//
//	/* USER CODE BEGIN TIM14_Init 1 */
//
//	/* USER CODE END TIM14_Init 1 */
//	htim14.Instance = TIM14;
//	htim14.Init.Prescaler = 47999;
//	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htim14.Init.Period = 9;
//	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN TIM14_Init 2 */
//
//	/* USER CODE END TIM14_Init 2 */
//
//}
//
///**
// * @brief USART1 Initialization Function
// * @param None
// * @retval None
// */
//static void MX_USART1_UART_Init(void)
//{
//
//	/* USER CODE BEGIN USART1_Init 0 */
//
//	/* USER CODE END USART1_Init 0 */
//
//	/* USER CODE BEGIN USART1_Init 1 */
//
//	/* USER CODE END USART1_Init 1 */
//	huart1.Instance = USART1;
//	huart1.Init.BaudRate = 9600;
//	huart1.Init.WordLength = UART_WORDLENGTH_8B;
//	huart1.Init.StopBits = UART_STOPBITS_1;
//	huart1.Init.Parity = UART_PARITY_NONE;
//	huart1.Init.Mode = UART_MODE_TX_RX;
//	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//	if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 1, 1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN USART1_Init 2 */
//
//	/* USER CODE END USART1_Init 2 */
//
//}
//
///**
// * Enable DMA controller clock
// */
//static void MX_DMA_Init(void)
//{
//
//	/* DMA controller clock enable */
//	__HAL_RCC_DMA1_CLK_ENABLE();
//
//	/* DMA interrupt init */
//	/* DMA1_Channel1_IRQn interrupt configuration */
//	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
//	/* DMA1_Channel2_3_IRQn interrupt configuration */
//	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0);
//	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
//
//}
//
///**
// * @brief GPIO Initialization Function
// * @param None
// * @retval None
// */
//static void MX_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	/* USER CODE BEGIN MX_GPIO_Init_1 */
//
//	/* USER CODE END MX_GPIO_Init_1 */
//
//	/* GPIO Ports Clock Enable */
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOF_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//
//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);
//
//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
//
//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOB, HI_INP_Pin|LO_INP_Pin, GPIO_PIN_RESET);
//
//	/*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
//	GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//	/*Configure GPIO pin : LED4_Pin */
//	GPIO_InitStruct.Pin = LED4_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);
//
//	/*Configure GPIO pin : K1_Pin */
//	GPIO_InitStruct.Pin = K1_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(K1_GPIO_Port, &GPIO_InitStruct);
//
//	/*Configure GPIO pins : K2_Pin K3_Pin K4_Pin */
//	GPIO_InitStruct.Pin = K2_Pin|K3_Pin|K4_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//	/*Configure GPIO pins : HI_INP_Pin LO_INP_Pin */
//	GPIO_InitStruct.Pin = HI_INP_Pin|LO_INP_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//	/* USER CODE BEGIN MX_GPIO_Init_2 */
//	HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, HI_INP_Pin|LO_INP_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
//
//	/* USER CODE END MX_GPIO_Init_2 */
//}
//
///* USER CODE BEGIN 4 */
//
///* USER CODE END 4 */
//
///**
// * @brief  This function is executed in case of error occurrence.
// * @retval None
// */
//void Error_Handler(void)
//{
//	/* USER CODE BEGIN Error_Handler_Debug */
//	/* User can add his own implementation to report the HAL error return state */
//	__disable_irq();
//	while (1)
//	{
//	}
//	/* USER CODE END Error_Handler_Debug */
//}
//
//#ifdef  USE_FULL_ASSERT
///**
// * @brief  Reports the name of the source file and the source line number
// *         where the assert_param error has occurred.
// * @param  file: pointer to the source file name
// * @param  line: assert_param error line source number
// * @retval None
// */
//void assert_failed(uint8_t *file, uint32_t line)
//{
//	/* USER CODE BEGIN 6 */
//	/* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//	/* USER CODE END 6 */
//}
//#endif /* USE_FULL_ASSERT */
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "keyboard.h"
#include "Fram.h"
#include "ADC.h"
#include "modbus.h"
#include "Output.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define NO_KEY 0x00
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
//extern I2C_HandleTypeDef hi2c1;  // From CubeMX
FRAM_Config_t device_config;
extern ADC_Handle_t ADCHandler;
//extern
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_CRC_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM14_Init(void);
static void MX_USART1_UART_Init(void);

extern void ADC_Save_Calibration_To_Config(FRAM_Config_t *config);
extern void ADC_Restore_Calibration_From_Config(FRAM_Config_t *config);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC_Init();
	MX_CRC_Init();
	MX_I2C1_Init();
	MX_TIM3_Init();
	MX_TIM14_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	//	void PowerON_Initilization(void);
	Keyboard_Init();
	//	FRAM_Init(&hi2c1);
	//	ADC_Init(&hadc);
	// Try to initialize FRAM hardware
	bool fram_ok = FRAM_Init(&hi2c1);

	if (fram_ok) {
		// FRAM hardware OK - try to read saved config
		if (FRAM_Read_Config(&device_config)) {
			// Config loaded from FRAM successfully
			ADC_Restore_Calibration_From_Config(&device_config);
			AnalogOutput_Set_Calibration(device_config.cal_output_lo,
					device_config.cal_output_hi);
			Keyboard_SetMode(MODE_NORMAL);
		} else {
			// FRAM read failed - config is corrupted or empty
			// DON'T auto-restore - just set error mode
			Keyboard_SetMode(MODE_MEMORY_CORRUPT);
			// Device will use whatever values are in device_config structure
		}
	} else {
		// FRAM hardware not responding
		Keyboard_SetMode(MODE_MEMORY_CORRUPT);
	}



	//	HAL_StatusTypeDef ret = FRAM_Init(&hi2c1);
	ADC_Init(&hadc);
	Modbus_Init(&huart1);
	AnalogOutput_Init(&htim3);

	// Configure ADC from FRAM
	ADC_Set_Sensor_Type(device_config.input_type);
	ADC_Set_Range(device_config.input_low, device_config.input_high);
	ADC_Set_Offset(device_config.offset_input);
	//	Modbus_Set_Slave_ID(device_config.slave_id);
	Modbus_Set_Slave_ID(1);
	Modbus_Enable(1);
	//	Modbus_Enable(device_config.comm_enable);


	AnalogOutput_Set_Range(device_config.output_low, device_config.output_high);
	AnalogOutput_Set_Direction(device_config.output_direction);
	AnalogOutput_Set_Offset(device_config.offset_output);
	AnalogOutput_Set_Decimal(device_config.output_decimal);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		ADC_Process();  // Process ADC readings
		Modbus_Process();

		// ===== REFERENCE PATTERN START =====
		/* Debounced EVENT, not a raw level.  Fires once per press, then at the
		 * auto-repeat rate for UP/DOWN only.  New_Val is reset inside the engine. */
		Keyboard.KeyPressed = Keys_GetEvent();

		if (Keyboard.KeyPressed != NO_KEY) {
			// Key event to act on

			// Check for power-on combination
			Keyboard_Scan();  // Handles K3+K4 at power-on

			// Process based on calibration mode
			if (CAL_Flag == NORMAL_MODE) {
				Keycheck_Normal();       // Process normal mode keys (includes long-press combos)
			}
			else if (CAL_Flag == CAL_MODE) {
				Keycheck_Calibration();  // Process calibration mode keys
			}

			// Wait for key release (except for UP/DOWN which can repeat)
//			if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
//				while (Read_key() != NO_KEY);
//			}

//			if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
//			    Keyboard.KeyPressed = NO_KEY;  // ✓ Just clear it, don't block
//			}//05/03/2026


			/*if (Keyboard.KeyPressed == KEY_ENTER) {
				while (Read_Keys() & KEY_ENTER) {
					HAL_Delay(10);
				}
				HAL_Delay(50);  // debounce after release
				Keyboard.KeyPressed = NO_KEY;
			}
			else*/ if ((Keyboard.KeyPressed != KEY_DOWN) && (Keyboard.KeyPressed != KEY_UP)) {
			    Keyboard.KeyPressed = NO_KEY;
			}
		}

		Keyboard_LED_Update();  // Update LED patterns
		// ===== REFERENCE PATTERN END =====

		// FRAM write handling
		if (ModbusHandler.pending_fram_write) {
			HAL_Delay(50);
			bool write_ok = FRAM_Write_Config(&device_config);
			ModbusHandler.pending_fram_write = false;
			if (write_ok) {
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			}
		}

		// Check for sensor fault
		bool sensor_fault = ADC_Is_Sensor_Fault();
		AnalogOutput_Set_Fault(sensor_fault);

//		if (sensor_fault) {
//			Keyboard_SetMode(MODE_SENSOR_BROKE);
//		}

		// Get current mode
		    OperationMode_e current_mode = Keyboard_GetMode();

		    if (sensor_fault) {
		        // Fault detected - go to fault mode (but preserve calibration modes!)
//		        if (current_mode != MODE_INPUT_CAL_LOW &&
//		            current_mode != MODE_INPUT_CAL_HIGH &&
//		            current_mode != MODE_OUTPUT_CAL_LOW &&
//		            current_mode != MODE_OUTPUT_CAL_HIGH)
		    	if (current_mode != MODE_INPUT_CAL      &&
		    	        current_mode != MODE_INPUT_CAL_LOW  &&
		    	        current_mode != MODE_INPUT_CAL_HIGH &&
		    	        current_mode != MODE_INPUT_CAL_DONE &&
		    	        current_mode != MODE_OUTPUT_CAL     &&
		    	        current_mode != MODE_OUTPUT_CAL_LOW &&
		    	        current_mode != MODE_OUTPUT_CAL_HIGH&&
		    	        current_mode != MODE_OUTPUT_CAL_DONE){
		            Keyboard_SetMode(MODE_SENSOR_BROKE);
		        }
		    } else {
		        // Fault cleared - AUTO-RECOVER to normal mode!
		        if (current_mode == MODE_SENSOR_BROKE) {
		            Keyboard_SetMode(MODE_NORMAL);  // ← AUTO-RECOVERY!
		        }
		    }



		// Update analog output
		float input_value = ADC_Get_Float_Value();
		AnalogOutput_Update(input_value,
				(float)device_config.input_low,
				(float)device_config.input_high);

		// Handle calibration value changes
		OperationMode_e mode = Keyboard_GetMode();
		uint8_t key = Keyboard.KeyPressed;  // From structure now



		switch (mode) {
		case MODE_INPUT_CAL_LOW:
			if (key == KEY_UP) {
			        if (device_config.cal_input_lo < 65535) device_config.cal_input_lo++;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Inc();
			            if (device_config.cal_input_lo > (65535 - Keyboard.New_Val))
			                device_config.cal_input_lo = 65535;
			            else
			                device_config.cal_input_lo += Keyboard.New_Val;
			        }
			    }
			    if (key == KEY_DOWN) {
			        if (device_config.cal_input_lo > 0) device_config.cal_input_lo--;   // guard underflow
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Dec();
			            if (device_config.cal_input_lo >= Keyboard.New_Val)
			                device_config.cal_input_lo -= Keyboard.New_Val;
			            else
			                device_config.cal_input_lo = 0;
			        }
			    }
			    device_config.Inp_Cal_code1 = ADCHandler.adc_raw;
			    break;

		case MODE_INPUT_CAL_HIGH:
			 if (key == KEY_UP) {
			        if (device_config.cal_input_hi < 65535) device_config.cal_input_hi++;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Inc();
			            if (device_config.cal_input_hi > (65535 - Keyboard.New_Val))
			                device_config.cal_input_hi = 65535;
			            else
			                device_config.cal_input_hi += Keyboard.New_Val;
			        }
			    }
			    if (key == KEY_DOWN) {
			        if (device_config.cal_input_hi > 0) device_config.cal_input_hi--;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Dec();
			            if (device_config.cal_input_hi >= Keyboard.New_Val)
			                device_config.cal_input_hi -= Keyboard.New_Val;
			            else
			                device_config.cal_input_hi = 0;
			        }
			    }
			    device_config.Inp_Cal_code2 = ADCHandler.adc_raw;
			    break;
		case MODE_OUTPUT_CAL_LOW:

			AnalogOutput_Calibration_Bypass(true);

			    if (key == KEY_UP) {
			        if (device_config.cal_output_lo < 2000) device_config.cal_output_lo++;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Inc();
			            device_config.cal_output_lo += Keyboard.New_Val;
			            if (device_config.cal_output_lo > 2000) device_config.cal_output_lo = 2000;
			        }
			    }
			    if (key == KEY_DOWN) {
			        if (device_config.cal_output_lo > 0) device_config.cal_output_lo--;   // guard underflow
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Dec();
			            if (device_config.cal_output_lo >= Keyboard.New_Val)
			                device_config.cal_output_lo -= Keyboard.New_Val;
			            else
			                device_config.cal_output_lo = 0;                              // floor, don't wrap
			        }
			    }

			    AnalogOutput_Set_Raw_PWM(device_config.cal_output_lo);
			    break;

		case MODE_OUTPUT_CAL_HIGH:

			AnalogOutput_Calibration_Bypass(true);

			    if (key == KEY_UP) {
			        if (device_config.cal_output_hi < 2000) device_config.cal_output_hi++;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Inc();
			            device_config.cal_output_hi += Keyboard.New_Val;
			            if (device_config.cal_output_hi > 2000) device_config.cal_output_hi = 2000;
			        }
			    }
			    if (key == KEY_DOWN) {
			        if (device_config.cal_output_hi > 0) device_config.cal_output_hi--;
			        if (Keyboard.Fast_Value_Change) {
			            Fast_value_Dec();
			            if (device_config.cal_output_hi >= Keyboard.New_Val)
			                device_config.cal_output_hi -= Keyboard.New_Val;
			            else
			                device_config.cal_output_hi = 0;
			        }
			    }

			    AnalogOutput_Set_Raw_PWM(device_config.cal_output_hi);
			    break;

		case MODE_INPUT_CAL_DONE:
		    ADC_Calibrate((float)device_config.cal_input_lo,
		            (float)device_config.cal_input_hi,
		            (float)device_config.Inp_Cal_code1,
		            (float)device_config.Inp_Cal_code2);

		    ADC_Save_Calibration_To_Config(&device_config);
		    FRAM_Write_Config(&device_config);
		    break;

		case MODE_OUTPUT_CAL_DONE:
		    // Disable bypass mode - return to normal temperature scaling
		    AnalogOutput_Calibration_Bypass(false);

		    // Apply the final calibration values
		    AnalogOutput_Set_Calibration(device_config.cal_output_lo,
		                                 device_config.cal_output_hi);

		    // Save to FRAM
		    FRAM_Write_Config(&device_config);
		    break;

		case MODE_RESTORE_CAL:
		    // Restore factory defaults
		    FRAM_Restore_Defaults();

		    // Read back the defaults
		    FRAM_Read_Config(&device_config);

		    // Restore ADC calibration
		    ADC_Restore_Calibration_From_Config(&device_config);

		    // Restore PWM calibration
		    AnalogOutput_Set_Calibration(device_config.cal_output_lo,
		                                 device_config.cal_output_hi);
		    break;


		case MODE_PWM_TEST:
		{
		    static uint16_t pwm_test_value = 0;   // Start at 0, not 500

		    AnalogOutput_Calibration_Bypass(true);

		    if (key == KEY_UP) {
		        if (pwm_test_value < 2000) pwm_test_value++;   // Guard before increment
		        if (Keyboard.Fast_Value_Change) {
		            Fast_value_Inc();
		            pwm_test_value += Keyboard.New_Val;
		            if (pwm_test_value > 2000) pwm_test_value = 2000;
		        }
		    }
		    if (key == KEY_DOWN) {
		        if (pwm_test_value > 0) pwm_test_value--;      // Guard: uint16_t can't go negative
		        if (Keyboard.Fast_Value_Change) {
		            Fast_value_Dec();
		            if (pwm_test_value >= Keyboard.New_Val)
		                pwm_test_value -= Keyboard.New_Val;
		            else
		                pwm_test_value = 0;
		        }
		    }

		    AnalogOutput_Set_Raw_PWM(pwm_test_value);

		    if (key == KEY_ENTER) {
		        pwm_test_value = 0;                            // Reset for next entry
		        AnalogOutput_Calibration_Bypass(false);
		        Keyboard_SetMode(MODE_NORMAL);
		    }
		}
		break;




		default:
		    break;
		}



		HAL_Delay(10);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC_Init(void)
{

	/* USER CODE BEGIN ADC_Init 0 */

	/* USER CODE END ADC_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC_Init 1 */

	/* USER CODE END ADC_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = ENABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel to be converted.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel to be converted.
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC_Init 2 */

	/* USER CODE END ADC_Init 2 */

}

/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRC_Init(void)
{

	/* USER CODE BEGIN CRC_Init 0 */

	/* USER CODE END CRC_Init 0 */

	/* USER CODE BEGIN CRC_Init 1 */

	/* USER CODE END CRC_Init 1 */
	hcrc.Instance = CRC;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN CRC_Init 2 */

	/* USER CODE END CRC_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00201D2B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 11;//23;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 2000;
			//1000;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;//TIM_OCPOLARITY_LOW;//
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * @brief TIM14 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM14_Init(void)
{

	/* USER CODE BEGIN TIM14_Init 0 */

	/* USER CODE END TIM14_Init 0 */

	/* USER CODE BEGIN TIM14_Init 1 */

	/* USER CODE END TIM14_Init 1 */
	htim14.Instance = TIM14;
	htim14.Init.Prescaler = 47999;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = 9;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM14_Init 2 */

	/* USER CODE END TIM14_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 1, 1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, HI_INP_Pin|LO_INP_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
	GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : LED4_Pin */
	GPIO_InitStruct.Pin = LED4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : K1_Pin */
	GPIO_InitStruct.Pin = K1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(K1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : K2_Pin K3_Pin K4_Pin */
	GPIO_InitStruct.Pin = K2_Pin|K3_Pin|K4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : HI_INP_Pin LO_INP_Pin */
	GPIO_InitStruct.Pin = HI_INP_Pin|LO_INP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	HAL_GPIO_WritePin(GPIOC, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, HI_INP_Pin|LO_INP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
