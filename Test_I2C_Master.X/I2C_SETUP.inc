;Created By: Payden Hoskins
;6th Semester
;Final Projects - I2C_MASTER_TEST
;Started 03/12/2026
;***************GOALS********************

;CODE FOR A I2C TO SEND EVERY 16MS
    
;****************************************

    
;****************************************
    ;VARIABLE $ EQU STATMENTS
;****************************************
    
ADDRESS_W	EQU H'070'
DATA_TX_1	EQU H'071'
DATA_TX_2	EQU H'072'
DATA_TX_3	EQU H'073'
DATA_TX_4	EQU H'074'	
    
I2C_CODE CODE 
    
I2C_MASTER_SETUP
;***********************************
    BANKSEL TRISC
    BSF     TRISC,3
    BSF	    TRISC,4		;Set SCL and SDA as inputs ON PORT C
    
;SETUP SDA AND SCL PINS
    BANKSEL APFCON1
    MOVLW   H'00'
    MOVWF   APFCON1
    BCF	    APFCON1,3
    BCF	    APFCON1,4		;SETS PORTC AS THE DATA AND CLOCK PINS
        
    BANKSEL	    SSP1ADD
    MOVLW	    H'55'
    MOVWF	    SSP1ADD		;Set BAUD Generator to 100Khz (16M Occilator)
    BANKSEL	    SSP1CON1
    MOVLW	    H'28'		
    MOVWF	    SSP1CON1		;Enable SDA and SLC Pins and Set as I2C Master Mode
    BANKSEL	    SSP1CON2
    MOVLW	    H'00'
    MOVWF	    SSP1CON2		;Disable General Call
    BANKSEL	    SSP1CON3
    MOVLW	    H'08'		;Min SDA Hold time 300ns
    MOVWF	    SSP1CON3
    BANKSEL	    SSP1STAT
    MOVLW	    H'A0'		
    MOVWF	    SSP1STAT		;Not in Tx mode, Slew Rate Control Disabled
    CLRF	    ADDRESS_W
    MOVLW	    H'55'
    MOVWF	    DATA_TX_1
    MOVLW	    H'AA'
    MOVWF	    DATA_TX_2
    MOVLW	    H'55'
    MOVWF	    DATA_TX_3
    RETURN
    
;***********************************
    
    ;******************************************************************************************************
;****I2C_WRITE Function is Used to Send 4 Bytes to the Addressed slave.  See Above for Instructions ***
;******************************************************************************************************
I2C_WRITE
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BSF	    SSP1CON2,0		;Generate Start Condition
START_WRITE_DONE
	BTFSC	    SSP1CON2,0
	GOTO	    START_WRITE_DONE	;Wait Until Start Condition Completes
	BANKSEL	    ADDRESS_W
	MOVFW	    ADDRESS_W
	BANKSEL	    SSP1BUF
	MOVWF	    SSP1BUF		;Send Peripheral Address (Write)
ADDRESS_SENT_W
	BANKSEL	    SSP1STAT
	BTFSC	    SSP1STAT,0		
	GOTO	    ADDRESS_SENT_W	;Wait Until Buffer is Clear (Address is Sent)
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BTFSC	    SSP1CON2,6		;Was an Acknowledge Received?
	GOTO	    BAD_COND		;Acknowledge Not Received
	BANKSEL	    DATA_TX_1
	MOVFW	    DATA_TX_1
	BANKSEL	    SSP1BUF
	MOVWF	    SSP1BUF		;Send First Data Byte
DATA_1_SENT
	BANKSEL	    SSP1STAT
	BTFSC	    SSP1STAT,0
	GOTO	    DATA_1_SENT		;Wait Until Buffer is Clear (Data is Sent)
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BTFSC	    SSP1CON2,6		;Was an Acknowledge Received?
	GOTO	    BAD_COND		;Acknowledge Not Received
	BANKSEL	    DATA_TX_2
	MOVFW	    DATA_TX_2
	BANKSEL	    SSP1BUF
	MOVWF	    SSP1BUF		;Send Second Data Byte
DATA_2_SENT
	BANKSEL	    SSP1STAT
	BTFSC	    SSP1STAT,0
	GOTO	    DATA_2_SENT		;Wait Unitl Buffer is Clear (Data is Sent)
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BTFSC	    SSP1CON2,6		;Was an Acknowledge Received?
	GOTO	    BAD_COND		;Acknowledge not Received
	BANKSEL	    DATA_TX_3
	MOVFW	    DATA_TX_3
	BANKSEL	    SSP1BUF
	MOVWF	    SSP1BUF		;Send Third Data Byte
DATA_3_SENT
	BANKSEL	    SSP1STAT
	BTFSC	    SSP1STAT,0
	GOTO	    DATA_3_SENT		;Wait Until Buffer is Clear (Data is Sent)
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BTFSC	    SSP1CON2,6		;Was an Acknowledge Received?
	GOTO	    BAD_COND		;Acknowledge Not Received
	BANKSEL	    DATA_TX_4
	MOVFW	    DATA_TX_4
	BANKSEL	    SSP1BUF
	MOVWF	    SSP1BUF		;Send Fourth Data Byte
DATA_4_SENT
	BANKSEL	    SSP1STAT
	BTFSC	    SSP1STAT,0
	GOTO	    DATA_4_SENT		;Wait Until Buffer is Clear (Data is Sent)
	CALL	    I2C_IDLE		;Ensure the I2C BUS is idle
	BANKSEL	    SSP1CON2
	BTFSC	    SSP1CON2,6		;Was an Acknowledge Received?
	GOTO	    BAD_COND		;Acknowledge Not Received
GOOD_STOP
	BANKSEL	    SSP1CON2
	BSF	    SSP1CON2,2		;Generate a Stop Condition
GOOD_STOP_DONE
	BTFSC	    SSP1CON2,2
	GOTO	    GOOD_STOP_DONE	;Wait Until Stop Condition Completes
	MOVLW	    H'00'
	RETURN				;Write Complete.  Return With 0x00 in Working Register
	
	
;******************************************************************************************************
;****BAD_COND Function is GOTO'd When a Slave Does not ACK.  Will Exit Current Routine		    ***
;******************************************************************************************************
BAD_COND
	BANKSEL	    SSP1CON2
	BSF	    SSP1CON2,2		;Generate Stop Condition
BAD_STOP_DONE
	BTFSC	    SSP1CON2,2
	GOTO	    BAD_STOP_DONE	;Wait Until Stop Condition Completes
	MOVLW	    H'FF'
	RETURN		;Write/Read Failed.  Return With 0xFF in Working Register
	
;************************************************************************************************************
;****I2C_Idle Function is Used to Ensure I2C Line is Not Busy.  Will Return to Function When Line is Idle ***
;************************************************************************************************************
I2C_IDLE
	BANKSEL	    SSP1CON2
	MOVLW	    H'1F'
	ANDWF	    SSP1CON2		;Compare to 0x1F to Test for 5 Busy Conditions
	BANKSEL	    STATUS
	BTFSS	    STATUS,2
	GOTO	    I2C_IDLE		;If any Conditions Still Busy Wait
CHECK_R_W
	BANKSEL	   SSP1STAT
	BTFSC	   SSP1STAT,2
	GOTO	   CHECK_R_W		;Wait Until  a Transmission is Not in Process
	RETURN				;I2C BUS is Idle Exit Routine	