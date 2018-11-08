#include "power_quality.h"
#include "math.h"
#include "fft.h"
#include "cmsis_os.h"

extern uint8_t AD_save_flag, AD_save_flag1;

extern uint16_t AD_01_dataupper[2560];  //C
extern uint16_t AD_01_datalower[2560];  //C
extern uint16_t AD_02_dataupper[2560];  //B
extern uint16_t AD_02_datalower[2560];  //B
extern uint16_t AD_03_dataupper[2560];  //A
extern uint16_t AD_03_datalower[2560];  //A

extern uint16_t flicker_01_dataupper[128];  //C
extern uint16_t flicker_01_datalower[128];  //C
extern uint16_t flicker_02_dataupper[128];  //B
extern uint16_t flicker_02_datalower[128];  //B
extern uint16_t flicker_03_dataupper[128];  //A
extern uint16_t flicker_03_datalower[128];  //A
extern uint8_t AD_TEMP_flag, flicker_flag;//, flicker_Falg1;

extern float fft_harmonicA_output[]; //FFT 输出数组
extern float fft_harmonicB_output[]; //FFT 输出数组
extern float fft_harmonicC_output[]; //FFT 输出数组




uint16_t AD_RMS_Time = 0;

double AD_data_sum[3];

double ad_RMS[3][10] = {0}; // 全局数组，三相电压均方根数组

/********************电压有效值************************************/
/* 每个周期（20ms）采集128个点，这样的话，所需定时器的频率为128*50Hz=6400Hz，即为TIM4频率 */
void Voltage_RMS_Calc(uint16_t ad3[], uint16_t ad2[], uint16_t ad1[], float buf[])
{
    float tmp[3] = {0};
    uint16_t i = 0, j = 0, k = 0;
    for (j = 0; j < 10; j++) 
    {
        k = j * 128;
        for (i = 0; i < 128; i++) 
        {
            tmp[0] = (float) *(ad1 + k + i); // A
            tmp[1] = (float) *(ad2 + k + i); // B
            tmp[2] = (float) *(ad3 + k + i); // C

            AD_data_sum[0] += pow(tmp[0], 2);
            AD_data_sum[1] += pow(tmp[1], 2);
            AD_data_sum[2] += pow(tmp[2], 2);
        }
        ad_RMS[0][j] = sqrt(AD_data_sum[0] / 128);
        ad_RMS[1][j] = sqrt(AD_data_sum[1] / 128);
        ad_RMS[2][j] = sqrt(AD_data_sum[2] / 128);
        for(i=0; i<3; i++)
        {
            AD_data_sum[i] = 0;
        }
    }

    // convert AD value to real voltage
    for(j=0; j<10; j++)
    {
        for(i=0; i<3; i++)
        {
            /* internal ref voltage & offset-binary code */
            ad_RMS[i][j] = ((ad_RMS[i][j] > 32768.0) ? (ad_RMS[i][j] - 32768.0) : 0)
            * 5.0 / 32768.0;
        }
    }
    for(i=0; i<3; i++)
    {
        buf[i] = 0;
        for(j=0; j<10; j++)
        {
            buf[i] += ad_RMS[i][j];
        }
        buf[i] /= 10.0;
        buf[i] *= VOL_STD_FACTOR;
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
    for (uint8_t i = 0; i < 10; i++) 
    {
        //A
        if (ad_RMS[0][i] > VOL_STD_RMS_A) 
        {
            if (((ad_RMS[0][i] - VOL_STD_RMS_A) > 0.1 * VOL_STD_RMS_A) && // >1.1p.u.
                ((ad_RMS[0][i] - VOL_STD_RMS_A) < 0.8 * VOL_STD_RMS_A))  // <1.8p.u.
            {
                __nop();
                //todo: 根据AD_save_flag，进行波形存储发送  暂升
            }
        }
        else if (ad_RMS[0][i] < VOL_STD_RMS_A) 
        {
            if (((VOL_STD_RMS_A - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_A) && // <0.9p.u.
                ((VOL_STD_RMS_A - ad_RMS[0][i]) < 0.9 * VOL_STD_RMS_A)) // >0.1p.u.
            { 
                //todo: 波形存储发送   暂降
                __nop();
            }
            if (((VOL_STD_RMS_A - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_A)) 
            {
                //todo: 波形存储发送   中断
                __nop();
            }
        }
        //B  
        if (ad_RMS[1][i] > VOL_STD_RMS_B) 
        {
            if (((ad_RMS[1][i] - VOL_STD_RMS_B) > 0.1 * VOL_STD_RMS_B) &&
                ((ad_RMS[1][i] - VOL_STD_RMS_B) < 0.8 * VOL_STD_RMS_B)) 
            {
                //波形存储发送    暂升
                __nop();
            }
        }
        else if (ad_RMS[01][i] < VOL_STD_RMS_B) 
        {
            if (((VOL_STD_RMS_B - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_B) &&
                ((VOL_STD_RMS_B - ad_RMS[1][i]) < 0.9 * VOL_STD_RMS_B)) 
            {
                //波形存储发送     暂降
                __nop();
            }

            if (((VOL_STD_RMS_B - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_B)) 
            {
                //波形存储发送    中断
                __nop();
            }
        }
        //C  
        if (ad_RMS[2][i] > VOL_STD_RMS_C) 
        {
            if (((ad_RMS[2][i] - VOL_STD_RMS_C) > 0.1 * VOL_STD_RMS_C) &&
                ((ad_RMS[2][i] - VOL_STD_RMS_C) < 0.8 * VOL_STD_RMS_C)) 
            {
                //波形存储发送  暂升
                __nop();
            }
        }
        else if (ad_RMS[2][i] < VOL_STD_RMS_C) 
        {
            if (((VOL_STD_RMS_C - ad_RMS[0][i]) > 0.1 * VOL_STD_RMS_C) &&
                ((VOL_STD_RMS_C - ad_RMS[2][i]) < 0.9 * VOL_STD_RMS_C)) 
            {
                //波形存储发送    暂降
                __nop();
            }
            if (((VOL_STD_RMS_C - ad_RMS[0][i]) < 0.1 * VOL_STD_RMS_C)) 
            {
                //波形存储发送   中断
                __nop();
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
void Voltage_Deviation_Calc(void)
{

    for (uint8_t i = 0; i < 10; i++) {
        Data_deviation[0][1] += ad_RMS[0][i];
        Data_deviation[1][1] += ad_RMS[1][i];
        Data_deviation[2][1] += ad_RMS[2][i];
    }
    Data_deviation1[0][1] /= 10;
    Data_deviation1[1][1] /= 10;
    Data_deviation1[2][1] /= 10;

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

float freq_tab[3];
float phase_tab_ang[3];
float phase_tab_rad[3];


/********************数据计算***************************************/
float rms_tab[3];
void data_process(void)
{
    if (0x01 == AD_TEMP_flag) {
        AD_TEMP_flag = 0;
        // if (0x01 == AD_save_flag) {
        //     Harmonic_calculation_FFT(AD_03_dataupper, A_line);
        //     Harmonic_calculation_FFT(AD_02_dataupper, B_line);
        //     Harmonic_calculation_FFT(AD_01_dataupper, C_line);
        // }
        // if (0x00 == AD_save_flag) {
        //     Harmonic_calculation_FFT(AD_03_datalower, A_line);
        //     Harmonic_calculation_FFT(AD_02_datalower, B_line);
        //     Harmonic_calculation_FFT(AD_01_datalower, C_line);
        // }

        // HAL_Delay(1);
        if (0x01 == AD_save_flag)
            Voltage_RMS_Calc(AD_01_dataupper,
                AD_02_dataupper, AD_03_dataupper, rms_tab);
        if (0x00 == AD_save_flag)
            Voltage_RMS_Calc(AD_01_datalower,
                AD_02_datalower, AD_03_datalower, rms_tab);
        // HAL_Delay(1);
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

