;****************************************************************************
;									    *
;	    File Name: Servo_Drive_Main.asm				    *
;	    Date: 3/19/26						    *
;	    File Version: 1						    *
;	    Author:    Payden Hoskins					    *
;	    Company:   Idaho State Universisty;	RCET			    *
;	    Description: Control Servo Positions based on		    *
;	                 what is sent over I2C				    *
;****************************************************************************

COMPARE_BYTE	    EQU H'070'
RESET_BYTE	    EQU H'071'
POSITION_BYTE	    EQU H'072'
COUNT1		    EQU	H'073'    
    
;******************************************
;Include Files
;******************************************
#include <p16f1788.inc>
#include "pic16f1788_Setup.inc"

;******************************************
;CONFIG
;******************************************
    
; CONFIG1
; __config 0xD9EC
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_OFF & _CLKOUTEN_ON & _IESO_ON & _FCMEN_OFF
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _WRT_OFF & _VCAPEN_OFF & _PLLEN_ON & _STVREN_OFF & _BORV_LO & _LPBOR_OFF & _LVP_ON

 
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
    
    BANKSEL	RCSTA
    BSF		RCSTA,4			;Disable Continous Receive UART
    
    BANKSEL	PORTB
    CLRF	PORTB
    
    CLRF	COMPARE_BYTE
    
    CLRF	POSITION_BYTE
    BSF		POSITION_BYTE,0
    
    MOVLW	H'14'
    MOVWF	COUNT1
    
    MOVLW	H'FF'
    MOVWF	RESET_BYTE
    
    GOTO	TIMER_2_START
    
;******************************************
;Interrupt Service Routine
;******************************************
INTERRUPT
    GOTO	PWM
    
TIMER_2_START
    BANKSEL	INTCON
    BSF		INTCON,7    ;ENABLE GLOBALS
    
    BANKSEL	T2CON
    BSF		T2CON, 2    ;START TIMER 2
    
    BANKSEL	PORTB
    BSF		PORTB, 0    ;SET PORT B BIT 0 TO HIGH SO ITS READY FOR PWM	
    
    GOTO	MAIN
MAIN
    BTFSC	PORTB, 1
    GOTO	POSITION_SET_CHANGE
    GOTO MAIN

    
;------------PWM--------------
PWM
    INCF	COMPARE_BYTE, 1
    
    MOVF	COMPARE_BYTE, 0
    XORWF	COUNT1, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	COUNT_EQUAL
    
    MOVF	COMPARE_BYTE, 0
    XORWF	RESET_BYTE, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	RESET_PULSE
    GOTO	END_PWM
    
COUNT_EQUAL
    BANKSEL	PORTB
    BCF		PORTB, 0
    GOTO	END_PWM
    
RESET_PULSE
    BANKSEL	PORTB
    BSF		PORTB, 0
    
    CLRF	COMPARE_BYTE
    GOTO	END_PWM
    
END_PWM
    BANKSEL	PIR1
    BCF		PIR1, 1
    RETFIE
;------------------------------
    
;-----------POSITION-----------
POSITION_SET_CHANGE
    BTFSC   POSITION_BYTE, 0
    GOTO    POSITION_1
    
    BTFSC   POSITION_BYTE, 1
    GOTO    POSITION_2
    GOTO    MAIN

POSITION_1
    MOVLW   H'1F'   ;MAX
    MOVWF   COUNT1
    CLRF    POSITION_BYTE
    BSF	    POSITION_BYTE, 1
    GOTO    MAIN
    
POSITION_2
    MOVLW   H'08'   ;MIN
    MOVWF   COUNT1
    CLRF    POSITION_BYTE
    BSF	    POSITION_BYTE, 0
    GOTO    MAIN
    END  


