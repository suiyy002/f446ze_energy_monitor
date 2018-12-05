#include "power_quality.h"
#include "math.h"
#include "fft.h"
#include "cmsis_os.h"
#include "alg_findextrema.h"
#include "alg_sort.h"

#define FLT_EPSINON 1e-7f
extern uint8_t AD_save_flag, AD_save_flag1;

extern uint16_t AD_Data_A_0[128];  //C
extern uint16_t AD_Data_A_1[128];  //C
extern uint16_t AD_Data_B_0[128];  //B
extern uint16_t AD_Data_B_1[128];  //B
extern uint16_t AD_Data_C_0[128];  //A
extern uint16_t AD_Data_C_1[128];  //A

// extern uint16_t flicker_ADdata_A_0[128];
// extern uint16_t flicker_ADdata_A_1[128];
// extern uint16_t flicker_ADdata_B_0[128];
// extern uint16_t flicker_ADdata_B_1[128];
// extern uint16_t flicker_ADdata_C_0[128];
// extern uint16_t flicker_ADdata_C_1[128]; 


/********************电压有效值************************************/
/* 每个周期（20ms）采集128个点，这样的话，所需定时器的频率为128*50Hz=6400Hz，即为TIM4频率 */
/* 保证实时计算还是要放到adc驱动里面，或者放到中断里面 */
double ad_RMS[3] = {0};         // 三相电压均方根数组
double rms_avg[2][3] = {0};     // 均值, Voltage_Deviation_Calc
uint8_t flg_rms_avg = 0;
uint16_t rms_cnt = 0;
float rms_fluct[2][3][512] = {0};         /* 三相rms离散数据 */
volatile uint8_t rms_fluct_flg = 0;
uint16_t idx_rms_fluct = 0;             /* 三相rms离散数据索引 */
volatile uint8_t flg_rms_fluct_valid = 0;
void Voltage_RMS_Calc(uint16_t ad1[128], uint16_t ad2[128], uint16_t ad3[128])
{
    double AD_data_sum[3] = {0};
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

    if(!(idx_rms_fluct < 512))
    {
        idx_rms_fluct = 0;
        rms_fluct_flg ^= 1;
        flg_rms_fluct_valid = 1;
    }

    // convert AD value to real voltage
    for(i = 0; i < 3; i++)
    {
        /* internal ref voltage & offset-binary code */
        ad_RMS[i] = ((ad_RMS[i] > 32768.0) ? (ad_RMS[i] - 32768.0) : 0) * 5.0 / 32768.0;
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
        flg_rms_avg = 1;
        rms_cnt = 0;
        for(i = 0; i < 3; i++)
        {
            rms_avg[1][i] = rms_avg[0][i] / 10;
            rms_avg[0][i] = 0;
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
double Data_deviation[3] = {0};
void Voltage_Deviation_Calc(void)
{
     if(!flg_rms_avg)
         return;
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

    flg_rms_avg = 0;
}

/* calculate flicker and voltage fluctuation */
#if 1 /* 折叠代码、注释代码用，挺方便的 */
    /* 每个周期都进行rms计算，采样512次得到数组rms_fluct[3][512]，
    使用极值寻找算法寻找相邻极值，计算电压波动 */

    double fluctuation[3] = {0};            /* 电压波动数据，每5.12s更新一次 */
    /* 找极值 */
    uint16_t min_idx_tab[3][512] = {0}; /* distance是50，所以元素肯定小于12个 */
    uint16_t max_idx_tab[3][512] = {0};
    uint16_t max_qty[3] = {0}, min_qty[3] = {0};
    double Udiff_max[3] = {FLT_EPSINON, FLT_EPSINON, FLT_EPSINON}; // 所有极值差中最大的
    uint16_t idx_flickermeter_response_freq[36] = /* N=512, res=0.09765Hz */
    {
        6,
        11,
        15,
        21,
        26,
        31,
        36,
        41,
        46,
        51,
        56,
        62,
        67,
        72,
        77,
        82,
        90,
        97,
        103,
        108,
        113,
        118,
        123,
        133,
        143,
        154,
        164,
        174,
        184,
        195,
        205,
        215,
        225,
        236,
        246,
        256
    };// 频率位置
    double sinusoidal_voltage_flickermeter_response[36] = 
    {
        2.325,
        1.397,
        1.067,
        0.879,
        0.747,
        0.645,
        0.564,
        0.497,
        0.442,
        0.396,
        0.357,
        0.325,
        0.3,
        0.28,
        0.265,
        0.256,
        0.25,
        0.254,
        0.261,
        0.271,
        0.283,
        0.298,
        0.314,
        0.351,
        0.393,
        0.438,
        0.486,
        0.537,
        0.59,
        0.646,
        0.704,
        0.764,
        0.828,
        0.894,
        0.964,
        1.037
    };//频响
    extern float fft_flicker_out_A[512];
    extern float fft_flicker_out_B[512];
    extern float fft_flicker_out_C[512];
    double flicker_S[2][3][60] = {0}; // 瞬时闪变值数组
    int cnt_flicker_S = 0;
    int flg_flicker_S = 0;
    // int qsort_cmp( const void *a , const void *b ){return *(double *)a > *(double *)b ? 1 : -1;}
    uint16_t flg_Pst_valid = 0;
    uint16_t cnt_Pst = 0;
    double flicker_Pst[3] = {0};
    double flicker_Pst_tab[3][12] = {0};
    uint16_t flg_Plt_valid = 0;
    double flicker_Plt[3] = {0};
    void Voltage_Fluctuation_and_Flicker_Calc(void)
    {
        if(!flg_rms_fluct_valid)
            return;
        // 寻找极值
        for(uint8_t j = 0; j < 3; j++) /* 遍历三相数组 */
        {
            findExtrema(rms_fluct[!rms_fluct_flg][j], 512, 50, 
               max_idx_tab[j], &max_qty[j], min_idx_tab[j], &min_qty[j], j);
            #if 0
                /* 问号表达式作用是取相邻极值 */
                for(uint8_t i = 0; i < (max_qty[j] > min_qty[j] ? min_qty[j] : max_qty[j]); i++)
                { /* 遍历极值索引数组 */
                    double tmp = rms_fluct[!rms_fluct_flg][j][max_idx_tab[j][i]] 
                        - rms_fluct[!rms_fluct_flg][j][min_idx_tab[j][i]];
                    Udiff_max[j] = (tmp > Udiff_max[j]) ? tmp : Udiff_max[j];
                    if(Udiff_max[j] > 1)
                        __nop();
                }
            #endif
        }
        #if 1
            // 开始计算电压波动，百分比表示
            for(uint8_t i = 0; i < 3; i++)
            {
                fluctuation[i] = Udiff_max[i] / VOL_STD_RMS_A * 100;
            }
            // fft
            fft_flicker_calc(rms_fluct[!rms_fluct_flg][0], A_line);
            fft_flicker_calc(rms_fluct[!rms_fluct_flg][1], B_line);
            fft_flicker_calc(rms_fluct[!rms_fluct_flg][2], C_line);
            // 求解瞬时闪变值
            if(!(cnt_flicker_S < 60))
            {
                cnt_flicker_S = 0;
                flg_flicker_S ^= 1;
                if(!(cnt_Pst < 12))
                {
                    cnt_Pst = 0;
                    for(uint8_t i = 0; i < 3; i++)
                    {
                        for(uint8_t j = 0; j < 12; j++)
                        {
                            flicker_Plt[i] += pow(flicker_Pst_tab[i][j], 3); /* 立方和，求平均 */
                        }
                        flicker_Plt[i] /= 12;
                        flicker_Plt[i] = pow(flicker_Plt[i], 1.0 / 3); /* 开立方 */
                    }
                    flg_Plt_valid = 1; /* 长时闪变计算完成 */ 
                }
                for(uint8_t i = 0; i < 3; i++)
                {
                    // qsort(flicker_S[!flg_flicker_S][i], 60, sizeof(double), qsort_cmp);
                    bubble_sort_f64(flicker_S[!flg_flicker_S][i], 60);
                    /* 论文公式 */
                    flicker_Pst[i] = 0.0314 * flicker_S[!flg_flicker_S][i][1]
                                + 0.0525 * flicker_S[!flg_flicker_S][i][1]
                                + 0.0657 * flicker_S[!flg_flicker_S][i][2]
                                + 0.2800 * flicker_S[!flg_flicker_S][i][6]
                                + 0.0800 * flicker_S[!flg_flicker_S][i][30];
                    flicker_Pst_tab[i][cnt_Pst] = flicker_Pst[i];
                }
                flg_Pst_valid = 1;
                cnt_Pst++;
            }
            for(uint8_t i = 0; i < 36; i++) // 查表求解
            {
                flicker_S[flg_flicker_S][0][cnt_flicker_S] /* 0代表A相 */
                += pow(
                    (fft_flicker_out_A[idx_flickermeter_response_freq[i]] / fft_flicker_out_A[0])
                    / sinusoidal_voltage_flickermeter_response[i],
                    2);
            }
            for(uint8_t i = 0; i < 36; i++)
            {
                flicker_S[flg_flicker_S][1][cnt_flicker_S] /* 1代表B相 */
                += pow(
                    (fft_flicker_out_B[idx_flickermeter_response_freq[i]] / fft_flicker_out_B[0])
                    / sinusoidal_voltage_flickermeter_response[i],
                    2);
            }
            for(uint8_t i = 0; i < 36; i++)
            {
                flicker_S[flg_flicker_S][2][cnt_flicker_S] /* 2代表C相 */
                += pow(
                    (fft_flicker_out_C[idx_flickermeter_response_freq[i]] / fft_flicker_out_C[0])
                    / sinusoidal_voltage_flickermeter_response[i],
                    2);
            }
        #endif
        cnt_flicker_S++;
        flg_rms_fluct_valid = 0;
    }
#endif

/***********************相位及频率计算********************************/
// 在mian.c的中断里面计算
float freq_tab[3];
float phase_tab_ang[3];
float phase_tab_rad[3];

/* 谐波参数计算 */
extern uint8_t AD_har_flg;
extern float fft_harmonicA_output[]; //FFT 输出数组
extern float fft_harmonicB_output[]; //FFT 输出数组
extern float fft_harmonicC_output[]; //FFT 输出数组
float HRUh[3][50] = {0}; // 第h次谐波电压含有率 49次谐波 
float UH[3] = {0}; // 谐波电压含量
float THDu[3] = {0}; //总谐波畸变率
void Voltage_Harmonic_Calc(void)
{
    if(AD_har_flg) /* 10个周波一次 */
    {
        if (0x01 == AD_save_flag) 
        {
            fft_harmonic_calc(AD_Data_A_0, A_line);
            fft_harmonic_calc(AD_Data_B_0, B_line);
            fft_harmonic_calc(AD_Data_C_0, C_line);
        }
        if (0x00 == AD_save_flag) 
        {
            fft_harmonic_calc(AD_Data_A_1, A_line);
            fft_harmonic_calc(AD_Data_B_1, B_line);
            fft_harmonic_calc(AD_Data_C_1, C_line);
        }
        for(uint8_t i = 1; i < 49; i++)
        {
            HRUh[0][i] = fft_harmonicA_output[i] / fft_harmonicA_output[0] * 100;
            UH[0] += pow(HRUh[0][i], 2);
        }
        UH[0] = pow(UH[0], 0.5);
        THDu[0] = UH[0] / fft_harmonicA_output[0] * 100;

        for(uint8_t i = 1; i < 49; i++)
        {
            HRUh[1][i] = fft_harmonicB_output[i] / fft_harmonicB_output[0] * 100;
            UH[1] += pow(HRUh[1][i], 2);
        }
        UH[1] = pow(UH[1], 0.5);
        THDu[1] = UH[1] / fft_harmonicB_output[0] * 100;

        for(uint8_t i = 1; i < 49; i++)
        {
            HRUh[2][i] = fft_harmonicC_output[i] / fft_harmonicC_output[0] * 100;
            UH[2] += pow(HRUh[2][i], 2);
        }
        UH[2] = pow(UH[2], 0.5);
        THDu[2] = UH[2] / fft_harmonicC_output[0] * 100;
    }
}

/********************数据计算***************************************/
void data_process(void)
{
    // Voltage_sag_swell_interruption(); //电压暂升暂降 短时中断
    // Voltage_Deviation_Calc();
    Voltage_Harmonic_Calc();
    Voltage_Fluctuation_and_Flicker_Calc();
}
