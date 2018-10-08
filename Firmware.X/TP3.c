#include "Type.h"
#include "p32xxxx.h"
#include <sys/attribs.h>


void           TP3(void)
{
    __builtin_enable_interrupts();
    TRISFbits.TRISF1 = 0; //  OUTPUT
    LATFbits.LATF1 = 0; // LED HIGH

    //TIMER va desde TMR2 a PR2
    PR2 = 62500;
    TMR2 = 0;
    //T2IF = watchdog, cuando TMR2 = PR2 -> T2IF = 1; [IMPORTANTE DEVOLVER A CERO]
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 6;

    //Encender clock
    T2CONbits.ON = 1;
    //TCKPS<2:0> 011 = 1:8 prescale value
    T2CONbits.TCKPS = 0b100;
    T2CONbits.TGATE = 0;
    //MULTIVECTOR CONF
        //MULTIVECTOR ON
        INTCONbits.MVEC = 1;
        //VS bits INTCTL = 32 (decimal)
        //ADDRESS BASE = 0x80000000 (en CPU register window)
        //IV bit 23 de registro CPU Cause ON
        //EXL bit 1 de registro CPU Status OFF
        //BEV bit 22 de registro CPU Status OFF


    IFS0bits.INT1IF = 0;
    IEC0bits.INT1IE = 1;
    IPC1bits.INT1IP = 7;
    INTCONbits.INT1EP = 0;
    while (1);

}

/*void __ISR(_TIMER_2_VECTOR, IPL6SRS)TP3T2Interrupt(void)
{
	// Toggle LED LD1
    if (LATFbits.LATF1)
        LATFbits.LATF1 = 0;
    else
	LATFbits.LATF1 = 1;

	// Reset interrupt flag
    IFS0bits.T2IF = 0;
}

void __ISR(_EXTERNAL_1_VECTOR , IPL7SRS)TP3speedmodify(void)
{
    if(T2CONbits.TCKPS != 0b000)
        (T2CONbits.TCKPS)--;
    else
        T2CONbits.TCKPS = 0b100;
    IFS0bits.INT1IF = 0;
}*/