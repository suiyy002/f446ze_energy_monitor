/* 参考 https://blog.csdn.net/zone53/article/details/77942341 */
#include "alg_sort.h"
#include "alg_findextrema.h"

// const float FLT_EPSINON = 1e-7;
const float FLT_EPSINON = 1e-4;
uint16_t max_pos = 0;
double tmp = 0;
float max_tmp = 0, min_tmp = 0;
float src_sorted[2] = {0};
volatile uint8_t __id = 2;
// float src_bak[10] = {0};
// float diff_bak[10] = {0};
int8_t sign[512] = {0}; /* 应该比src少一个元素，src_len-1，前511个元素有效 */
uint16_t tmp_idx[512] = {0};
uint16_t out_of_range_tab[512] = {0};
uint16_t flag_extrema_idx[512] = {0};
uint16_t max_idx = 0, min_idx = 0; /* 之前不小心把它拿出去了，导致运行一段时间就数组越界卡死 */

void findExtrema
(
    float *src, uint16_t src_len, uint16_t distance,
    uint16_t *idx_max_tab, uint16_t *max_qty,
    uint16_t *idx_min_tab, uint16_t *min_qty, 
    uint8_t id
)
{
    for(uint16_t i = 0; i < 512; i++)
    {
        // src_bak[i] = src[i];
        sign[i] = 0;
        tmp_idx[i] = 0;
        out_of_range_tab[i] = 0;
        flag_extrema_idx[i] = 0;
        idx_max_tab[i] = 0;
        idx_min_tab[i] = 0;
    }
    src_sorted[0] = 0;
    src_sorted[1] = 0;
    max_idx = 0;
    min_idx = 0;
    /* 找出最大值和最小值，作为之后寻找极值的标准 */
    findmax_min_f32(src, src_len, src_sorted);
    /* 后值减前值，差值为负记作 -1，为零记作 0，为正记作 1 */
    for (int i = 1; i < src_len; i++)
    {
        float diff = src[i] - src[i - 1];
        // diff_bak[i] = diff;
        if (diff > FLT_EPSINON)
            sign[i - 1] = 1;
        else if (diff < -FLT_EPSINON)
            sign[i - 1] = -1;
        else if ((diff >= -FLT_EPSINON) && (diff <= FLT_EPSINON))
            sign[i - 1] = 0;
    }
    /* 判断极大值、极小值位置 */
    for (int j = 1; j < src_len - 1; j++) /* sign有效的值个数为src_len-1 */
    {
        float diff = sign[j] - sign[j - 1];
        if (diff < -FLT_EPSINON)
            idx_max_tab[max_idx++] = j;// start from j=1, so it's not j-1
        else if (diff > FLT_EPSINON)
            idx_min_tab[min_idx++] = j;
    }
    /* 最大值索引 */
    int extrema = 0, extrema_old = 555; /* old初始值不能为零 */
    double tmp_val = FLT_EPSINON;
    int i, j, k;
    /* 开始检测极大值是否在范围内 */
    for (i = 0; i < max_idx; i++)
    {
        flag_extrema_idx[i] = 0;
        out_of_range_tab[i] = 0;
    }
    for (i = 0; i < max_idx; i++)
    {
        /* 初始最小值 */
        tmp_val = src_sorted[1];
        /* 遍历极大值列表，捶出一个新最大值*/
        for (j = 0; j < max_idx; j++)
        {
            /* 已经剔除的就不用检测了 */
            if (!flag_extrema_idx[j] && !out_of_range_tab[j])
            {
                if (src[idx_max_tab[j]] > tmp_val)
                {
                    extrema = j;
                    tmp_val = src[idx_max_tab[j]];
                }
            }
        }
        /* 经过上面的遍历比较，如果本次极大值索引等于上次极大值索引，说明应该结束检测 */
        if(extrema == extrema_old)
        {
            break;
        }
        /* 记录最大值位置, 下次跳过这个点再找一个新的最大值(伪) */
        flag_extrema_idx[extrema] = 1;
        /* 检测有无其他极大值落在本次极大值的邻域内，有则剔除邻域内的其他极大值 */
        for (k = 0; k < max_idx; k++)
        {
            if
            (
                idx_max_tab[k] - distance <= idx_max_tab[extrema]
                &
                idx_max_tab[k] + distance >= idx_max_tab[extrema]
                &
                k != extrema /* 可不能剔除自身（笑） */
            )
            {
                out_of_range_tab[k] = 1; /* 剔除记录表 */
//                src[k] = 0; /* 赋零剔除 */
            }
        }
        extrema_old = extrema;
    }
    /* 剔除完成，将剩余在distance范围内的极大值依次存储 */
    for (i = 0, j = 0; i < max_idx; i++)
    {
        if (!out_of_range_tab[i]) /* 未经剔除的值 */
            tmp_idx[j++] = idx_max_tab[i];
    }
    /* 前j个索引值存为极大值，后面的索引值清空 */
    for (i = 0; i < max_idx; i++)
    {
        if (i < j)
            idx_max_tab[i] = tmp_idx[i];
        else
            idx_max_tab[i] = 0;
    }
    /* 极大值个数 */
    max_idx = j;

    /* 开始检测极小值是否在范围内 */
    for (i = 0; i < min_idx; i++)
    {
        flag_extrema_idx[i] = 0;
        out_of_range_tab[i] = 0;
        tmp_idx[i] = 0;
    }
    for (i = 0; i < min_idx; i++)
    {
        /* 初始最大值 */
        tmp_val = src_sorted[0];
        for (j = 0; j < min_idx; j++)
        {
            if (!flag_extrema_idx[j] && !out_of_range_tab[j])
            {
                if (src[idx_min_tab[j]] < tmp_val)
                {
                    extrema = j;
                    tmp_val = src[idx_min_tab[j]];
                }
            }
        }
        if(extrema == extrema_old)
        {
            break;
        }
        flag_extrema_idx[extrema] = 1;
        for (k = 0; k < min_idx; k++)
        {
            if
            (
                idx_min_tab[k] - distance <= idx_min_tab[extrema]
                &
                idx_min_tab[k] + distance >= idx_min_tab[extrema]
                &
                k != extrema
            )
            {
                out_of_range_tab[k] = 1;
//                src[k] = 0;
            }
        }
        extrema_old = extrema;
    }
    for (i = 0, j = 0; i < min_idx; i++)
    {
        if (!out_of_range_tab[i])
            tmp_idx[j++] = idx_min_tab[i];
    }
    for (i = 0; i < min_idx; i++)
    {
        if (i < j)
            idx_min_tab[i] = tmp_idx[i];
        else
            idx_min_tab[i] = 0;
    }
    min_idx = j;

    *max_qty = max_idx;
    *min_qty = min_idx;

    #if 1
        // extern volatile uint8_t rms_fluct_flg;
        // extern float rms_fluct[2][3][512];
        max_pos = 0;
        tmp = 0;
        max_tmp = 0, min_tmp = 0;
        extern double Udiff_max[3];
        Udiff_max[id] = 0;
        if(id == __id)
          __nop();
        for(uint8_t i = 0; i < (max_idx >= min_idx ? min_idx : max_idx); i++)
        {
            tmp = src[idx_max_tab[i]] - src[idx_min_tab[i]];
//            Udiff_max[id] = (tmp > Udiff_max[id]) ? tmp : Udiff_max[id];
            if(tmp > Udiff_max[id] && tmp < 1.0)
            {
                Udiff_max[id] = tmp;
                max_tmp = src[idx_max_tab[i]];
                min_tmp = src[idx_min_tab[i]];
                max_pos = i;
            }
            if(Udiff_max[id] > 1)
                __nop();
        }
        if(id == __id)
          __nop();
    #endif
}

