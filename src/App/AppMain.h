#ifndef __APPMAIN_H
    #define __APPMAIN_H

extern U8 Sidekey_GetRemapEvent(U8 event);
extern void SideKey_Process(U8 realEvent);
extern void KeyProcess_Main(U8 keyEvent);

#endif
