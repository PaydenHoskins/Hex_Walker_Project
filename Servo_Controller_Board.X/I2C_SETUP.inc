;Created By: Payden Hoskins
;6th Semester
;Final Projects - Hex Walker Servo Board
;Started 03/12/2026
;***************GOALS********************

;START WORKING ON REGISTERS OF DATA THAT SETS UP I2C
    
;****************************************

    
;****************************************
    ;VARIABLE $ EQU STATMENTS
;****************************************
    
BYTE_NUMBER	EQU H'070'
BUFFER_SAVE	EQU H'071'
BYTE_ADDRESS	EQU H'072'
BYTE_1		EQU H'073'
BYTE_2		EQU H'074'
BYTE_3		EQU H'075'
BYTE_4		EQU H'076'
BYTE_5		EQU H'077'
BYTE_6	        EQU H'078'
BYTE_7	        EQU H'079'
BYTE_8		EQU H'07A'
TEMP		EQU H'07B'
    
I2C_CODE CODE 
    
I2C_SLAVE_SETUP
;***********************************
    BANKSEL TRISC
    BSF     TRISC,3
    BSF	    TRISC,4		;Set SCL and SDA as inputs 
    
;SETUP SDA AND SCL PINS
    BANKSEL APFCON1
    CLRF    APFCON1
    BCF	    APFCON1,3
    BCF	    APFCON1,4
        
;SET ADDRESS
    BANKSEL SSP1ADD
    MOVLW   H'10'
    MOVWF   SSP1ADD

    BANKSEL SSP1CON1
    MOVLW   H'36'
    MOVWF   SSP1CON1

    BANKSEL SSP1CON2
    MOVLW   H'81'   ;ENABLE GENERAL CALL, ENABLE I2C MASTER RECIVE AND CLOCK STRECHING
    MOVWF   SSP1CON2
 
    BANKSEL SSP1CON3
    MOVLW   H'1B'   ;Disable Slave Interrupts, SDA Hold Time of 300nS, Auto Enable Address and Data Clock Stretching
    MOVWF   SSP1CON3
        
    BANKSEL SSP1STAT
    MOVLW   H'80'   ;Disable Slew Rate Control and flags
    MOVWF   SSP1STAT
    
    CLRF    BYTE_NUMBER
    CLRF    BUFFER_SAVE
    CLRF    BYTE_ADDRESS
    CLRF    BYTE_1
    CLRF    BYTE_2
    CLRF    BYTE_3
    CLRF    BYTE_4
    CLRF    BYTE_5
    CLRF    BYTE_6
    CLRF    BYTE_7
    CLRF    BYTE_8
    
    BANKSEL INTCON
    BSF	    INTCON,7
    BSF	    INTCON,6   
    
    RETURN
    
;***********************************
    
I2C_RECIVE      
    ;MOVE BUFFER TO BUFFER SAVE
    BANKSEL SSP1BUF
    MOVF    SSP1BUF,0	    ;MOVE BUFFER TO TEMP REG
    MOVWF   BUFFER_SAVE
    
    BANKSEL SSP1STAT
    BTFSS   SSP1STAT, 5	    ;TEST IF ADDRESS OR DATA, DATA-1, ADD-0
    GOTO    ADDRESS_RECIVE
    GOTO    DATA_RECIVE
    
ADDRESS_RECIVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_ADDRESS
    GOTO    END_RECIVE  
    
DATA_RECIVE   
    INCF    BYTE_NUMBER, 1
    MOVLW   H'10'
    XORWF   BYTE_ADDRESS, 0
    BANKSEL STATUS
    BTFSS   STATUS, 2
    GOTO    JOYSTICKS		;DETERMINE WHAT BYTE IT IS, 2 PACKETS ARE SENT EACH PACKET HAD 4 BYTES AND EACH ARE SENT UNDER DIFFRENT ADDRESSES.
    GOTO    BUTTONS
    
;------------WHICH BYTE-----------------  
JOYSTICKS
    BANKSEL STATUS
    MOVLW   H'01'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_1_SAVE
    
    MOVLW   H'02'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_2_SAVE
    
    MOVLW   H'03'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_3_SAVE
    
    MOVLW   H'04'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_4_SAVE
    
BUTTONS
    MOVLW   H'01'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_5_SAVE
    
    MOVLW   H'02'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_6_SAVE
    
    MOVLW   H'03'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_7_SAVE
    
    MOVLW   H'04'
    XORWF   BYTE_NUMBER, 0
    BTFSC   STATUS, 2
    GOTO    BYTE_8_SAVE
    GOTO    END_RECIVE        
;--------------------------------------------
    
;------------DATA BYTE SAVE----------------
BYTE_1_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_1
    GOTO    END_RECIVE
    
BYTE_2_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_2
    GOTO    END_RECIVE
    
BYTE_3_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_3
    GOTO    END_RECIVE
    
BYTE_4_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_4
    GOTO    END_RECIVE
    
BYTE_5_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_5
    GOTO    END_RECIVE
    
BYTE_6_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_6
    GOTO    END_RECIVE
    
BYTE_7_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_7
    GOTO    END_RECIVE
    
BYTE_8_SAVE
    MOVF    BUFFER_SAVE, 0
    MOVWF   BYTE_8
    CLRF    BYTE_NUMBER
    GOTO    END_RECIVE
;--------------------------------------------
    
END_RECIVE   
    BANKSEL PORTB
    MOVF    BYTE_2, 0
    MOVWF   TEMP
    LSRF    TEMP, 1
    LSRF    TEMP, 1
    MOVF    TEMP, 0
    MOVWF   PORTB
    
    BANKSEL SSP1CON1	
    BSF	    SSP1CON1,4	;RELEASE THE CLK LOW
    
    BANKSEL PIR1
    BCF	    PIR1,3
    
    RETFIE