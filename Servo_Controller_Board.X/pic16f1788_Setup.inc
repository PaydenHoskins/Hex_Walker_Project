;Created By: Payden Hoskins
;6th Semester
;Final Projects - Hex Walker Servo Board
;Started 03/11/2026
    
SETUP_CODE CODE 
    
INITALIZE
    ;   SETUP 16Mhz CLOCK   
    
    BANKSEL OSCCON
    MOVLW H'FB'
    MOVWF OSCCON			;Set Internal Oscilator to 16Mhz
    
    ;*** SET OPTION_REG: ****
    BANKSEL	OPTION_REG
    CLRF	OPTION_REG		;Weak Pull Up Disabled, Interrupt Edge Selection, Timer Clock Set-Up
    
    ;*** SET CCPXCON REG: **
    BANKSEL	CCP1CON
    CLRF	CCP1CON			;DISABLE PWM & CCP
    
    BANKSEL	CCP2CON
    CLRF	CCP2CON
    
    BANKSEL	CCP3CON
    CLRF	CCP3CON
    
    ;*** TIMER 1 SETUP *****
    BANKSEL	T1CON
    MOVLW	H'30'
    MOVWF	T1CON			;Use instruction clock (Fosc/4); 1:8 prescaler (wait on enable)
    
    BANKSEL	T1GCON
    CLRF	T1GCON			;Gate always enabled
    
    ;*** TIMER 2 SETUP *****
    BANKSEL	T2CON
    MOVLW	H'09'			;Sets Prescaler to 1:4; Post Scaler to 1:2(wait on enable)
    MOVWF	T2CON
    BANKSEL	PR2		
    MOVLW	D'25'			;Set PR2 count to 25 interrupt ever 0.05mS
    MOVWF	PR2			;(((16M/4)^-1 * 4) * 25) * 2 = 0.05mS
    
    ;SETUP PORT A
    BANKSEL	TRISA
    CLRF	TRISA	;SET A AT OUTPUTS
    BSF		TRISA,0	;SET RA0 TO OUTPUT (NEEDS TO BE INPUT IN FINAL VERSION FOR ANALOG INPUT)
    
    BANKSEL	WPUA
    CLRF	WPUA			;Disable Weak Pull Ups
    
    BANKSEL	ODCONA
    CLRF	ODCONA			;Disable Open Drain
    
    BANKSEL	SLRCONA
    CLRF	SLRCONA			;Disable Slew Rate Limit
    
    BANKSEL	INLVLA
    MOVLW	H'FF'
    MOVWF	INLVLA			;Set Input Threshold Voltage to CMOS Levels
    
    BANKSEL	LATA
    CLRF	LATA	;CLEAR ALL DATA FROM LAT A
    
    BANKSEL	PORTA
    CLRF	PORTA	;CLEAR ALL DATA FROM PORT A
    
    ;SETUP PORT B
    BANKSEL	TRISB
    MOVLW	H'00'
    MOVWF	TRISB
    
    BANKSEL	WPUB
    CLRF	WPUB			;Disable Weak Pull Ups
    
    BANKSEL	ODCONB
    CLRF	ODCONB			;Disable Open Drain
    
    BANKSEL	SLRCONB
    CLRF	SLRCONB			;Disable Slew Rate Limit
    
    BANKSEL	INLVLB
    MOVLW	H'FF'
    MOVWF	INLVLB			;Set Input Threshold Voltage to CMOS Levels
    
    BANKSEL	LATB
    MOVLW	H'00'
    MOVWF	LATB
    
    BANKSEL	PORTB
    MOVLW	H'00'
    MOVWF	PORTB
    
    ;SETUP PORT C
    BANKSEL	TRISC
    MOVLW	H'69'
    MOVWF	TRISC
    
    BANKSEL	WPUC
    CLRF	WPUC			;Disable Weak Pull Ups
    
    BANKSEL	ODCONC
    CLRF	ODCONC			;Disable Open Drain
    
    BANKSEL	SLRCONC
    CLRF	SLRCONC			;Disable Slew Rate Limit
    
    BANKSEL	INLVLC
    MOVLW	H'FF'
    MOVWF	INLVLC			;Set Input Threshold Voltage to CMOS Levels
    
    BANKSEL	LATC
    CLRF	LATC
    
    BANKSEL	PORTC
    CLRF	PORTC    
    
    BANKSEL	CLKRCON
    MOVLW	H'00'
    MOVWF	CLKRCON			;Disable Output CLock Reference
    
    ;*** SET OPTION_REG: ****
    BANKSEL	OPTION_REG
    CLRF	OPTION_REG		;Weak Pull Up Disabled, Interrupt Edge Selection, Timer Clock Set-Up

    ;*** SET CCPXCON REG: **
    BANKSEL	CCP1CON
    CLRF	CCP1CON			;DISABLE PWM & CCP
    
    BANKSEL	CCP2CON
    CLRF	CCP2CON
    
    BANKSEL	CCP3CON
    CLRF	CCP3CON
    
    ;*** Interrupt Set-Up : ****
    BANKSEL	INTCON
    CLRF	INTCON			;Disable Interrupts
    
    BANKSEL	PIE1
    CLRF	PIE1			;Enable MSSP, Disable Timer 2, and Timer 1 Interrupts
    BSF		PIE1,3
 
    BANKSEL	PIE2
    MOVLW	H'00'
    MOVWF	PIE2			;Disable Interrupts
    
    BANKSEL	PIE3
    MOVLW	H'00'
    MOVWF	PIE3			;Disable Interrupt
    
    BANKSEL	PIE4
    MOVLW	H'00'
    MOVWF	PIE4			;Disable Interrupts
    
    RETURN