#ifndef __FUNCTIONS_H
    #define __FUNCTIONS_H
    
extern void ResetInputBuf(void);
extern void ExitAllFunction(U8 flag);
extern void CheckAutoKeyLockTask(void);
extern void KeyLockFunSwitch(void);
extern void DualModeFastSwitch(void);
extern void ChDisModeFastSwitch(void);
extern void VoxSwitchOnOff(void);
extern void VoxCheckTask(void);
extern void Radio_TxPowerSWitch(void);
extern void Radio_TxKeyTone(U8 event, U8 para);

#endif
