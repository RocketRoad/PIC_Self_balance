// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H
#include<xc.h>
void init_ADC()                   //Initialize ADC
{
    AD1PCFGL = 0;            //Analog input 

    AD1PCFGLbits.PCFG0 = 0;      
    /**/
    AD1CON2bits.VCFG = 0b000;     //reference voltage AVdd AVss
    AD1CON3bits.ADCS = 0;         
    
    AD1CSSL = 0;                  //don't scan the ports
    AD1CON3bits.SAMC = 0b11111;   //Capacitor charging time
    AD1CON1bits.SSRC = 0b111;     // (auto-convert) Internal counter ends sampling 
                                  // and starts conversion 
    AD1CON1bits.FORM = 0b00;      //Output form
    AD1CON1bits.ADON = 1;         //Start an ADC
}

int read_ADC(int channel)         //Reading value from ADC
{
    AD1CHS = channel;             //Select channel
    AD1CON1bits.SAMP = 1;         //Start conversion
    while(!AD1CON1bits.DONE);     //Wait until end of conversion
    return ADC1BUF0;              //Obvious
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ADC_H */

