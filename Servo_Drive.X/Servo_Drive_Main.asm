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
COUNT1		    EQU	H'072' 
COUNT2		    EQU H'073'
COUNT3		    EQU H'074'
TEMP		    EQU H'075'
LEG_GROUP	    EQU H'076'
    
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
    
    MOVLW	H'14'
    MOVWF	COUNT1
    MOVLW	H'14'
    MOVWF	COUNT2
    MOVLW	H'14'
    MOVWF	COUNT3
    
    MOVLW	H'FF'
    MOVWF	RESET_BYTE
    
    GOTO	TIMER_2_START
    
;******************************************
;Interrupt Service Routine
;******************************************
INTERRUPT
    GOTO	LEG_GROUP_CHECK
    
TIMER_2_START
    
    CLRF    LEG_GROUP
    BANKSEL PORTA
    BTFSS   PORTA, 0
    BSF	    LEG_GROUP, 0
    BSF	    LEG_GROUP, 1
    
    BANKSEL	INTCON
    BSF		INTCON,7    ;ENABLE GLOBALS
    
    BANKSEL	T2CON
    BSF		T2CON, 2    ;START TIMER 2
    
    BANKSEL	PORTB
    BSF		PORTB, 2
    BSF		PORTB, 1
    BSF		PORTB, 0    ;SET PORT B BIT 0 TO HIGH SO ITS READY FOR PWM	
    
    GOTO	MAIN
MAIN
    NOP
    NOP
    ;GOTO	POSITION_SET_CHANGE
    NOP
    NOP
    GOTO MAIN
  
;-----------POSITION-----------    
    
LEG_GROUP_CHECK
    BTFSC   LEG_GROUP, 0
    GOTO    POSITIONS1
    BTFSC   LEG_GROUP, 1
    GOTO    POSITIONS2
    
;<editor-fold defaultstate="collapsed" desc="TEST_DIPSWITCH 1, 4, 5">
    
POSITIONS1   
    
    BANKSEL PORTB
    CLRF    TEMP
    MOVF    PORTB, 0
    MOVWF   TEMP
    
    LSRF    TEMP,1
    LSRF    TEMP,1
    LSRF    TEMP,1
    
    MOVLW   H'00'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_01
   
    MOVLW   H'01'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_11
    
    MOVLW   H'02'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_21
    
    MOVLW   H'03'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_31
    
    MOVLW   H'04'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_41
    
    MOVLW   H'05'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_51
    
    MOVLW   H'06'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_61
    
    MOVLW   H'07'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_71
    
    MOVLW   H'08'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_76
    
    MOVLW   H'09'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_67
    GOTO    POSITION_01
    ;</editor-fold>
    
;<editor-fold defaultstate="collapsed" desc="TEST_DIPSWITCH 2, 3, 6">
    
POSITIONS2
    
    BANKSEL PORTB
    CLRF    TEMP
    MOVF    PORTB, 0
    MOVWF   TEMP
    
    LSRF    TEMP,1
    LSRF    TEMP,1
    LSRF    TEMP,1
    
    MOVLW   H'00'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_02
   
    MOVLW   H'01'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_12
    
    MOVLW   H'02'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_22
    
    MOVLW   H'03'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_32
    
    MOVLW   H'04'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_42
    
    MOVLW   H'05'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_52
    
    MOVLW   H'06'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_62
    
    MOVLW   H'07'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_72
    
    MOVLW   H'08'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_82
    
    MOVLW   H'09'
    XORWF   TEMP, 0
    BANKSEL STATUS
    BTFSC   STATUS, 2
    GOTO    POSITION_92
    GOTO    POSITION_02
    ;</editor-fold>

;<editor-fold defaultstate="collapsed" desc="LEGS 1,4,5">
POSITION_01
    ;<editor-fold defaultstate="collapsed" desc="POSITION H'00'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>

POSITION_11
    ;<editor-fold defaultstate="collapsed" desc="POSITION H'01'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
   
POSITION_21
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_31
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'03'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'10'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_41
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'10'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_51
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'10'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_61
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'10'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_71
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_76
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM
    ;</editor-fold>
    
POSITION_67
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM
    ;</editor-fold>
    ;</editor-fold>     
    
;<editor-fold defaultstate="collapsed" desc="LEGS 2,3,6">
POSITION_02
    ;<editor-fold defaultstate="collapsed" desc="POSITION H'00'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>

POSITION_12
    ;<editor-fold defaultstate="collapsed" desc="POSITION H'01'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
   
POSITION_22
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_32
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'03'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_42
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_52
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'19'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_62
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_72
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_82
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'14'   ;MID (FOOT SERVO)
    MOVWF   COUNT1
    MOVLW   H'14'   ;MID (JOINT SERVO)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (SHOLDER SERVO)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    
POSITION_92
    ;<editor-fold defaultstate="collapsed" desc="POSTITON H'02'">
    MOVLW   H'0B'   ;(45 DEGREE)
    MOVWF   COUNT1
    MOVLW   H'0B'   ;MIN (ALL THE WAY UP)
    MOVWF   COUNT2
    MOVLW   H'14'   ;MID (90 DEGREE)
    MOVWF   COUNT3
    GOTO    INC_PWM;</editor-fold>
    ;</editor-fold>  

INC_PWM    
    INCF	COMPARE_BYTE, 1
    
    MOVF	COMPARE_BYTE, 0
    XORWF	COUNT1, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	COUNT1_EQUAL
    
COUNT2_TEST
    
    MOVF	COMPARE_BYTE, 0
    XORWF	COUNT2, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	COUNT2_EQUAL
    
COUNT3_TEST    
    
    MOVF	COMPARE_BYTE, 0
    XORWF	COUNT3, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	COUNT3_EQUAL
    
RESET_TEST    
    
    MOVF	COMPARE_BYTE, 0
    XORWF	RESET_BYTE, 0
    
    BANKSEL	STATUS
    BTFSC	STATUS, 2
    GOTO	RESET_PULSE
    GOTO	END_PWM
    
;PW_END   
;<editor-fold defaultstate="collapsed" desc="THIS RUNS WHEN THE COMPARE BYTE = THE COUNT BYTE">
    
COUNT1_EQUAL
    BANKSEL	PORTB
    BCF		PORTB, 0
    GOTO	COUNT2_TEST
    
COUNT2_EQUAL
    BANKSEL	PORTB
    BCF		PORTB, 1
    GOTO	COUNT3_TEST
    
COUNT3_EQUAL
    BANKSEL	PORTB
    BCF		PORTB, 2
    GOTO	RESET_TEST    ;</editor-fold>
 
RESET_PULSE
    BANKSEL	PORTB
    BSF		PORTB, 0
    BSF		PORTB, 1
    BSF		PORTB, 2
    
    CLRF	COMPARE_BYTE
    GOTO	END_PWM
    
END_PWM
    BANKSEL	PIR1
    BCF		PIR1, 1
    RETFIE
;------------------------------  
   
    END  


