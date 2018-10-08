#include "Type.h"
#include "p32xxxx.h"
#include <sys/attribs.h>

u16 DC_PR2 = 560;
u16 MAX_PR4 = 550;
u16 DC_PR4 = 400;
u16 DEGRAD = 2;
u16 DIMM_MODE = 0;
void           TP4(void)
{
    __builtin_enable_interrupts();
    TRISFbits.TRISF1 = 0; //  OUTPUT
    LATFbits.LATF1 = 0; // LED HIGH

    //TIMER va desde TMR2 a PR2
    PR2 = 62500;
    TMR2 = 0;
    T2CONbits.ON = 1;       //Encender clock
    T2CONbits.TCKPS = 0b100;    //TCKPS<2:0> 011 = 1:8 prescale value
    T2CONbits.TGATE = 0;
    IFS0bits.T2IF = 0;       //T2IF = watchdog, cuando TMR2 = PR2 -> T2IF = 1; [IMPORTANTE DEVOLVER A CERO]
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 5;


    PR3 = 62500;
    TMR3 = 0;
    T3CONbits.ON = 1;
    T3CONbits.TCKPS = 0b101;
    T3CONbits.TGATE = 0;
    IFS0bits.T3IF = 0; //inicializar valor watchdog T3
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 6;


  //  PR4 = 18000;
    TMR4 = 0;
    T4CONbits.ON = 0;
    T4CONbits.TCKPS = 0b100;
    T4CONbits.TGATE = 0;
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    IPC4bits.T4IP = 4;

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

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL5SRS)TP4InterruptT2(void)
{
    if (!DIMM_MODE)
    {
            // Toggle LED LD1
        if (LATFbits.LATF1)
            LATFbits.LATF1 = 0;
        else
            LATFbits.LATF1 = 1;
    }
    else
    {
        LATFbits.LATF1 = 1;
        if (DC_PR4 < 5 || DC_PR4 > MAX_PR4)
        {
           DEGRAD *= -1;
        }
        DC_PR4 += DEGRAD;
        T4CONbits.ON = 1;
        TMR4 = 0;
        PR4 = DC_PR4;
    }
	// Reset interrupt flag
    IFS0bits.T2IF = 0;
}

void __ISR(_TIMER_4_VECTOR, IPL4SRS)TP4InterruptT4(void)
{
    if (DIMM_MODE)
        LATFbits.LATF1 = 0;
    T4CONbits.ON = 0;
    IFS0bits.T4IF = 0;
}


void __ISR(_EXTERNAL_1_VECTOR , IPL7SRS)TP4ButtonInterrupt(void)
{    
    TMR3 = 0;
    IFS0bits.T3IF = 0;
    if (!DIMM_MODE)
    {
        if(T2CONbits.TCKPS != 0b000)
            (T2CONbits.TCKPS)--;
        else
            T2CONbits.TCKPS = 0b100;
    }
    IFS0bits.INT1IF = 0;
}


void __ISR(_TIMER_3_VECTOR, IPL6SRS)TP4InterruptT3(void)
{
    if (!PORTDbits.RD8 && IFS0bits.T3IF)
    {
        if (DIMM_MODE == 0)
        {
          DIMM_MODE = 1;
          T4CONbits.ON = 1;
          PR2 = DC_PR2;
          TMR2 = 0;
          PR4 = DC_PR4;
          TMR4 = 0;
          T2CONbits.TCKPS = 0b100;
        }
        else
        {
          T4CONbits.ON = 0;
          DIMM_MODE = 0;
          PR2 = 62500;
        }
    }
      IFS0bits.T3IF = 0;
}