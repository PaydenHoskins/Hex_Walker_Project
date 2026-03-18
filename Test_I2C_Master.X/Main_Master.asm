;Created By: Payden Hoskins
;6th Semester
;Final Projects - I2C_MASTER_TEST
;Started 03/16/2026

;******************************************
;Include Files
;******************************************

#include "p16f1788.inc"
#include "pic16f1788_Setup.inc"
#include "I2C_SETUP.inc"

;******************************************
;CONFIG
;******************************************
    
; CONFIG1
; __config 0xD9EC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_OFF & _CLKOUTEN_ON & _IESO_ON & _FCMEN_OFF
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_ON

 
;******************************************		
;Interrupt Vectors
;******************************************
    ORG H'00'					
    GOTO SETUP				;RESET CONDITION GOTO SETUP
    
    ORG H'04'
    GOTO INTERRUPT			;Interrupt occur GOTO INTERRUPT
    RETFIE

;******************************************
;Setup Routine
;******************************************
SETUP
    CALL	INITALIZE		;Set Up pic main operation
    CALL	I2C_MASTER_SETUP
    
    BANKSEL	T2CON
    BCF		T2CON,2			;Disable Timer 2
    
    BANKSEL	INTCON
    BCF		INTCON,7		;Disable Global Interrupts
     
    GOTO	MAIN
    
;******************************************
;Interrupt Service Routine
;******************************************
INTERRUPT
    RETFIE
    
MAIN
    CALL	I2C_WRITE
    END

