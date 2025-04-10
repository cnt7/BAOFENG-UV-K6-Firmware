#ifndef __RADIOTASK_H
    #define __RADIOTASK_H

extern void RfOff(void);
extern void RxReset(void);
extern void CalculateSqlLevel(void);
extern void RF_RxEnd(void);
extern void RF_TxEnd(void);
extern void RF_TxRoger(void);
extern void Radio_EnterTxMode(void);
extern void RF_Task(void);

#endif
