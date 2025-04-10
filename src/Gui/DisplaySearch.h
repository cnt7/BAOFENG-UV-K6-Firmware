#ifndef __DISPLAYSEARCH_H
    #define __DISPLAYSEARCH_H

typedef enum
{
    STEP_SEEK_FREQ, STEP_SEEK_CTSDCS,
} ENUM_SEARCHFREQ_STEP;

extern void SearchFreqModeDisplayDCSData(U8 ctsDcsType, U32 dat, U8 isStandard);
extern void SearchFreqModeDisplayStepMsg( ENUM_SEARCHFREQ_STEP step );
extern void SearchFreqDisplayHome(void);

#endif
