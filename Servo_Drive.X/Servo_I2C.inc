I2C_SETUP 
    ;******************************************************************************************************
    ;****			    Set Up Pic for I2C Slave COM					***
    ;******************************************************************************************************
	I2C_TEMP    EQU	H'22'
    
	BANKSEL	    PIE1
	BSF	    PIE1,3		;ENABLE MSSP INTERRUPTS
	BANKSEL	    TRISC
	BSF	    TRISC,3
	BSF	    TRISC,4 		;Set SCL and SDA as inputs
	BANKSEL	    SSP1ADD
	MOVLW	    H'30'
	MOVWF	    SSP1ADD		
	BANKSEL	    SSP1CON1
	MOVLW	    H'36'		
	MOVWF	    SSP1CON1		;Enable SDA and SLC Pins and Set as I2C Master Mode
	BANKSEL	    SSP1CON2
	MOVLW	    H'00'
	MOVWF	    SSP1CON2		;Disable General Call
	BANKSEL	    SSP1CON3
	MOVLW	    H'71'		;Min SDA Hold time 300ns
	MOVWF	    SSP1CON3
	BANKSEL	    SSP1STAT
	MOVLW	    H'70'		
	MOVWF	    SSP1STAT		;Not in Tx mode, Slew Rate Control Disabled
	RETURN
	
;******************************************************************************************************
;****I2C_RECEIVE Function is Used to RECIVE 1 Bytes to the Addressed slave.  See Above for Instructions ***
;******************************************************************************************************
I2C_RECEIVE
	BANKSEL SSP1STAT
	BTFSS SSP1STAT,5 
	GOTO	ADDRESS_RECEIVE
	GOTO	DATA_RECEIVE
	
ADDRESS_RECEIVE
	BANKSEL	SSP1BUF
	MOVF	SSP1BUF,0
	GOTO	END_RECEIVE
	
DATA_RECEIVE
	BANKSEL	SSP1BUF
	MOVF	SSP1BUF,0
	MOVWF	I2C_TEMP
	GOTO	END_RECEIVE
	