#include "Type.h"
#include "p32xxxx.h"

/*
 DOCS
 PIC32	    http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
 TIMER	    http://ww1.microchip.com/downloads/en/devicedoc/61105f.pdf
 OSCILAtOR  http://ww1.microchip.com/downloads/en/devicedoc/61112h.pdf
 */

static void    speed_modify(u8 *flag)
{
    if (PORTDbits.RD8 == 0 && *flag)
    {
        if(T2CONbits.TCKPS != 0b000)
            (T2CONbits.TCKPS)--;
        else
            T2CONbits.TCKPS = 0b100;
        *flag = 0;
    }
    else if (PORTDbits.RD8 == 1)
        *flag = 1;
}

void           TP2(void)
{
    u8 flag = 0;
    TRISFbits.TRISF1 = 0;
    LATFbits.LATF1 = 0;
    PR2 = 62500;
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    //Encender clock
    T2CONbits.ON = 1;

    //MUX1
    T2CONbits.TGATE = 0;
    //T2CONbits.TCS = 1;

    //TCKPS<2:0> 011 = 1:8 prescale value
    T2CONbits.TCKPS = 0b100;

    while (1)
    {
        speed_modify(&flag);
	if (IFS0bits.T2IF)
	{
	    if (LATFbits.LATF1)
		LATFbits.LATF1 = 0;
	    else
		LATFbits.LATF1 = 1;
	    IFS0bits.T2IF = 0;
	}
    }

}
