#ifndef __ALG_FINDEXTREMA_H__
#define __ALG_FINDEXTREMA_H__
extern void findExtrema(
    double *src, int src_len, double distance,
    int *idx_max_tab, int *max_quantity,
    int *idx_min_tab, int *min_quantity);
#endif // !__ALG_FINDEXTREMA_H__