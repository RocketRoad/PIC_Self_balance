#include <xc.h>
#include "ALLINIT.h"
#define gyro_address 0b1101000

char rawBuffer[6] = {1,1,1,1,1,1};
int AccelX = 0;
int AccelY = 0;
int AccelZ = 0;
int GyroX = 0;
int GyroY = 0;
int GyroZ = 0;
float angle_acc = 0;
float LeftMotorControl = 0;
float RightMotorControl = 0;
int gyro_yaw_calibration_value = 0;
int gyro_pitch_calibration_value = 0;
float angle_gyro = 0;
float pRegPhi = 500;
float dRegPhi = 0.1;

float saturation(float Number, float Number1, float Number2)
{
    if(Number>Number2)
    {
        return Number2;
    }
    if(Number<Number1)
    {
        return Number1;
    }
    return Number;
    
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) //
{
    i2c_read_bytes(gyro_address,0x3B,6,rawBuffer);
    //AccelX = (((int)rawBuffer[0])<<8)|(rawBuffer[1] & 0xff);
    //AccelY = (((int)rawBuffer[2])<<8)|(rawBuffer[3] & 0xff);
    AccelZ = (((int)rawBuffer[4])<<8)|(rawBuffer[5] & 0xff);
    i2c_read_bytes(gyro_address,0x43,6,rawBuffer);
    GyroX = (((int)rawBuffer[0])<<8)|(rawBuffer[1] & 0xff);
    GyroY = (((int)rawBuffer[2])<<8)|(rawBuffer[3] & 0xff);
    //GyroZ = (((int)rawBuffer[4])<<8)|(rawBuffer[5] & 0xff);
    AccelZ = saturation(AccelZ,-8192,8192);
    angle_acc = asin((float)AccelZ/8192.0)* 57.296;
    GyroX -= gyro_yaw_calibration_value; 
    GyroY -= gyro_pitch_calibration_value;
    angle_gyro += GyroY * 0.000031;
    
    angle_gyro = angle_gyro * 0.999 + angle_acc * 0.001;
    //sendInt(angle_gyro);
    LeftMotorControl = -angle_gyro*pRegPhi-GyroY*dRegPhi;
    RightMotorControl = -angle_gyro*pRegPhi-GyroY*dRegPhi;
    motorControlL(LeftMotorControl);
    motorControlR(RightMotorControl);
    
    
    
    
    
    
    
    
  
    
    IFS0bits.T2IF = 0;
    
}

void main() 
{
    int i = 0;
    init_PWM();
    init_UART1();
    init_ENC();       
    //init_ADC();
    TRISAbits.TRISA7 = 0;
    LATAbits.LATA7 = 0;
    i2c_init();
    i2c_write_byte(gyro_address,0x6B,0x00);
    i2c_write_byte(gyro_address,0x1B,0x00);
    i2c_write_byte(gyro_address,0x1C,0x08);
    i2c_write_byte(gyro_address,0x1A,0x03);
    for(i = 0;i<500;i++)
    {
    i2c_read_bytes(gyro_address,0x43,6,rawBuffer);
    gyro_yaw_calibration_value += (((int)rawBuffer[0])<<8)|(rawBuffer[1] & 0xff);
    gyro_pitch_calibration_value += (((int)rawBuffer[2])<<8)|(rawBuffer[3] & 0xff);
    __delay_us(3700);
    }
    gyro_yaw_calibration_value /= 500;
    gyro_pitch_calibration_value /= 500;
    LATAbits.LATA7 = 1;
    
    do
    {
    i2c_read_bytes(gyro_address,0x3B,6,rawBuffer);
    AccelZ = (((int)rawBuffer[4])<<8)|(rawBuffer[5] & 0xff);
    AccelZ = saturation(AccelZ,-8192,8192);
    angle_acc = asin((float)AccelZ/8192.0)* 57.296;
    }while(angle_acc > 0.5 || angle_acc <-0.5);
    
    angle_gyro = angle_acc;
    
    
    IEC0bits.T2IE = 1;  
    while(1)
    {
        
    };
}