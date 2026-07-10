#include "adc.h"
#include "fram.h"

// Private variables

// K-Type Tables
float const  mvktable[8] = {8.2160,14.5020,31.38,38.479,45.07,50.347,54.875};
float const slopktable[8] = {24.5861733,24.3398027,23.6402417,24.6513593,25.7927477,27.0987303,28.7102473};
float const consktable[8] = {0.00,2.02,12.17,-19.56,-63.48,-122.34,-203.47};


// J-Type Tables
float const mvjtable[8] = {5.4310,29.9260,36.7920,44.8520,53.607,61.923,69.536};
float const slopjtable[8] = {18.9651998,18.0444989,17.0404894,15.8808933,15.7624215,16.8350168,17.3387626};
float const consjtable[8] = {0.00,5.00,35.05,77.71,83.02,25.53,-5.67};

// R-Type Tables
float const mvrtable[14] = {0.2450,0.5950,1.0900,1.7660,2.6660,3.8370,5.26600,6.8240,8.5450,10.4500,12.7250,19.4920,20.9940,21.0960};
float const sloprtable[14] = {171.428571,145.714286,127.272727,113.905325,104.444444,97.3526900,91.6724983,86.6495507,81.9291110,77.6902887,73.8461538,71.3757943,74.5672437,78.4313725};
float const consrtable[14] = {0.00,6.30,17.27,31.84,48.55,67.46,89.25,115.70,147.92,184.14,224.31,255.74,193.54,112.41};


// S-Type Tables
float const mvstable[13] = {0.2728,0.6674,1.2313,2.0421,3.2024,4.7719,6.3376,8.0916,9.9786,12.5498,16.7949,18.3715,18.6982};
float const slopstable[13] = {168.621701,144.450076,127.682213,115.934899,107.730759,101.943294,97.0811777,92.3603193,87.9703233,84.0074673,82.91911303,86.2615755,91.8273646};
float const consstable[13] = {0.00,6.59,17.78,32.25,49.00,67.54,90.74,120.66,156.16,195.72,209.38,153.25,50.99};


// B-Type Tables
float const mvbtable[26] = {0.0030,0.0050,0.0130,0.0210,0.036,0.06,0.0990,0.155,0.222,0.3170,0.437,0.592,0.794,1.052,1.36,1.726,2.177,2.7240,3.363,4.1170,4.989,5.993,7.212,8.694,10.767,13.814};
float const slopbtable[26] = {17333.333333333,3000.0,1875.0,1500.0,1200.0,958.333333333,743.58974358974,589.2857142857,492.5373343284,410.52631578947,350.0,303.22580645161,262.37623762376,228.68217054264,201.29870128870,180.32786885246,161.86252771619,146.25228519196,133.02034428795,122.01591511936,112.38532110092,104.58167330677,97.621000820345,91.767881241565,87.313072841293,85.986215950115};
float const consbtable[26] = {0.0,43.0,48.63,53.50,59.8,68.5,81.38,96.66,111.66,129.86,149.05,169.49,193.67,220.43,249.23,277.75,309.63,343.61,379.65,416.66,456.31,495.24,536.96,579.17,617.9,632.19};


// T-Type Tables
float const mvttable[68] = {-6.258,-6.232,-6.18,-6.105,-6.007,-5.888,-5.603,-5.439,-5.261,-5.07,-4.865,-4.648,-4.419,-4.177,-3.923,-3.657,-3.379,-3.089,-2.788,-2.476,-2.153,-1.819,-1.475,-1.121,-0.757,-0.383,0,0.391,0.79,1.196,1.612,2.036,2.468,2.909,3.358,3.814,4.279,4.75,5.228,5.714,6.206,6.704,7.209,7.72,8.237,8.759,9.288,9.822,10.362,10.907,11.458,12.013,12.574,13.139,13.709,14.283,14.862,15.445,16.032,16.62,17.219,17.819,18.422,19.03,19.641,20.255,20.872};
float const slopttable[68] = {384.6153846,192.3076923	,133.3333333	,102.0408163	,84.03361345	,74.07407407	,66.66666667	,60.97560976	,56.17977528	,52.35602094	,48.7804878	,46.08294931	,43.66812227	,41.32231405	,39.37007874	,37.59398496	,35.97122302	,34.48275862	,33.22259136	,32.05128205	,30.95975232	,29.94011976	,29.06976744	,28.24858757	,27.47252747	,26.73796791	,26.10966057	,25.57544757	,25.06265664	,24.63054187	,24.03846154	,23.58490566	,23.14814815	,22.67573696	,22.27171492	,21.92982456	,21.50537634	,21.23142251	,20.92050209	,20.57613169	,20.32520325	,20.08032129	,19.8019802	,19.56947162	,19.34235977	,19.15708812	,18.90359168,
18.72659176	,18.51851852	,18.34862385	,18.14882033	,18.01801802	,17.82531194	,17.69911504	,17.54385965	,17.42160279	,
17.27115717	,17.15265866	,17.03577513	,16.89189189	,16.80672269	,16.66666667	,16.58374793	,16.44736842	,16.36661211	,16.28664495	,
16.20745543	,19.16443082	};
float const consttable[68] = {2136.923077	,938.4615385	,574	,382.9591837	,274.789916	,216.1481481	,173.5333333	,141.6463415	,
115.5617978	,95.44502618	,77.31707317	,64.19354839	,52.96943231	,42.60330579	,34.4488189	,27.48120301	,21.54676259	,16.51724138	,
12.62458472	,9.358974359	,6.656346749	,4.461077844	,2.877906977	,1.666666667	,0.796703297	,0.240641711	,0	,0	,0.200501253	,0.541871921	,
1.25	,1.981132075	,2.87037037	,4.036281179	,5.211581292	,6.359649123	,7.978494624	,9.1507431	,10.62761506	,12.42798354	,13.86178862	,
15.3815261	,17.24752475	,18.92367906	,20.67698259	,22.20306513	,24.42344045	,26.06741573	,28.11111111	,29.87155963	,
32.0508167	,33.54954955	,35.86452763	,37.45132743	,39.49122807	,41.16724739	,43.31606218	,45.07718696	,46.88245315	,49.18918919	,
50.60504202	,53.01666667	,54.49419569	,57.00657895	,58.54337152	,60.11400651	,61.71799028	,0	};


// N-Type Tables
float const mvntable[21] = {0.793,1.619,2.774,4.302,5.913,7.597,9.341,10.413,11.501,12.974,14.846,16.748,18.672,20.613,
24.527,28.455,32.371,36.256,43.846,47.152};

float const slopntable[21] = {37.83102144,36.29536921,34.60514641,32.71028037,31.01265823,29.67898243,28.65497076,
27.99227799,27.56653992,27.13987474,26.70299728,26.28755365,25.98091198,25.76235542,25.5483871,25.45641553,
25.53520763,25.74102964,26.35046113,27.22323049};

float const consntable[21] = {0,1.237797247,4.005323869,9.280373832,16.6221519,24.52877044,32.33391813,38.51640927,
42.95722433,47.88726514,53.56730245,59.7360515,64.88441145,68.96056782,73.37470968,75.63769607,73.39979365,
66.73322933,44.63768116,6.370235935};

// RTD Tables
float const mvrtdtable[10] = {8.1720,16.3949,24.1197,32.0769,39.1980,46.5535,53.9637,61.2463,68.2404,71.0372};
float const sloprtdtable[10] = {5.2618698,5.3509103,5.4370340,5.5295833,5.6171097,5.7100129,5.8028123,5.9044847,6.0050614,6.0783753};
float const constrtdtable[10] = {0.00,-0.73,-2.14,-4.33,-7.18,-10.82,-15.14,-20.63,-26.79,-31.79};
int   const temprtd[10] = {43,87,129,173,213,255,298,341,383,400};



// Private variables
static ADC_HandleTypeDef *adc_handle;
static uint8_t table_count = 0;

ADC_Handle_t ADCHandler;
extern FRAM_Config_t device_config;

// Private function prototypes
static void Sensitivity_Selection(void);
static float MV_Conversion(void);
static void Convert_To_Temp(void);
static void Temp_Cal(void);
static void Voltage_Cal(void);
static void mA_Cal(void);
static void Breakpoint_Select(void);
static void Temp_Final_Cal(void);
static void Add_Offset(void);
float rtd_val(uint16_t calstore);
void ADC_Save_Calibration_To_Config(FRAM_Config_t *config);
void ADC_Restore_Calibration_From_Config(FRAM_Config_t *config);
void ADC_Get_Calibration(uint16_t *oe_code, uint16_t *ge_code, uint16_t *flag_code);
void ADC_Set_Calibration(uint16_t oe_code, uint16_t ge_code, uint16_t flag_code);
void ADC_Init(ADC_HandleTypeDef *hadc)
{
    adc_handle = hadc;

    // Initialize default values
    ADCHandler.sensor_type = MA_4_20;
    ADCHandler.decimal_point = DP_1;
    ADCHandler.input_low = 0;
    ADCHandler.input_high = 1000;
    ADCHandler.offset = 0;

//    // Default calibration (no correction)
//    ADCHandler.cal.oe_code = 0;
//    ADCHandler.cal.ge_code = 256;  // 256/256 = 1.0 (unity gain)
//    ADCHandler.cal.flag_code = 0;

    // Initialize buffers
    ADCHandler.adc_raw = 0;
    ADCHandler.cjc_raw = 0;
    ADCHandler.mv_result = 0.0f;
    ADCHandler.lin_value = 0.0f;
    ADCHandler.modbus_value = 0;

    // Start ADC with DMA
    HAL_ADC_Start_DMA(adc_handle, (uint32_t*)ADCHandler.adc_buffer, ADC_BUFFER_SIZE);
}

void ADC_Process(void)
{
    // Update raw ADC values from DMA buffer
    ADCHandler.adc_raw = ADCHandler.adc_buffer[0];  // Input channel
    ADCHandler.cjc_raw = ADCHandler.adc_buffer[1];  // CJC channel

    // Check if thermocouple type - calculate CJC compensation
    if (ADCHandler.sensor_type >= K_TYPE && ADCHandler.sensor_type <= T_TYPE) {
        Sensitivity_Selection();

        // Calculate CJC compensation
        ADCHandler.mv_cjc = (((float)(ADCHandler.cjc_raw * VREF) / ADC_MAX_VALUE) - AMB_KELVIN)
                            * ADCHandler.sensitivity;
    }

    // Convert ADC code to mV
    ADCHandler.mv_result = MV_Conversion();

    // Convert to temperature/value
    Convert_To_Temp();

    // Handle B-type thermocouple special case
    if (ADCHandler.sensor_type == B_TYPE) {
        if ((ADCHandler.lin_value > -200) && (ADCHandler.lin_value < 0)) {
            ADCHandler.lin_value = 0;
        }
    }

    // Store modbus value
    ADCHandler.modbus_value = (int16_t)ADCHandler.lin_value;
    device_config.process_value = ADCHandler.lin_value;
}

static void Sensitivity_Selection(void)
{
    switch (ADCHandler.sensor_type) {
        case J_TYPE:
            ADCHandler.sensitivity = 0.0530f;
            break;
        case K_TYPE:
            ADCHandler.sensitivity = 0.040f;
            break;
        case R_TYPE:
            ADCHandler.sensitivity = 0.00591380f;
            break;
        case S_TYPE:
            ADCHandler.sensitivity = 0.005807f;
            break;
        case N_TYPE:
            ADCHandler.sensitivity = 0.0260f;
            break;
        case T_TYPE:
            ADCHandler.sensitivity = 0.0430f;
            break;
        case B_TYPE:
            ADCHandler.sensitivity = 0.006f;
            break;
        default:
            ADCHandler.sensitivity = 0.0f;
            break;
    }
}

static float MV_Conversion(void)
{
    float mv_temp;

    // Apply offset error correction
    if (ADCHandler.cal.flag_code == 1) {
        // Positive offset
        mv_temp = (float)(ADCHandler.adc_raw + ADCHandler.cal.oe_code);
    } else {
        // Negative offset
        mv_temp = (float)(ADCHandler.adc_raw - ADCHandler.cal.oe_code);
    }

    // Apply gain error correction
    float mv_return = (mv_temp / 256.0f) * (ADCHandler.cal.ge_code / 256.0f);

    return mv_return;
}

static void Convert_To_Temp(void)
{
    if ((ADCHandler.sensor_type == MA_0_20) ||
        (ADCHandler.sensor_type == MA_4_20) ||
        (ADCHandler.sensor_type == RTD)) {

        ADCHandler.temp = ADCHandler.mv_result / 100.0f;

        if (ADCHandler.sensor_type == RTD) {
            Temp_Cal();
        } else {
            mA_Cal();
        }
    }
    else if ((ADCHandler.sensor_type >= VOLT_0_1) &&
             (ADCHandler.sensor_type <= VOLT_0_10)) {

        ADCHandler.temp = ADCHandler.mv_result;
        Voltage_Cal();
    }
    else if (ADCHandler.sensor_type <= T_TYPE) {
        // Thermocouples - add CJC compensation
        ADCHandler.temp = ((ADCHandler.mv_result / 100.0f + ADCHandler.mv_cjc)) * 100.0f;
        Temp_Cal();
    }

    Add_Offset();
}

static void Temp_Cal(void)
{
    if (ADCHandler.sensor_type <= T_TYPE) {
        ADCHandler.temp = ADCHandler.temp / 100.0f;
    }

    table_count = 0;

    do {
        Breakpoint_Select();

        // Check table limits for each sensor type
        if ((ADCHandler.table_mv_input >= ADCHandler.temp) ||
            ((table_count >= 9) && (ADCHandler.sensor_type == RTD)) ||
            ((table_count >= 6) && (ADCHandler.sensor_type == K_TYPE)) ||
            ((table_count >= 6) && (ADCHandler.sensor_type == J_TYPE)) ||
            ((table_count >= 25) && (ADCHandler.sensor_type == B_TYPE)) ||
            ((table_count >= 13) && (ADCHandler.sensor_type == R_TYPE)) ||
            ((table_count >= 12) && (ADCHandler.sensor_type == S_TYPE)) ||
            ((table_count >= 20) && (ADCHandler.sensor_type == N_TYPE)) ||
            ((table_count >= 67) && (ADCHandler.sensor_type == T_TYPE))) {
            break;
        }

        table_count++;
        Breakpoint_Select();

    } while (ADCHandler.table_mv_input <= ADCHandler.temp);

    Temp_Final_Cal();

    // Apply decimal point
    if ((ADCHandler.sensor_type == RTD) && (ADCHandler.decimal_point == DP_1)) {
        ADCHandler.lin_value = ADCHandler.lin_value / 10.0f;
    } else {
        ADCHandler.lin_value = ADCHandler.lin_value / 100.0f;
    }

    table_count = 0;
}

static void Voltage_Cal(void)
{
    float span = (float)(ADCHandler.input_high - ADCHandler.input_low);

    if (ADCHandler.sensor_type == VOLT_0_1) {
        ADCHandler.lin_value = (ADCHandler.temp * span / 1000.0f) +
                              (float)ADCHandler.input_low;
    }
    else if (ADCHandler.sensor_type == VOLT_0_5) {
        ADCHandler.lin_value = (ADCHandler.temp * span / 5000.0f) +
                              (float)ADCHandler.input_low;
    }
    else if (ADCHandler.sensor_type == VOLT_0_10) {
        ADCHandler.lin_value = (ADCHandler.temp * span / 10000.0f) +
                              (float)ADCHandler.input_low;
    }
}

static void mA_Cal(void)
{
    float span = (float)(ADCHandler.input_high - ADCHandler.input_low);

    if (ADCHandler.sensor_type == MA_4_20) {
        ADCHandler.lin_value = ((ADCHandler.temp-4.0f) /* 10.0f*/ * span / 16.0f) +
                              (float)(ADCHandler.input_low);
    }
    else if (ADCHandler.sensor_type == MA_0_20) {
        ADCHandler.lin_value = (ADCHandler.temp / 10.0f * span / 20.0f) +
                              (float)ADCHandler.input_low;
    }

}

static void Add_Offset(void)
{
    ADCHandler.lin_value = ADCHandler.lin_value + (float)ADCHandler.offset;
}

static void Breakpoint_Select(void)
{
    switch (ADCHandler.sensor_type) {
        case RTD:
            ADCHandler.table_mv_input = mvrtdtable[table_count];
            break;
        case K_TYPE:
            ADCHandler.table_mv_input = mvktable[table_count];
            break;
        case J_TYPE:
            ADCHandler.table_mv_input = mvjtable[table_count];
            break;
        case R_TYPE:
            ADCHandler.table_mv_input = mvrtable[table_count];
            break;
        case S_TYPE:
            ADCHandler.table_mv_input = mvstable[table_count];
            break;
        case B_TYPE:
            ADCHandler.table_mv_input = mvbtable[table_count];
            break;
        case N_TYPE:
            ADCHandler.table_mv_input = mvntable[table_count];
            break;
        case T_TYPE:
            ADCHandler.table_mv_input = mvttable[table_count];
            break;
        default:
            ADCHandler.table_mv_input = 0;
            break;
    }
}

static void Temp_Final_Cal(void)
{
    switch (ADCHandler.sensor_type) {
        case K_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopktable[table_count] +
                                   consktable[table_count]) * 100.0f;
            break;
        case J_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopjtable[table_count] +
                                   consjtable[table_count]) * 100.0f;
            break;
        case R_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * sloprtable[table_count] +
                                   consrtable[table_count]) * 100.0f;
            break;
        case S_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopstable[table_count] +
                                   consstable[table_count]) * 100.0f;
            break;
        case B_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopbtable[table_count] +
                                   consbtable[table_count]) * 100.0f;
            break;
        case RTD:
            ADCHandler.lin_value = (ADCHandler.temp * sloprtdtable[table_count] +
                                   constrtdtable[table_count]) * 100.0f;
            break;
        case N_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopntable[table_count] +
                                   consntable[table_count]) * 100.0f;
            break;
        case T_TYPE:
            ADCHandler.lin_value = (ADCHandler.temp * slopttable[table_count] +
                                   consttable[table_count]) * 100.0f;
            break;
        default:
            break;
    }
}

void ADC_Calibrate(float cal_low_mv, float cal_high_mv,
                   float actual_low, float actual_high)
{
//    // Calculate offset error
//    float offset_error = actual_low - cal_low_mv;
//
//    if (offset_error >= 0) {
//        ADCHandler.cal.flag_code = 1;  // Positive offset
//        ADCHandler.cal.oe_code = (uint16_t)fabsf(offset_error);
//    } else {
//        ADCHandler.cal.flag_code = 0;  // Negative offset
//        ADCHandler.cal.oe_code = (uint16_t)fabsf(offset_error);
//    }
//
//    // Calculate gain error
//    float gain = (actual_high - actual_low) / (cal_high_mv - cal_low_mv);
//    ADCHandler.cal.ge_code = (uint16_t)(gain * 256.0f);

uint32_t meas1, meas2;
float    oecal1 ,gecal1,flagcal1;
float    oetemp1,getemp1;
meas1 = cal_low_mv;//cal1 value
meas2 = cal_high_mv;//cal2 value
if(ADCHandler.sensor_type == RTD)//for rtd type i/p resistance value to be converted to equivalent temperature
{
meas1 = (uint32_t)rtd_val(cal_low_mv);//get meas1 from cal1store resistance value
meas2 = (uint32_t)rtd_val(cal_high_mv);//get meas2 from cal2store resistance value
}
if((ADCHandler.sensor_type == MA_0_20) || (ADCHandler.sensor_type == MA_4_20) || ((ADCHandler.sensor_type >= VOLT_0_1)&&(ADCHandler.sensor_type <= VOLT_0_10)))//for rtd type i/p resistance value to be converted to equivalent temperature
{
meas1 = (cal_low_mv/**10*/);//get meas1 from cal1store resistance value
meas2 = (cal_high_mv/**10*/);//get meas2 from cal2store resistance value
}

getemp1 = (float)(((uint32_t)(meas2 - meas1)<<16))/(actual_high - actual_low);
gecal1 = (float)((getemp1/*+1*/)/*/2.0*/);
//oetemp1 = (float)((uint32_t)(meas1<<16)/gecal1);//gain error
oetemp1 = (float)((uint32_t)(meas1<<16)) / gecal1;//gain error
if (oetemp1 <= actual_low)
{
oecal1 = (float)(actual_low - oetemp1);//offerror
flagcal1 = 0;
}
if (oetemp1 > actual_low)
{
oecal1 = (float)(oetemp1 - actual_low);//offerror
flagcal1 = 1;//offset sign
}
ADCHandler.cal.oe_code		= oecal1;
ADCHandler.cal.ge_code		= gecal1;
ADCHandler.cal.flag_code = flagcal1;
}




void ADC_Set_Sensor_Type(SensorType_t type)
{
    ADCHandler.sensor_type = type;
}

void ADC_Set_Range(int16_t low, int16_t high)
{
    ADCHandler.input_low = low;
    ADCHandler.input_high = high;
}

void ADC_Set_Offset(int16_t offset)
{
    ADCHandler.offset = offset;
}

int16_t ADC_Get_Value(void)
{
    return ADCHandler.modbus_value;
}

float ADC_Get_Float_Value(void)
{
    return ADCHandler.lin_value;
}

bool ADC_Is_Sensor_Fault(void)
{
    // Sensor fault detection logic
    // You can implement based on your sensor type

    // Example for RTD/Thermocouple:
    // If ADC reading is too low or too high (open circuit detection)
//    if (ADCHandler.adc_raw < 50 || ADCHandler.adc_raw > 4000) {
//        return true;  // Sensor fault detected
//    }

//    if (ADCHandler.sensor_type >= K_TYPE && ADCHandler.sensor_type <= T_TYPE) {
//        if (ADCHandler.lin_value < -500 || ADCHandler.lin_value > 5000) {
//            return true;  // Out of valid range
//        }
//    }

	if (ADCHandler.lin_value < ADCHandler.input_low || ADCHandler.lin_value > ADCHandler.input_high)
		return true;

    // For thermocouples, check if reading is out of valid range
//    if (ADCHandler.sensor_type >= K_TYPE && ADCHandler.sensor_type <= T_TYPE) {
//        if (ADCHandler.lin_value < -500 || ADCHandler.lin_value > 5000) {
//            return true;  // Out of valid range
//        }
//    }

    return false;  // No fault
}


float rtd_val(uint16_t calstore)
{
float mv_rtd;
uint16_t tablecount=0;
do
  {
  tablecount = tablecount + 1;
  mv_rtd = (float)calstore/10.0;
  }while (temprtd[tablecount] <= (uint32_t)mv_rtd);//when temp fed > table temperature
  mv_rtd = (((mv_rtd - constrtdtable[tablecount-1])/ sloprtdtable[tablecount-1]) *100);//mv = (temperature-constant)/slope

  tablecount = 0;
  return mv_rtd;
}


void ADC_Save_Calibration_To_Config(FRAM_Config_t *config)
{
    config->adc_cal_oe_code = ADCHandler.cal.oe_code;
    config->adc_cal_ge_code = ADCHandler.cal.ge_code;
    config->adc_cal_flag_code = ADCHandler.cal.flag_code;
}

/**
 * @brief Restore ADC calibration parameters from FRAM config structure
 * @param config: Pointer to FRAM_Config_t structure
 * @retval None
 */
void ADC_Restore_Calibration_From_Config(FRAM_Config_t *config)
{
    ADCHandler.cal.oe_code = config->adc_cal_oe_code;
    ADCHandler.cal.ge_code = config->adc_cal_ge_code;
    ADCHandler.cal.flag_code = config->adc_cal_flag_code;
}

/**
 * @brief Get current ADC calibration parameters
 * @param oe_code: Pointer to store offset error code
 * @param ge_code: Pointer to store gain error code
 * @param flag_code: Pointer to store flag code
 * @retval None
 */
void ADC_Get_Calibration(uint16_t *oe_code, uint16_t *ge_code, uint16_t *flag_code)
{
    if (oe_code != NULL) {
        *oe_code = ADCHandler.cal.oe_code;
    }
    if (ge_code != NULL) {
        *ge_code = ADCHandler.cal.ge_code;
    }
    if (flag_code != NULL) {
        *flag_code = ADCHandler.cal.flag_code;
    }
}

/**
 * @brief Set ADC calibration parameters directly
 * @param oe_code: Offset error code
 * @param ge_code: Gain error code
 * @param flag_code: Flag code (0=negative offset, 1=positive offset)
 * @retval None
 */
void ADC_Set_Calibration(uint16_t oe_code, uint16_t ge_code, uint16_t flag_code)
{
    ADCHandler.cal.oe_code = oe_code;
    ADCHandler.cal.ge_code = ge_code;
    ADCHandler.cal.flag_code = flag_code;
}
