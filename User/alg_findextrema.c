/* 参考 https://blog.csdn.net/zone53/article/details/77942341 */
// #include <stdio.h>
#include "stdlib.h"
#include "FreeRTOS.h"
// #define MALLOC malloc
// #define FREE free
int cmp( const void *a , const void *b )
{
    return *(double *)a > *(double *)b ? 1 : -1;
}
/* wrapper for malloc */
void *malloc(size_t size)
{
    return pvPortMalloc(size);
}
void free(void *ptr)
{
    vPortFree(ptr);
}
/*
    * 函数:    findExtrema
    * 参数:    *src             原始数据数组
    *          src_len          原始数据数组数组长度
    *          distance         最大相邻极小/大值之间的距离
    *          *idx_max_tab     找到的极大值的index数组
    *          *max_qty    极大值个数
    *          *idx_min_tab     找到的极小值的index数组
    *          *min_qty    极小值个数
*/
// double src_sorted[12]={0};
// int sign[12];
// int tmp_max_idx[12];
// int out_of_range_tab[12];
// int flag_extrema_idx[12];
// double diff = 0;
// int idx_max_tab[12];
// int idx_min_tab[12];
// double src[12] = {0, -1, 0, 1, 0, 1, 0, -2, 2, 0, 1, 1};
void findExtrema(
    double *src, int src_len, double distance,
    int *idx_max_tab, int *max_qty,
    int *idx_min_tab, int *min_qty)
{
    // 开辟一段空间，记录差值符号信息
    int *sign = (int *)malloc(src_len * sizeof(int));
    // 记录极值位置的数组的索引
    int max_idx = 0,
        min_idx = 0;
    *max_qty = 0;
    *min_qty = 0;
    double *src_sorted = (double *)malloc(src_len * sizeof(double));
    for(int i = 0; i < src_len; i++)
    {
        src_sorted[i] = src[i];
    }
    qsort(src_sorted, src_len, sizeof(double), cmp);
    // 后值减前值，差值为负记作 -1，为零记作 0，为正记作 1
    for (int i = 1; i < src_len; i++)
    {
        double diff = src[i] - src[i - 1];
        if (diff > 0)
            sign[i - 1] = 1;
        else if (diff < 0)
            sign[i - 1] = -1;
        else
            sign[i - 1] = 0;
    }
    for (int j = 1; j < src_len - 1; j++)
    {
        double diff = sign[j] - sign[j - 1];
        if (diff < 0)
            idx_max_tab[max_idx++] = j;// start from j=1, so it's not j-1
        else if (diff > 0)
            idx_min_tab[min_idx++] = j;
    }

    int *flag_extrema_idx = (int *)malloc(sizeof(int) * (max_idx > min_idx ? max_idx : min_idx));
    int *out_of_range_tab = (int *)malloc(sizeof(int) * (max_idx > min_idx ? max_idx : min_idx));
    int *tmp_max_idx = (int *)malloc(sizeof(int) * (max_idx > min_idx ? max_idx : min_idx));
    // 最大值索引
    int extrema = 0, extrema_old = 0;
    double tmp_val = 0;
    int i, j, k;
    //波峰
    for (int i = 0; i < max_idx; i++)
    {
        flag_extrema_idx[i] = 0;
        out_of_range_tab[i] = 0;
    }

    for (i = 0; i < max_idx; i++)
    {
        tmp_val = src_sorted[0];
        /* 捶出一个新最大值*/
        for (j = 0; j < max_idx; j++)
        {
            // 已经剔除的就不用检测了
            if (!flag_extrema_idx[j] && !out_of_range_tab[j])
            {
                if (src[idx_max_tab[j]] > tmp_val)
                {
                    extrema = j;
                    tmp_val = src[idx_max_tab[j]];
                }
            }
        }
        if(extrema == extrema_old)
        {
            break;
        }
        // 记录最大值位置, 下次跳过这个点再找一个新的最大值(伪)
        flag_extrema_idx[extrema] = 1;
        // 当前最大是否被标记剔除
//        if (!out_of_range_tab[extrema])
//        {
            for (k = 0; k < max_idx; k++)
            {
                // 若当前最大值落在某极值的 distance 邻域内, 标记1剔除该极值
//                out_of_range_tab[k] |=
//                    (
//                    idx_max_tab[k] - distance <= idx_max_tab[extrema]
//                    &
//                    idx_max_tab[extrema] <= idx_max_tab[k] + distance
//                    );
                if
                (
                    idx_max_tab[k] - distance <= idx_max_tab[extrema]
                    &
                    idx_max_tab[extrema] <= idx_max_tab[k] + distance
                    &
                    k != extrema
                )
                {
                    out_of_range_tab[k] = 1;
                    src[k] = 0;
                }
            }
            // 修正当前最大值也会被标记的zz bug
//            out_of_range_tab[extrema] = 0;
            // 尽早退出循环用
            extrema_old = extrema;
//        }
    }
    for (i = 0, j = 0; i < max_idx; i++)
    {
        if (!out_of_range_tab[i])
            tmp_max_idx[j++] = idx_max_tab[i];
    }
    for (i = 0; i < max_idx; i++)
    {
        if (i < j)
            idx_max_tab[i] = tmp_max_idx[i];
        else
            idx_max_tab[i] = 0;
    }
    max_idx = j;

    //波谷
    for (int i = 0; i < min_idx; i++)
    {
        flag_extrema_idx[i] = 0;
        out_of_range_tab[i] = 0;
    }
    for (i = 0; i < min_idx; i++)
    {
        tmp_val = src_sorted[src_len - 1];
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
//        if (!out_of_range_tab[extrema])
//        {
            for (k = 0; k < min_idx; k++)
            {
//                out_of_range_tab[k] |=
//                    (
//                        idx_min_tab[k] - distance <= idx_min_tab[extrema]
//                        &
//                        idx_min_tab[extrema] <= idx_min_tab[k] + distance
//                    );
//            }
                if
                (
                    idx_max_tab[k] - distance <= idx_max_tab[extrema]
                    &
                    idx_max_tab[extrema] <= idx_max_tab[k] + distance
                    &
                    k != extrema
                )
                {
                    out_of_range_tab[k] = 1;
                    src[k] = 0;
                }
            }
//            out_of_range_tab[extrema] = 0;
        extrema_old = extrema;
//        }
    }
    for (i = 0, j = 0; i < min_idx; i++)
    {
        if (!out_of_range_tab[i])
            tmp_max_idx[j++] = idx_min_tab[i];
    }
    for (i = 0; i < min_idx; i++)
    {
        if (i < j)
            idx_min_tab[i] = tmp_max_idx[i];
        else
            idx_min_tab[i] = 0;
    }
    min_idx = j;

    *max_qty = max_idx;
    *min_qty = min_idx;

    free(sign);
    free(src_sorted);
    free(flag_extrema_idx);
    free(tmp_max_idx);
    free(out_of_range_tab);
}
// double array[12] = {0, -1, 0, 1, 0, 1, 0, -2, 2, 0, 1, 1};
// int maxima_number = 0, minima_number = 0;
// int maxima_tab[10], minima_tab[10];
// int main()
// {

//     findPeaks(12, 0, &maxima_number, &minima_number);
//     printf("Hello, World!\n");
//     return 0;
// }