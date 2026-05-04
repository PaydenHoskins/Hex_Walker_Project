I2C_SETUP 
    ;******************************************************************************************************
    ;****			    Set Up Pic for I2C Slave COM					***
    ;******************************************************************************************************
    
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
	MOVLW	    H'81'
	MOVWF	    SSP1CON2		;Disable General Call
	BANKSEL	    SSP1CON3
	MOVLW	    H'08'		;Min SDA Hold time 300ns
	MOVWF	    SSP1CON3
	BANKSEL	    SSP1STAT
	MOVLW	    H'80'		
	MOVWF	    SSP1STAT		;Not in Tx mode, Slew Rate Control Disabled
	CLRF	    I2C_TEMP
	RETURN
	
;******************************************************************************************************
;****I2C_RECEIVE Function is Used to RECIVE 1 Bytes to the Addressed slave.  See Above for Instructions ***
;******************************************************************************************************
I2C_RECEIVE
	BANKSEL SSP1STAT
	BTFSS	SSP1STAT,5 
	GOTO	ADDRESS_RECEIVE
	GOTO	DATA_RECEIVE
	
ADDRESS_RECEIVE
	BANKSEL	SSP1BUF
	MOVF	SSP1BUF,0
	GOTO	END_RECEIVE
	
DATA_RECEIVE
	BANKSEL	SSP1BUF
	MOVF	SSP1BUF,0
	MOVWF	POSITION_BYTE
	GOTO	END_RECEIVE

END_RECEIVE
	BANKSEL	PIR1
	BCF	PIR1, 3
    
	BANKSEL	SSP1CON1
	BSF	SSP1CON1,4
    
    
	MOVF	BANK_SAVE, 0
	MOVWF	BSR
    
	MOVF	W_SAVE, 0
	RETURN

