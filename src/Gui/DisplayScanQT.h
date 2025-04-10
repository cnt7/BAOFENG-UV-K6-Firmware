#ifndef __DISPLAYSCANQT_H
    #define __DISPLAYSCANQT_H

extern void QTScanDisplayStatus(U8 status);
extern void QTScanDisplay(U8 ctsDcsType, U32 dat, U8 isStandard);
extern void QTScanDisplayHome(void);

#endif
