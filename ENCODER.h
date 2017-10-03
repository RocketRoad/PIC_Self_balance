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
#ifndef ENCODER_H
#define	ENCODER_H

#include <xc.h> // include processor files - each processor file is guarded.  

volatile int EncLeft = 0;
volatile int EncRight = 0;

volatile short LeftPrev = 0;
volatile short RightPrev = 0;

void init_ENC()
{
   
    TRISGbits.TRISG8 = 1;               // pin RG8 (CN10) set as input - Hall sensor #1
    CNPU1bits.CN10PUE = 0;              // internal Pull-up resistor is disEnable for pin RG8
    CNPD1bits.CN10PDE = 1;              // internal Pull-down resistor is Enable for pin RG8
    CNEN1bits.CN10IE = 1;               // Change Notification on CN10 pin RG8 is Enable
    
    LeftPrev = PORTGbits.RG8;
    RightPrev = PORTGbits.RG6;
    TRISGbits.TRISG6 = 1;
    CNPU1bits.CN8PUE = 0;
    CNPD1bits.CN8PDE = 1;
    CNEN1bits.CN8IE = 1;
    
    IEC1bits.CNIE = 1;                  // Interrupt for ICN is Enable
    IFS1bits.CNIF = 0;                  // Clear Flag for CN
    
    
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    if(PORTGbits.RG8!=LeftPrev)
    {
        if(PORTGbits.RG8==PORTGbits.RG9)
            EncRight++;
        else
            EncRight--;
        LeftPrev=!LeftPrev;
    }
    if(PORTGbits.RG6!=RightPrev)
    {
        if(PORTGbits.RG6==PORTGbits.RG7)
            EncLeft--;
        else
            EncLeft++;
        RightPrev=!RightPrev;
    }
    IFS1bits.CNIF = 0; 
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ENCODER_H */

