#include "analysis.h"
#include "math.h"
#include "fft.h"

#define VOL_STD_RMS_A  3.950780771168715722E+4
#define VOL_STD_RMS_B  3.950780771168715722E+4
#define VOL_STD_RMS_C  3.951002815712404111E+4

#define VOL_STD_FACTOR 2345
#define PI_ 3.141592654
extern uint8_t AD_save_flag, AD_save_flag1;

extern uint16_t AD_01_dataupper[2566];  //C
extern uint16_t AD_01_datalower[2566];  //C
extern uint16_t AD_02_dataupper[2566];  //B
extern uint16_t AD_02_datalower[2566];  //B
extern uint16_t AD_03_dataupper[2566];  //A
extern uint16_t AD_03_datalower[2566];  //A

extern uint16_t flicker_01_dataupper[130];  //C
extern uint16_t flicker_01_datalower[130];  //C
extern uint16_t flicker_02_dataupper[130];  //B
extern uint16_t flicker_02_datalower[130];  //B
extern uint16_t flicker_03_dataupper[130];  //A
extern uint16_t flicker_03_datalower[130];  //A
extern uint8_t AD_TEMP_flag, flicker_flag;//, flicker_Falg1;

extern float fft_harmonicA_output[]; //FFT 输出数组
extern float fft_harmonicB_output[]; //FFT 输出数组
extern float fft_harmonicC_output[]; //FFT 输出数组




uint16_t AD_RMS_Time = 0;

double AD_data0 = 0, AD_data1 = 0, AD_data2 = 0;

double ad_RMS[3][10] = {0}; // 全局数组，三相电压均方根数组
float LOGIC_RMS[3] = {0};

/********************电压有效值************************************/
// 求均方根
void LOGIC_Voltage_RMS_value(uint16_t ad3[], uint16_t ad2[], uint16_t ad1[])
{
    uint16_t i = 0, j = 0, data_j = 0;;
    for (j = 0; j < 10; j++) {
        data_j = j * 128;
        for (i = 0; i < 128; i++) {
            LOGIC_RMS[0] = (float) *(ad1 + data_j + i);//(double)
            LOGIC_RMS[1] = (float) *(ad2 + data_j + i);//AD_02_data[data_j+i];
            LOGIC_RMS[2] = (float) *(ad3 + data_j + i);//AD_01_data[data_j+i];
            //if((ad0>35500)|(ad0<35500)){flag_a0++;}                              //电压暂升、暂降、短时中断
            // if((ad0>35500)|(ad0<35500)){flag_a1++;}
            //if((ad0>35500)|(ad0<35500)){flag_a2++;}
            AD_data0 = pow(LOGIC_RMS[0], 2) + AD_data0;//
            AD_data1 = pow(LOGIC_RMS[1], 2) + AD_data1;//pow(LOGIC_RMS[1],2)
            AD_data2 = pow(LOGIC_RMS[2], 2) + AD_data2;   //pow(LOGIC_RMS[2],2)
        }
        ad_RMS[0][j] = sqrt(AD_data0 / 128);
        ad_RMS[1][j] = sqrt(AD_data1 / 128);
        ad_RMS[2][j] = sqrt(AD_data2 / 128);
        AD_data0 = 0;
        AD_data1 = 0;
        AD_data2 = 0;
    }
    // 以下求10次均方根的均值，然后干啥用呢
    LOGIC_RMS[0] = 0;
    LOGIC_RMS[1] = 0;
    LOGIC_RMS[2] = 0;
    for (i = 0; i < 10; i++) {
        LOGIC_RMS[0] = LOGIC_RMS[0] + ad_RMS[0][i];
        LOGIC_RMS[1] = LOGIC_RMS[1] + ad_RMS[1][i];
        LOGIC_RMS[2] = LOGIC_RMS[2] + ad_RMS[2][i];
    }
    LOGIC_RMS[0] = LOGIC_RMS[0] / 10;
    LOGIC_RMS[1] = LOGIC_RMS[1] / 10;
    LOGIC_RMS[2] = LOGIC_RMS[2] / 10;

    //return  ad_RMS;

}


/********************电压暂降、暂升：电压暂降是指电力系统中某点工频电压方均根值暂时降低至0.1~0.9p.u.，并在
短时持续10ms~1min后恢复正常的现象；电压暂升是指电力系统中某点工频方均根值暂时上升至1.1~1.8p.u.，并在短时持
续10ms~1min后恢复正常的现象；电压中断是指一相或多相电压瞬时降低到0.1p.u.以下，且持续时间为10ms~1min。电压暂
降、暂升的电压有效值的变化范围大于±10%。参考于《电能质量 电压暂降与短时中断》 GB/T 30137-2013。*************/

// p.u.是一个标幺值（per unit）的缩写。
//比如，220v=1p.u. ，那么0.1个p.u.就是22v

void LOGIC_zansheng_zanjiang_zhongduan(void)
{

    for (uint8_t i = 0; i < 10; i++) {
//A
        if (ad_RMS[0][i] > VOL_STD_RMS_A) {
            if (((ad_RMS[0][i] - VOL_STD_RMS_A) > 0.1 * VOL_STD_RMS_A) && // >1.1p.u.
                ((ad_RMS[0][i] - VOL_STD_RMS_A) < 0.8 * VOL_STD_RMS_A)) { // <1.8p.u.
                //todo: 波形存储发送  暂升
            }
        }
        else if (ad_RMS[0][i] < VOL_STD_RMS_A) {
            if (((VOL_STD_RMS_A - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_A) && // <0.9p.u.
                ((VOL_STD_RMS_A - ad_RMS[0][i]) < 0.9 * VOL_STD_RMS_A)) { // >0.1p.u.
                //todo: 波形存储发送   暂降
            }
            if (((VOL_STD_RMS_A - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_A)) {
                //todo: 波形存储发送   中断
            }
        }


//B  
        if (ad_RMS[1][i] > VOL_STD_RMS_B) {
            if (((ad_RMS[1][i] - VOL_STD_RMS_B) > 0.1 * VOL_STD_RMS_B) &&
                ((ad_RMS[1][i] - VOL_STD_RMS_B) < 0.8 * VOL_STD_RMS_B)) {
                //波形存储发送    暂升
            }
        }
        else if (ad_RMS[01][i] < VOL_STD_RMS_B) {
            if (((VOL_STD_RMS_B - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_B) &&
                ((VOL_STD_RMS_B - ad_RMS[1][i]) < 0.9 * VOL_STD_RMS_B)) {
                //波形存储发送     暂降
            }

            if (((VOL_STD_RMS_B - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_B)) {
                //波形存储发送    中断
            }
        }


//C  
        if (ad_RMS[2][i] > VOL_STD_RMS_C) {
            if (((ad_RMS[2][i] - VOL_STD_RMS_C) > 0.1 * VOL_STD_RMS_C) &&
                ((ad_RMS[2][i] - VOL_STD_RMS_C) < 0.8 * VOL_STD_RMS_C)) {
                //波形存储发送  暂升
            }
        }
        else if (ad_RMS[2][i] < VOL_STD_RMS_C) {
            if (((VOL_STD_RMS_C - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_C) &&
                ((VOL_STD_RMS_C - ad_RMS[2][i]) < 0.9 * VOL_STD_RMS_C)) {
                //波形存储发送    暂降
            }
            if (((VOL_STD_RMS_C - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_C)) {
                //波形存储发送   中断
            }
        }
    }
}


/*********************电压偏差********************************/
/******************压偏差强调的是实际电压偏离系统标称电压的数值
，与偏差持续的时间无关。参考于《电能质量 供电电压偏差》
GB/T 12325-2008************************************************/
double Data_deviation[3][2];
double Data_deviation1[3][2];
// [:][1] for value, [:][0] for sign
void LOGIC_voltage_deviation(void)
{

    for (uint8_t i = 0; i < 10; i++) {
        Data_deviation[0][1] = ad_RMS[0][i] + Data_deviation[0][1];
        Data_deviation[1][1] = ad_RMS[1][i] + Data_deviation[1][1];
        Data_deviation[2][1] = ad_RMS[2][i] + Data_deviation[2][1];
    }
    Data_deviation1[0][1] = Data_deviation[0][1] / 10;
    Data_deviation1[1][1] = Data_deviation[1][1] / 10;
    Data_deviation1[2][1] = Data_deviation[2][1] / 10;

    Data_deviation[0][1] = 0;
    Data_deviation[1][1] = 0;
    Data_deviation[2][1] = 0;
    //A
    if (Data_deviation1[0][1] > VOL_STD_RMS_A)  //正偏差
    {
        Data_deviation[0][1] = (Data_deviation1[0][1] - VOL_STD_RMS_A)
                                / VOL_STD_RMS_A;
        Data_deviation[0][0] = 1;
    }
    else if (Data_deviation1[0][1] < VOL_STD_RMS_A)//负偏差
    {
        Data_deviation[0][1] = (VOL_STD_RMS_A - Data_deviation1[0][1])
                                / VOL_STD_RMS_A;
        Data_deviation[0][0] = -1;
    }
    //B
    if (Data_deviation1[1][1] > VOL_STD_RMS_B)  //正偏差
    {
        Data_deviation[1][1] = (Data_deviation1[1][1] - VOL_STD_RMS_B)
                                / VOL_STD_RMS_B;
        Data_deviation[0][0] = 1;
    }
    else if (Data_deviation1[1][1] < VOL_STD_RMS_B)//负偏差
    {
        Data_deviation[1][1] = (VOL_STD_RMS_B - Data_deviation1[1][1])
                                / VOL_STD_RMS_B;
        Data_deviation[0][0] = -1;
    }
    //C
    if (Data_deviation1[2][1] > VOL_STD_RMS_C)  //正偏差
    {
        Data_deviation[2][1] = (Data_deviation1[2][1] - VOL_STD_RMS_C)
                                / VOL_STD_RMS_C;
        Data_deviation[0][0] = 1;
    }
    else if (Data_deviation1[2][1] < VOL_STD_RMS_C)//负偏差
    {
        Data_deviation[2][1] = (VOL_STD_RMS_C - Data_deviation1[2][1])
                                / VOL_STD_RMS_C;
        Data_deviation[0][0] = -1;
    }
    // 加个标志位
}



/*********************电压波动********************************/
/*电压波动：电压方均根值一系列相对快速变动或连续改变的现象，
IEEE中给出的典型电压波动范围为0.1%~7%，变化频率小于25Hz，
电压的有效值的变化范围小于±10%。可参考《电能质量 电压波动与闪变》
GB/T 12326-2008。**********************************************/
void GET_LESSTHAN25Hz_data(void)//20ms取256个点，50分之一就是1秒取256个点进行FFT
{

    if (0x01 == AD_save_flag1) {
        Harmonic_calculation_FFT(flicker_03_dataupper, A_line);//谐波计算 a
        Harmonic_calculation_FFT(flicker_02_dataupper, B_line);//谐波计算 b
        Harmonic_calculation_FFT(flicker_01_dataupper, C_line);//谐波计算 c
    }
    if (0x00 == AD_save_flag1) {
        Harmonic_calculation_FFT(flicker_03_datalower, A_line);//谐波计算 a
        Harmonic_calculation_FFT(flicker_02_datalower, B_line);//谐波计算 b
        Harmonic_calculation_FFT(flicker_01_datalower, C_line);//谐波计算 c
    }

}

/********************电压闪变***************************/
//
//void LOGIC_voltage_flicker(float B0, float B1, float B2)
//{
//    ;// too complex to calculate
//}



/*********谐波及峰值计算***************************************


void Get_50HZ_data(void)
{ uint16_t logic_a_data=0,logic_b_data=0,logic_c_data=0;

 Get_FFT_DATA(AD_03_data);

 Get_FFT_DATA(AD_02_data);

 Get_FFT_DATA(AD_01_data);

}
*/

/***********************相位及频率计算********************************/


uint16_t TIME_A[3] = {0},
         TIME_A1[3] = {0},
         TIME_B[3] = {0},
         TIME_B1[3] = {0},
         TIME_C[3] = {0},
         TIME_C1[3] = {0};

// [:][1] increases itself in tim7 routine
uint16_t TIME_DIFF[3][2] = {0};
// after [:][0] is set, it increases itself in tim7 interrupt routine
uint16_t TIME_DIFF_PHASE[3][2] = {0};

uint16_t TIMER_BUFFER[3] = {0}; /* 数据采集次计数变量 */
//uint16_t  TIM7_CNT_OLD, TIM7_CNT_NOW, TIM7_UPDATE_CNT;
float freq_tab[3];
float phase_tab_ang[3];
float phase_tab_rad[3];
static uint8_t
    TIMER_TEMP = 0,
    TIMER_PEMP[3][2] = {0};

static uint16_t phase[3][10] = {0},
                TIME_DIFF1[3][3][10] = {0};

static uint16_t TIME_DIFF_PHASE_DATA[3][2] = {0},
                TIME_DIFF_DATA[3][2] = {0};

static uint32_t aassd = 0;
volatile uint8_t deviation_diff = 200;

/********************数据计算***************************************/

void data_process(void)
{
    if (0x01 == AD_TEMP_flag) {
        AD_TEMP_flag = 0;
        if (0x01 == AD_save_flag) {
            Harmonic_calculation_FFT(AD_03_dataupper, A_line);
            Harmonic_calculation_FFT(AD_02_dataupper, B_line);
            Harmonic_calculation_FFT(AD_01_dataupper, C_line);
        }
        if (0x00 == AD_save_flag) {
            Harmonic_calculation_FFT(AD_03_datalower, A_line);
            Harmonic_calculation_FFT(AD_02_datalower, B_line);
            Harmonic_calculation_FFT(AD_01_datalower, C_line);
        }

        HAL_Delay(1);
        if (0x01 == AD_save_flag)
            LOGIC_Voltage_RMS_value(AD_01_dataupper,
                AD_02_dataupper, AD_03_dataupper);
        if (0x00 == AD_save_flag)
            LOGIC_Voltage_RMS_value(AD_01_datalower,
                AD_02_datalower, AD_03_datalower);
        //计算后会存储到ad_RMS里面
        HAL_Delay(1);
        LOGIC_zansheng_zanjiang_zhongduan(); //电压暂升暂降 短时中断
        LOGIC_voltage_deviation();                 //电压偏差
    }

    if (0x01 == flicker_flag)              //闪变
    {
        flicker_flag = 0;
        GET_LESSTHAN25Hz_data();
        HAL_Delay(1);
    }
}

