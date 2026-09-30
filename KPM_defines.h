#ifndef __KPM_DEFINES_H__
#define __KPM_DEFINES_H__

#include <lpc214x.h>
#include "types.h"
#include "delay.h"

#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19

#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

u8 kpmLUT[4][4] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};


void Init_KPM(void)
{
    IODIR1 |= 15<<ROW0;
}


u32 ColScan(void)
{
    if(((IOPIN1>>COL0)&15)<15)
        return 0;
    else
        return 1;
}


u32 RowCheck(void)
{
    u32 rno;

    for(rno=0; rno<4; rno++)
    {
        IOPIN1=(IOPIN1&~(15<<ROW0))|((~(1<<rno))<<ROW0);

        if(ColScan()==0)
            break;
    }

    IOCLR1=15<<ROW0;

    return rno;
}


u32 ColCheck(void)
{
    u32 cno;

    for(cno=0; cno<4; cno++)
    {
        if(((IOPIN1>>(COL0+cno))&1)==0)
            break;
    }

    return cno;
}


u32 KeyScan(void)
{
    u32 rno,cno,key;

    while(ColScan());
    
	delay_ms(20);

    rno=RowCheck();
    cno=ColCheck();

    key=kpmLUT[rno][cno];

    while(!ColScan());
    delay_ms(20);

    return key;
}


u8 key;

u32 ReadNum(void)
{
    u32 sum=0;

    while(1)
    {
        key=KeyScan();

        if(key>='0' && key<='9')
        {
            sum=(sum*10)+(key-'0');
        }
        else
            break;
    }

    return sum;
}

#endif
