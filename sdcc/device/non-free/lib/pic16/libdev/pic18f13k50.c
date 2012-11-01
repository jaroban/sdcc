/*
 * This definitions of the PIC18F13K50 MCU.
 *
 * This file is part of the GNU PIC library for SDCC, originally
 * created by Molnar Karoly <molnarkaroly@users.sf.net> 2012.
 *
 * This file is generated automatically by the cinc2h.pl, 2012-09-28 08:20:05 UTC.
 *
 * SDCC is licensed under the GNU Public license (GPL) v2. Note that
 * this license covers the code to the compiler and other executables,
 * but explicitly does not cover any code or objects generated by sdcc.
 *
 * For pic device libraries and header files which are derived from
 * Microchip header (.inc) and linker script (.lkr) files Microchip
 * requires that "The header files should state that they are only to be
 * used with authentic Microchip devices" which makes them incompatible
 * with the GPL. Pic device libraries and header files are located at
 * non-free/lib and non-free/include directories respectively.
 * Sdcc should be run with the --use-non-free command line option in
 * order to include non-free header files and libraries.
 *
 * See http://sdcc.sourceforge.net/ for the latest information on sdcc.
 */

#include <pic18f13k50.h>

//==============================================================================

__at(0x0F53) __sfr UEP0;
__at(0x0F53) volatile __UEP0bits_t UEP0bits;

__at(0x0F54) __sfr UEP1;
__at(0x0F54) volatile __UEP1bits_t UEP1bits;

__at(0x0F55) __sfr UEP2;
__at(0x0F55) volatile __UEP2bits_t UEP2bits;

__at(0x0F56) __sfr UEP3;
__at(0x0F56) volatile __UEP3bits_t UEP3bits;

__at(0x0F57) __sfr UEP4;
__at(0x0F57) volatile __UEP4bits_t UEP4bits;

__at(0x0F58) __sfr UEP5;
__at(0x0F58) volatile __UEP5bits_t UEP5bits;

__at(0x0F59) __sfr UEP6;
__at(0x0F59) volatile __UEP6bits_t UEP6bits;

__at(0x0F5A) __sfr UEP7;
__at(0x0F5A) volatile __UEP7bits_t UEP7bits;

__at(0x0F5B) __sfr UEIE;
__at(0x0F5B) volatile __UEIEbits_t UEIEbits;

__at(0x0F5C) __sfr UADDR;
__at(0x0F5C) volatile __UADDRbits_t UADDRbits;

__at(0x0F5D) __sfr UFRML;
__at(0x0F5D) volatile __UFRMLbits_t UFRMLbits;

__at(0x0F5E) __sfr UFRMH;
__at(0x0F5E) volatile __UFRMHbits_t UFRMHbits;

__at(0x0F5F) __sfr UEIR;
__at(0x0F5F) volatile __UEIRbits_t UEIRbits;

__at(0x0F60) __sfr UIE;
__at(0x0F60) volatile __UIEbits_t UIEbits;

__at(0x0F61) __sfr UCFG;
__at(0x0F61) volatile __UCFGbits_t UCFGbits;

__at(0x0F62) __sfr UIR;
__at(0x0F62) volatile __UIRbits_t UIRbits;

__at(0x0F63) __sfr USTAT;
__at(0x0F63) volatile __USTATbits_t USTATbits;

__at(0x0F64) __sfr UCON;
__at(0x0F64) volatile __UCONbits_t UCONbits;

__at(0x0F68) __sfr SRCON0;
__at(0x0F68) volatile __SRCON0bits_t SRCON0bits;

__at(0x0F69) __sfr SRCON1;
__at(0x0F69) volatile __SRCON1bits_t SRCON1bits;

__at(0x0F6B) __sfr CM2CON0;
__at(0x0F6B) volatile __CM2CON0bits_t CM2CON0bits;

__at(0x0F6C) __sfr CM2CON1;
__at(0x0F6C) volatile __CM2CON1bits_t CM2CON1bits;

__at(0x0F6D) __sfr CM1CON0;
__at(0x0F6D) volatile __CM1CON0bits_t CM1CON0bits;

__at(0x0F6F) __sfr SSPMSK;
__at(0x0F6F) volatile __SSPMSKbits_t SSPMSKbits;

__at(0x0F76) __sfr SLRCON;
__at(0x0F76) volatile __SLRCONbits_t SLRCONbits;

__at(0x0F77) __sfr WPUA;
__at(0x0F77) volatile __WPUAbits_t WPUAbits;

__at(0x0F78) __sfr WPUB;
__at(0x0F78) volatile __WPUBbits_t WPUBbits;

__at(0x0F79) __sfr IOCA;
__at(0x0F79) volatile __IOCAbits_t IOCAbits;

__at(0x0F7A) __sfr IOCB;
__at(0x0F7A) volatile __IOCBbits_t IOCBbits;

__at(0x0F7E) __sfr ANSEL;
__at(0x0F7E) volatile __ANSELbits_t ANSELbits;

__at(0x0F7F) __sfr ANSELH;
__at(0x0F7F) volatile __ANSELHbits_t ANSELHbits;

__at(0x0F80) __sfr PORTA;
__at(0x0F80) volatile __PORTAbits_t PORTAbits;

__at(0x0F81) __sfr PORTB;
__at(0x0F81) volatile __PORTBbits_t PORTBbits;

__at(0x0F82) __sfr PORTC;
__at(0x0F82) volatile __PORTCbits_t PORTCbits;

__at(0x0F89) __sfr LATA;
__at(0x0F89) volatile __LATAbits_t LATAbits;

__at(0x0F8A) __sfr LATB;
__at(0x0F8A) volatile __LATBbits_t LATBbits;

__at(0x0F8B) __sfr LATC;
__at(0x0F8B) volatile __LATCbits_t LATCbits;

__at(0x0F92) __sfr DDRA;
__at(0x0F92) volatile __DDRAbits_t DDRAbits;

__at(0x0F92) __sfr TRISA;
__at(0x0F92) volatile __TRISAbits_t TRISAbits;

__at(0x0F93) __sfr DDRB;
__at(0x0F93) volatile __DDRBbits_t DDRBbits;

__at(0x0F93) __sfr TRISB;
__at(0x0F93) volatile __TRISBbits_t TRISBbits;

__at(0x0F94) __sfr DDRC;
__at(0x0F94) volatile __DDRCbits_t DDRCbits;

__at(0x0F94) __sfr TRISC;
__at(0x0F94) volatile __TRISCbits_t TRISCbits;

__at(0x0F9B) __sfr OSCTUNE;
__at(0x0F9B) volatile __OSCTUNEbits_t OSCTUNEbits;

__at(0x0F9D) __sfr PIE1;
__at(0x0F9D) volatile __PIE1bits_t PIE1bits;

__at(0x0F9E) __sfr PIR1;
__at(0x0F9E) volatile __PIR1bits_t PIR1bits;

__at(0x0F9F) __sfr IPR1;
__at(0x0F9F) volatile __IPR1bits_t IPR1bits;

__at(0x0FA0) __sfr PIE2;
__at(0x0FA0) volatile __PIE2bits_t PIE2bits;

__at(0x0FA1) __sfr PIR2;
__at(0x0FA1) volatile __PIR2bits_t PIR2bits;

__at(0x0FA2) __sfr IPR2;
__at(0x0FA2) volatile __IPR2bits_t IPR2bits;

__at(0x0FA6) __sfr EECON1;
__at(0x0FA6) volatile __EECON1bits_t EECON1bits;

__at(0x0FA7) __sfr EECON2;

__at(0x0FA8) __sfr EEDATA;

__at(0x0FA9) __sfr EEADR;
__at(0x0FA9) volatile __EEADRbits_t EEADRbits;

__at(0x0FAB) __sfr RCSTA;
__at(0x0FAB) volatile __RCSTAbits_t RCSTAbits;

__at(0x0FAC) __sfr TXSTA;
__at(0x0FAC) volatile __TXSTAbits_t TXSTAbits;

__at(0x0FAD) __sfr TXREG;

__at(0x0FAE) __sfr RCREG;

__at(0x0FAF) __sfr SPBRG;

__at(0x0FB0) __sfr SPBRGH;

__at(0x0FB1) __sfr T3CON;
__at(0x0FB1) volatile __T3CONbits_t T3CONbits;

__at(0x0FB2) __sfr TMR3;

__at(0x0FB2) __sfr TMR3L;

__at(0x0FB3) __sfr TMR3H;

__at(0x0FB6) __sfr ECCP1AS;
__at(0x0FB6) volatile __ECCP1ASbits_t ECCP1ASbits;

__at(0x0FB7) __sfr PWM1CON;
__at(0x0FB7) volatile __PWM1CONbits_t PWM1CONbits;

__at(0x0FB8) __sfr BAUDCON;
__at(0x0FB8) volatile __BAUDCONbits_t BAUDCONbits;

__at(0x0FB8) __sfr BAUDCTL;
__at(0x0FB8) volatile __BAUDCTLbits_t BAUDCTLbits;

__at(0x0FB9) __sfr PSTRCON;
__at(0x0FB9) volatile __PSTRCONbits_t PSTRCONbits;

__at(0x0FBA) __sfr REFCON0;
__at(0x0FBA) volatile __REFCON0bits_t REFCON0bits;

__at(0x0FBA) __sfr VREFCON0;
__at(0x0FBA) volatile __VREFCON0bits_t VREFCON0bits;

__at(0x0FBB) __sfr REFCON1;
__at(0x0FBB) volatile __REFCON1bits_t REFCON1bits;

__at(0x0FBB) __sfr VREFCON1;
__at(0x0FBB) volatile __VREFCON1bits_t VREFCON1bits;

__at(0x0FBC) __sfr REFCON2;
__at(0x0FBC) volatile __REFCON2bits_t REFCON2bits;

__at(0x0FBC) __sfr VREFCON2;
__at(0x0FBC) volatile __VREFCON2bits_t VREFCON2bits;

__at(0x0FBD) __sfr CCP1CON;
__at(0x0FBD) volatile __CCP1CONbits_t CCP1CONbits;

__at(0x0FBE) __sfr CCPR1;

__at(0x0FBE) __sfr CCPR1L;

__at(0x0FBF) __sfr CCPR1H;

__at(0x0FC0) __sfr ADCON2;
__at(0x0FC0) volatile __ADCON2bits_t ADCON2bits;

__at(0x0FC1) __sfr ADCON1;
__at(0x0FC1) volatile __ADCON1bits_t ADCON1bits;

__at(0x0FC2) __sfr ADCON0;
__at(0x0FC2) volatile __ADCON0bits_t ADCON0bits;

__at(0x0FC3) __sfr ADRES;

__at(0x0FC3) __sfr ADRESL;

__at(0x0FC4) __sfr ADRESH;

__at(0x0FC5) __sfr SSPCON2;
__at(0x0FC5) volatile __SSPCON2bits_t SSPCON2bits;

__at(0x0FC6) __sfr SSPCON1;
__at(0x0FC6) volatile __SSPCON1bits_t SSPCON1bits;

__at(0x0FC7) __sfr SSPSTAT;
__at(0x0FC7) volatile __SSPSTATbits_t SSPSTATbits;

__at(0x0FC8) __sfr SSPADD;

__at(0x0FC9) __sfr SSPBUF;

__at(0x0FCA) __sfr T2CON;
__at(0x0FCA) volatile __T2CONbits_t T2CONbits;

__at(0x0FCB) __sfr PR2;

__at(0x0FCC) __sfr TMR2;

__at(0x0FCD) __sfr T1CON;
__at(0x0FCD) volatile __T1CONbits_t T1CONbits;

__at(0x0FCE) __sfr TMR1;

__at(0x0FCE) __sfr TMR1L;

__at(0x0FCF) __sfr TMR1H;

__at(0x0FD0) __sfr RCON;
__at(0x0FD0) volatile __RCONbits_t RCONbits;

__at(0x0FD1) __sfr WDTCON;
__at(0x0FD1) volatile __WDTCONbits_t WDTCONbits;

__at(0x0FD2) __sfr OSCCON2;
__at(0x0FD2) volatile __OSCCON2bits_t OSCCON2bits;

__at(0x0FD3) __sfr OSCCON;
__at(0x0FD3) volatile __OSCCONbits_t OSCCONbits;

__at(0x0FD5) __sfr T0CON;
__at(0x0FD5) volatile __T0CONbits_t T0CONbits;

__at(0x0FD6) __sfr TMR0;

__at(0x0FD6) __sfr TMR0L;

__at(0x0FD7) __sfr TMR0H;

__at(0x0FD8) __sfr STATUS;
__at(0x0FD8) volatile __STATUSbits_t STATUSbits;

__at(0x0FD9) __sfr FSR2L;

__at(0x0FDA) __sfr FSR2H;

__at(0x0FDB) __sfr PLUSW2;

__at(0x0FDC) __sfr PREINC2;

__at(0x0FDD) __sfr POSTDEC2;

__at(0x0FDE) __sfr POSTINC2;

__at(0x0FDF) __sfr INDF2;

__at(0x0FE0) __sfr BSR;

__at(0x0FE1) __sfr FSR1L;

__at(0x0FE2) __sfr FSR1H;

__at(0x0FE3) __sfr PLUSW1;

__at(0x0FE4) __sfr PREINC1;

__at(0x0FE5) __sfr POSTDEC1;

__at(0x0FE6) __sfr POSTINC1;

__at(0x0FE7) __sfr INDF1;

__at(0x0FE8) __sfr WREG;

__at(0x0FE9) __sfr FSR0L;

__at(0x0FEA) __sfr FSR0H;

__at(0x0FEB) __sfr PLUSW0;

__at(0x0FEC) __sfr PREINC0;

__at(0x0FED) __sfr POSTDEC0;

__at(0x0FEE) __sfr POSTINC0;

__at(0x0FEF) __sfr INDF0;

__at(0x0FF0) __sfr INTCON3;
__at(0x0FF0) volatile __INTCON3bits_t INTCON3bits;

__at(0x0FF1) __sfr INTCON2;
__at(0x0FF1) volatile __INTCON2bits_t INTCON2bits;

__at(0x0FF2) __sfr INTCON;
__at(0x0FF2) volatile __INTCONbits_t INTCONbits;

__at(0x0FF3) __sfr PROD;

__at(0x0FF3) __sfr PRODL;

__at(0x0FF4) __sfr PRODH;

__at(0x0FF5) __sfr TABLAT;

__at(0x0FF6) __sfr TBLPTR;

__at(0x0FF6) __sfr TBLPTRL;

__at(0x0FF7) __sfr TBLPTRH;

__at(0x0FF8) __sfr TBLPTRU;

__at(0x0FF9) __sfr PC;

__at(0x0FF9) __sfr PCL;

__at(0x0FFA) __sfr PCLATH;

__at(0x0FFB) __sfr PCLATU;

__at(0x0FFC) __sfr STKPTR;
__at(0x0FFC) volatile __STKPTRbits_t STKPTRbits;

__at(0x0FFD) __sfr TOS;

__at(0x0FFD) __sfr TOSL;

__at(0x0FFE) __sfr TOSH;

__at(0x0FFF) __sfr TOSU;
