#include "power_quality.h"
#include "math.h"
#include "fft.h"
#include "cmsis_os.h"
#include "alg_findextrema.h"
extern uint8_t AD_save_flag, AD_save_flag1;

extern uint16_t AD_Data_A_0[128];  //C
extern uint16_t AD_Data_A_1[128];  //C
extern uint16_t AD_Data_B_0[128];  //B
extern uint16_t AD_Data_B_1[128];  //B
extern uint16_t AD_Data_C_0[128];  //A
extern uint16_t AD_Data_C_1[128];  //A

extern uint16_t flicker_ADdata_A_0[128];
extern uint16_t flicker_ADdata_A_1[128];
extern uint16_t flicker_ADdata_B_0[128];
extern uint16_t flicker_ADdata_B_1[128];
extern uint16_t flicker_ADdata_C_0[128] ;
extern uint16_t flicker_ADdata_C_1[128]; 

extern uint8_t AD_TEMP_flag, flicker_flag;//, flicker_Falg1;

extern float fft_harmonicA_output[]; //FFT 输出数组
extern float fft_harmonicB_output[]; //FFT 输出数组
extern float fft_harmonicC_output[]; //FFT 输出数组



/********************电压有效值************************************/
/* 每个周期（20ms）采集128个点，这样的话，所需定时器的频率为128*50Hz=6400Hz，即为TIM4频率 */
/* 保证实时计算还是要放到ad驱动里面，或者放到中断里面 */
double ad_RMS[3] = {0};         // 三相电压均方根数组
double rms_avg[2][3] = {0};     // 均值, Voltage_Deviation_Calc
uint16_t rms_cnt = 0;
double rms_fluct[2][3][512] = {0};         /* 三相rms离散数据 */
uint8_t rms_fluct_flg = 0;
uint16_t idx_rms_fluct = 0;             /* 三相rms离散数据索引 */
void Voltage_RMS_Calc(uint16_t ad1[128], uint16_t ad2[128], uint16_t ad3[128])
{
    double AD_data_sum[3];
    float tmp[3] = {0};
    uint8_t i = 0;
    for (; i < 128; i++) 
    {
        tmp[0] = (float) *(ad1 + i); // A
        tmp[1] = (float) *(ad2 + i); // B
        tmp[2] = (float) *(ad3 + i); // C

        AD_data_sum[0] += pow(tmp[0], 2);
        AD_data_sum[1] += pow(tmp[1], 2);
        AD_data_sum[2] += pow(tmp[2], 2);
    }
    ad_RMS[0] = sqrt(AD_data_sum[0] / 128);
    ad_RMS[1] = sqrt(AD_data_sum[1] / 128);
    ad_RMS[2] = sqrt(AD_data_sum[2] / 128);
    for(i = 0; i < 3; i++)
    {
        AD_data_sum[i] = 0;
    }
    if(!(idx_rms_fluct < 512))
    {
        idx_rms_fluct = 0;
        rms_fluct_flg ^= 1;
    }

    // convert AD value to real voltage
    for(i = 0; i < 3; i++)
    {
        /* internal ref voltage & offset-binary code */
        ad_RMS[i] = ((ad_RMS[i] > 32768.0) ? (ad_RMS[i] - 32768.0) : 0)
        * 5.0 / 32768.0;
        /* 存一个点 */
        rms_fluct[rms_fluct_flg][i][idx_rms_fluct] = ad_RMS[i];
    }
    idx_rms_fluct++;
    /* 每10次数据计算一次平均 */
    if(rms_cnt < 10)
    {
        for(i = 0; i < 3; i++)
        {
            rms_avg[0][i] += ad_RMS[i];
        }
        rms_cnt++;
    }
    else
    {
        rms_cnt = 0;
        for(i = 0; i < 3; i++)
        {
            rms_avg[1][i] = rms_avg[0][i] / 10;
        }
    }
}


/********************电压暂降、暂升：电压暂降是指电力系统中某点工频电压方均根值暂时降低至0.1~0.9p.u.，并在
短时持续10ms~1min后恢复正常的现象；电压暂升是指电力系统中某点工频方均根值暂时上升至1.1~1.8p.u.，并在短时持
续10ms~1min后恢复正常的现象；电压中断是指一相或多相电压瞬时降低到0.1p.u.以下，且持续时间为10ms~1min。电压暂
降、暂升的电压有效值的变化范围大于±10%。参考于《电能质量 电压暂降与短时中断》 GB/T 30137-2013。*************/

// p.u.是一个标幺值（per unit）的缩写。
//比如，220v=1p.u. ，那么0.1个p.u.就是22v

void Voltage_sag_swell_interruption(void)
{
    //A
    if (ad_RMS[0] > VOL_STD_RMS_A) 
    {
        if (((ad_RMS[0] - VOL_STD_RMS_A) > 0.1 * VOL_STD_RMS_A) && // >1.1p.u.
            ((ad_RMS[0] - VOL_STD_RMS_A) < 0.8 * VOL_STD_RMS_A))  // <1.8p.u.
        {
            __nop();
            //todo: 根据AD_save_flag，进行波形存储发送  暂升
        }
    }
    else if (ad_RMS[0] < VOL_STD_RMS_A) 
    {
        if (((VOL_STD_RMS_A - ad_RMS[0]) > 0.1 * VOL_STD_RMS_A) && // <0.9p.u.
            ((VOL_STD_RMS_A - ad_RMS[0]) < 0.9 * VOL_STD_RMS_A)) // >0.1p.u.
        { 
            //todo: 波形存储发送   暂降
            __nop();
        }
        if (((VOL_STD_RMS_A - ad_RMS[0]) < 0.1 * VOL_STD_RMS_A)) 
        {
            //todo: 波形存储发送   中断
            __nop();
        }
    }
    //B  
    if (ad_RMS[1] > VOL_STD_RMS_B) 
    {
        if (((ad_RMS[1] - VOL_STD_RMS_B) > 0.1 * VOL_STD_RMS_B) &&
            ((ad_RMS[1] - VOL_STD_RMS_B) < 0.8 * VOL_STD_RMS_B)) 
        {
            //波形存储发送    暂升
            __nop();
        }
    }
    else if (ad_RMS[1] < VOL_STD_RMS_B) 
    {
        if (((VOL_STD_RMS_B - ad_RMS[0]) > 0.1 * VOL_STD_RMS_B) &&
            ((VOL_STD_RMS_B - ad_RMS[1]) < 0.9 * VOL_STD_RMS_B)) 
        {
            //波形存储发送     暂降
            __nop();
        }

        if (((VOL_STD_RMS_B - ad_RMS[0]) < 0.1 * VOL_STD_RMS_B)) 
        {
            //波形存储发送    中断
            __nop();
        }
    }
    //C  
    if (ad_RMS[2] > VOL_STD_RMS_C) 
    {
        if (((ad_RMS[2] - VOL_STD_RMS_C) > 0.1 * VOL_STD_RMS_C) &&
            ((ad_RMS[2] - VOL_STD_RMS_C) < 0.8 * VOL_STD_RMS_C)) 
        {
            //波形存储发送  暂升
            __nop();
        }
    }
    else if (ad_RMS[2] < VOL_STD_RMS_C) 
    {
        if (((VOL_STD_RMS_C - ad_RMS[0]) > 0.1 * VOL_STD_RMS_C) &&
            ((VOL_STD_RMS_C - ad_RMS[2]) < 0.9 * VOL_STD_RMS_C)) 
        {
            //波形存储发送    暂降
            __nop();
        }
        if (((VOL_STD_RMS_C - ad_RMS[0]) < 0.1 * VOL_STD_RMS_C)) 
        {
            //波形存储发送   中断
            __nop();
        }
    }
}


/*********************电压偏差********************************/
/******************压偏差强调的是实际电压偏离系统标称电压的数值
，与偏差持续的时间无关。参考于《电能质量 供电电压偏差》
GB/T 12325-2008************************************************/
double Data_deviation[3];
void Voltage_Deviation_Calc(void)
{

    for (uint8_t i = 0; i < 10; i++) 
    {
        Data_deviation[0] += rms_avg[1][0];
        Data_deviation[1] += rms_avg[1][1];
        Data_deviation[2] += rms_avg[1][2];
    }

    Data_deviation[0] /= 10;
    Data_deviation[1] /= 10;
    Data_deviation[2] /= 10;

    Data_deviation[0] = (Data_deviation[0] - VOL_STD_RMS_A) / VOL_STD_RMS_A;
    Data_deviation[1] = (Data_deviation[1] - VOL_STD_RMS_B) / VOL_STD_RMS_B;
    Data_deviation[2] = (Data_deviation[2] - VOL_STD_RMS_C) / VOL_STD_RMS_C;
}


#if 0
    /*********************电压波动********************************/
    /*电压波动：电压方均根值一系列相对快速变动或连续改变的现象，
    IEEE中给出的典型电压波动范围为0.1%~7%，变化频率小于25Hz，
    电压的有效值的变化范围小于±10%。可参考《电能质量 电压波动与闪变》
    GB/T 12326-2008。**********************************************/
    /* 采样点数为128，采样频率为6400/50Hz, Fn = (n-1)Hz，所以FFT计算出的数组只需要前25个数据 */
    void GET_LESSTHAN25Hz_data(void)
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
#endif

/* calculate flicker and voltage fluctuation */
#if 1 
    /* 每个周期都进行rms计算，采样512次得到数组rms_fluct[3][512]，
    使用极值寻找算法寻找相邻极值，计算电压波动 */

    double fluctuation[3] = {0};            /* 电压波动数据，每5.12s更新一次 */
    void calc_fluctuation_and_flicker(void)
    {
        
        
    }
#endif



/***********************相位及频率计算********************************/

float freq_tab[3];
float phase_tab_ang[3];
float phase_tab_rad[3];


/********************数据计算***************************************/
void data_process(void)
{
    if (0x01 == AD_TEMP_flag) 
    {
        AD_TEMP_flag = 0;
        if (0x01 == AD_save_flag) 
        {
            Harmonic_calculation_FFT(AD_Data_A_0, A_line);
            Harmonic_calculation_FFT(AD_Data_B_0, B_line);
            Harmonic_calculation_FFT(AD_Data_C_0, C_line);
        }
        if (0x00 == AD_save_flag) 
        {
            Harmonic_calculation_FFT(AD_Data_A_1, A_line);
            Harmonic_calculation_FFT(AD_Data_B_1, B_line);
            Harmonic_calculation_FFT(AD_Data_C_1, C_line);
        }
        if (0x01 == AD_save_flag)
            Voltage_RMS_Calc(AD_Data_A_0, AD_Data_B_0, AD_Data_C_0);
        if (0x00 == AD_save_flag)
            Voltage_RMS_Calc(AD_Data_A_1, AD_Data_B_1, AD_Data_C_1);
        // Voltage_sag_swell_interruption(); //电压暂升暂降 短时中断
        // Voltage_Deviation_Calc();                 //电压偏差
    }

    // if (0x01 == flicker_flag)              //闪变
    // {
    //     flicker_flag = 0;
    //     GET_LESSTHAN25Hz_data();
    //     HAL_Delay(1);
    // }
}

