/*
 * File:   assignment_1.c
 * Author: nancy-masa
 *
 * Created on 21 ????, 2024, 10:33 ?
 */

// PIC18F4620 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.





#include <xc.h>
#include <stdio.h>
#include "my_ser.h"
#define _XTAL_FREQ   4000000UL 
#define RELAY1 PORTCbits.RC0
#define RELAY2 PORTEbits.RE0
#define COOLER PORTCbits.RC2
#define HEATER PORTCbits.RC5

void initPorts(void)
{
    ADCON0 = 0;
    ADCON1 = 0b00001011; //4 analog channels, change this according to your application

    PORTA = PORTB = PORTC = PORTD = PORTE = 0x00;
    LATA  = LATB  = LATC = LATD  = LATE  = 0x00;
    TRISD = TRISC =TRISE= 0x00;
    TRISB = 0xFF;
    
}
void delay_ms(unsigned int n)
{
    int i;
    for (i=0; i < n; i++){
         __delaywdt_ms(1) ; 
    }
}
void sprintf_binary(char *buffer, unsigned char value) {
    for (int i = 7; i >= 0; i--) {
        buffer[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    buffer[8] = '\0'; 
}
void main(void) {
    
    INTCON = 0;
    INTCON2 =0;
    CLRWDT();
    initPorts();
    setupSerial();
    send_string_no_lib((unsigned char *)"Nancy-Masa\r\n");
   RELAY1 = COOLER = HEATER = RELAY2 =0;
   delay_ms(250);
   PORTD=0x00;
    delay_ms(250);
    char x;
    while (1) {
        
        CLRWDT();
        if (PORTBbits.RB0==0) {  PORTD=0x00; RELAY1 = COOLER = HEATER = RELAY2 =0; delay_ms(250); }
        if (PORTBbits.RB1==0) { HEATER=0; COOLER = 1; delay_ms(250); }
        if (PORTBbits.RB2==0) { HEATER=1; COOLER = 0; delay_ms(250); }
        
        if (PORTBbits.RB3==0) {  PORTDbits.RD0=!PORTDbits.RD0;  PORTDbits.RD1=!PORTDbits.RD1;   PORTDbits.RD6=!PORTDbits.RD6; PORTDbits.RD7=!PORTDbits.RD7; delay_ms(250);}
        if (PORTBbits.RB4==0) {  PORTDbits.RD2=!PORTDbits.RD2;  PORTDbits.RD3=!PORTDbits.RD3;   PORTDbits.RD4=!PORTDbits.RD4; PORTDbits.RD5=!PORTDbits.RD5;delay_ms(250);}
        if (PORTBbits.RB5==0) { RELAY1 = !RELAY1; RELAY2 = !RELAY2;   delay_ms(250);}
        //1. Receiving a Character from Serial
		//checks if a byte is available on the serial buffer to be read.
   if(is_byte_available()){
            x = read_byte_no_lib();// is used to read the byte (
								  //character in this context) from the serial buffer.
            send_string_no_lib((unsigned char *)"\r\n");
			//2. Checking the Character
            if( x == 'G' ){ 
                if (HEATER)send_string_no_lib((unsigned char *)"Heater : ON \r\n"); 
                else send_string_no_lib((unsigned char *)"Heater : OFF \r\n");
                
                if (COOLER)send_string_no_lib((unsigned char *)"COOLER : ON \r\n");
                else send_string_no_lib((unsigned char *)"COOLER : OFF \r\n");
                
                if (RELAY1)send_string_no_lib((unsigned char *)"RELAY1 : ON \r\n");
                else send_string_no_lib((unsigned char *)"RELAY1 : OFF \r\n");
                 if (RELAY2)send_string_no_lib((unsigned char *)"RELAY2 : ON \r\n");
                else send_string_no_lib((unsigned char *)"RELAY2 : OFF \r\n");
            }
            else if (x == 'g'){
                unsigned char portDStatus = PORTD; 
                 char hexStr[3];  
                  char binaryStr[9]; 
                   sprintf(hexStr, "%02X", portDStatus);
                   sprintf_binary(binaryStr, portDStatus);
                     send_string_no_lib((unsigned char *)"LEDs Hex: 0x");
                     send_string_no_lib((unsigned char *)hexStr);
                     send_string_no_lib((unsigned char *)" \r\nLEDs Bin: 0b");
                     send_string_no_lib((unsigned char *)binaryStr);
                     send_string_no_lib((unsigned char *)"\r\n");
                
            }
            else if(x == 'h'){HEATER=0; COOLER=1; delay_ms(250);}
            else if(x == 'H'){HEATER=1; COOLER=0; delay_ms(250);}
            else if(x == 'c'){RELAY1=0;delay_ms(250);}
            else if(x == 'C'){RELAY1=1; delay_ms(250);}
            else if(x == 'r'){RELAY2=0;delay_ms(250);}
            else if(x == 'R'){RELAY2=1;delay_ms(250);}
			//to toggle LEDs 
            else if(x == 'd'){PORTDbits.RD0=!PORTDbits.RD0;  PORTDbits.RD1=!PORTDbits.RD1;   PORTDbits.RD6=!PORTDbits.RD6; PORTDbits.RD7=!PORTDbits.RD7; delay_ms(250);}
            else if(x == 'D'){PORTDbits.RD2=!PORTDbits.RD2;  PORTDbits.RD3=!PORTDbits.RD3;   PORTDbits.RD4=!PORTDbits.RD4; PORTDbits.RD5=!PORTDbits.RD5; delay_ms(250);}
            //Reset Everything to Default
			else if(x == 'z'){PORTD=0x00;RELAY1 = COOLER = HEATER = RELAY2 = 0; delay_ms(250);}
            
            else {
                send_string_no_lib((unsigned char *)"THERE IS NO CHOICE LIKE THIS\r\n");
            }
        }
    }
    return;
}