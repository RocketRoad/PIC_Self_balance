/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  

void init_PWM ()                  //Init_PWM
{
    TMR2 = 0;                     //Startting value of timer 2
    PR2 =  15999;                  //End value of timer 2
    T2CONbits.TCKPS = 0b00;       //Prescaler 1:2  
    T2CONbits.TON = 1;            //Start timer 2 
    CLKDIVbits.RCDIV = 0b000;     //FRC Postscaler bits (1:1)
    
    RPOR11bits.RP22R = 18;        //Output compare 1
    RPOR11bits.RP23R = 19;        //Output compare 2
    RPOR10bits.RP20R = 20;
    RPOR12bits.RP25R = 21;
    
    
    OC1R = 0;                     //Current value of compare 1
    OC1RS = PR2;                  //Finish value of compare 1
    OC2R = 0;                     //Current value of compare 2
    OC2RS = PR2;                  //Finish value of compare 2
    OC3R = 0;
    OC3RS = PR2;
    OC4R = 0;
    OC4RS = PR2;
    
    
    
    OC1CON2bits.SYNCSEL = 0b01100;//sync source of output compare 1
    OC1CON2bits.OCTRIG = 0;       
    OC1CON1bits.OCTSEL = 0;       //Select timer 2
    
    
    OC2CON2bits.SYNCSEL = 0b01100;//sync source
    OC2CON2bits.OCTRIG = 0;       
    OC2CON1bits.OCTSEL = 0; 
    
    OC3CON2bits.SYNCSEL = 0b01100;//sync source
    OC3CON2bits.OCTRIG = 0;       
    OC3CON1bits.OCTSEL = 0; 
    
    OC4CON2bits.SYNCSEL = 0b01100;//sync source
    OC4CON2bits.OCTRIG = 0;       
    OC4CON1bits.OCTSEL = 0; 
    
    
    IEC0bits.T2IE = 0;            //Interrupt from overflowing of timer 2 disabled
    IFS0bits.T2IF = 0;
    IEC0bits.OC1IE = 0;             //Interrupt from compare disabled
    IEC0bits.OC2IE = 0;
    IEC1bits.OC3IE = 0;
    IEC1bits.OC4IE = 0;
    
    
    IEC0bits.U1RXIE = 0;          
    OC1CON1bits.OCM = 0b110; //Edge-aligned PWM 
    OC2CON1bits.OCM = 0b110;
    OC3CON1bits.OCM = 0b110;
    OC4CON1bits.OCM = 0b110;
}

void motorControlL(float speed)
{
    ///(A-1A-RD2)(A-1B-RD3)
    speed*=16;
    if(speed<0)
    {
        OC1R=(int)(-speed+0.5);
        OC2R=0;
    }
    else
    {
        OC2R = (int)(speed+0.5);
        OC1R = 0;
    }
}
void motorControlR(float speed)
{
    ///(B-1A-RD5)(B-1B-RD4)
    speed*=16;
    if(speed<0)
    {
        OC4R=(int)(-speed+0.5);
        OC3R=0;
    }
    else
    {
        OC3R = (int)(speed+0.5);
        OC4R = 0;
    } 
}
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PWM_H */

