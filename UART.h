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
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded. 
volatile char GottenString[STRINGLENGTH];

volatile int speed = 0;


void init_UART1()                  //initialize of UART1
{
    U1MODEbits.UARTEN = 0;         //switch off UART for a moment
    U1MODE = 0;                    //Turn off all ports
    U1BRG = 25;                    //Baudrate 9600         
    U1MODEbits.BRGH = 0;           //High speed disabled
    U1STA = 0;                     //Control register 
    U1MODEbits.UARTEN = 1;         //Turn on UART
    U1STAbits.UTXEN = 1;           //Allow to send data
    IEC0bits.U1RXIE = 1;     //get data interrupt allowed
    IFS0bits.U1RXIF = 0;     //null flag of getting data 
    RPINR18bits.U1RXR = 32;        //Set RP32 as receiver (RX)RF12   
    RPOR15bits.RP31R = 3;          //Set RPI31 as transiver (TX)RF13
}

char getCharUART1()                //get character from UART 1
{
    char rv_simv;                  //
    while(U1STAbits.URXDA == 0);   //waiting for data
    rv_simv = U1RXREG;             //we have a data take it from the register
    IFS0bits.U1RXIF = 0;           //we get the data so we need to null the flag
    return rv_simv;                //return the data
}
int charToInt()
{
    int Result = 0;
    int i;
    short sym = 1;
    short Offset = 0;
    if(GottenString[0]=='-')
        {
            sym = -1;
            Offset = 1;
        }
    for(i=0;i<STRINGLENGTH;i++)
    {
        
        Result+=(GottenString[i+Offset]-'0')*pow(10,(STRINGLENGTH-1)-i);
    }
    
    return Result*sym;
}

int getStringUART1()
{
    short cnt = 0;
    short off = 0;
    while(cnt<STRINGLENGTH+off)
    {
        GottenString[cnt]=getCharUART1();
        if(GottenString[0]=='-')
            off=1;
        cnt++;
    }
   return charToInt(); 
}


void sendCharUART1(char tr_simv)   //Sending data to UART 1
{
    while (U1STAbits.UTXBF == 1);       //waiting for buffer allowance
    U1TXREG = tr_simv;                  //send character
}


void sendInt(int Num)
{
    char buffer[8];
    sprintf(buffer, "%d", Num);
    char *p_buffer = buffer;
    while(*p_buffer)
    {
        sendCharUART1(*p_buffer++);
    }
    
//    char buff[10]={'f','f','f','f','f','f','f','f','f','f'};
//    char time;
//    if(Num<0)
//    {
//       sendCharUART1('-');
//       Num*=-1;
//    }
//    if(Num==0)
//    {
//        sendCharUART1('0');
//        return;
//    }
//    while(Num)
//    {
//        cnt++;
//        time=Num%10+'0';
//        buff[cnt] = time;
//        Num=Num/10;
//    }
//
//    while(cnt>=0)
//    {
//        sendCharUART1(buff[cnt]);
//        cnt--;
//    }
}

void sendString(char* string)
{
    while(*string)
    {
        sendCharUART1(*string);
        string++;
    }
}


void __attribute__ ((interrupt, auto_psv)) _U1RXInterrupt(void)
{
    speed = getStringUART1();
    sendInt(speed);
    
}
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UART_H */

