#include "Type.h"
#include "p32xxxx.h"

void    TP1(void)
{
    u8 flag = 0;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 1;
    while (1)
    {
        if (flag && PORTDbits.RD8 == 0 && LATFbits.LATF1)
        {
            LATFbits.LATF1 = 0;
            flag = 0;
        }
        else if (flag && PORTDbits.RD8 == 0 && !LATFbits.LATF1)
        {
            LATFbits.LATF1 = 1;
            flag = 0;
        }
        if (PORTDbits.RD8)
            flag = 1;
    }
}
