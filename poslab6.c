/*
 * File:   poslab6.c
 * Author: aldoa
 *
 * Created on 7 de noviembre de 2022, 05:29 PM
 */


#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define _XTAL_FREQ 8000000

uint8_t ADC;
int flag;
char dato[57]={'e','s','c','o','j','a',32,'u','n','a',32,'o','p','c','i','o','n',10,13,
                '1',32,'l','e','e','r',32,'p','o','t','e','n','c','i','o','m','e','t','r','o',10,13,
                '2',32,'e','n','v','i','a','r',32,'a','s','c','i','i',10,13};
char index = 0;

void setup(void);
void initUART(void);
void setupINTOSC(void);
void setupADC(void);


//******************************************************************************
// CÃ³digo Principal
//******************************************************************************
void main(void) {
    
    setup();
    setupINTOSC();         // 8 MHz
    initUART();
    setupADC();
    
    
    while(1){
        
        ADCON0bits.GO = 1;  // enciendo la bandera
        while(ADCON0bits.GO == 1){
            ;
        }
        ADIF = 0;           // apago la bandera
        ADC = ADRESH;
        
        PORTB++;
        if(TXSTAbits.TRMT == 1){
        //TXREG = PORTB+48;
        TXREG=dato[index];
        index++;

        }
        if(index>57)
            index=0;

        if(PIR1bits.RCIF == 1){
        PORTD = RCREG;
        PIR1bits.RCIF = 0;
        }
        __delay_ms(100);
    }
}
//******************************************************************************
// FunciÃ³n para configurar GPIOs
//******************************************************************************
void setup(void){
    ANSELH = 0;
    ANSEL = 0;
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
}

void setupINTOSC(void){
    OSCCONbits.IRCF = 0b111;       // 500 KHz
    OSCCONbits.SCS = 1;
}
//******************************************************************************
// FunciÃ³n para configurar UART
//******************************************************************************
void initUART(void){
    // Paso 1: configurar velocidad baud rate
    
    SPBRG = 12;
    
    // Paso 2:
    
    TXSTAbits.SYNC = 0;         // Trabajaremos de forma asincrona
    RCSTAbits.SPEN = 1;         // habilitamos mÃ³dulo UART
    
    // Paso 3:
    // Saltamos porque no usamos 9 bits
    
    // Paso 4:
    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    
    PIR1bits.TXIF = 0;
    
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion
      
}

void setupADC(void){
    
    // Paso 1 Seleccionar puerto de entrada
    //TRISAbits.TRISA0 = 1;
    TRISA = TRISA | 0x01;
    ANSEL = ANSEL | 0x01;
    
    // Paso 2 Configurar mÃ³dulo ADC
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;       // Fosc/ 8
    
    ADCON1bits.VCFG1 = 0;       // Ref VSS
    ADCON1bits.VCFG0 = 0;       // Ref VDD
    
    ADCON1bits.ADFM = 0;        // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        // Canal AN0
    
    ADCON0bits.ADON = 1;        // Habilitamos el ADC
    __delay_us(100);
}

