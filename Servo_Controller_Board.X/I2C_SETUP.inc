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
BUTTON_JOYSTICK	EQU H'078'
BYTE_1_B	EQU H'079'
BYTE_2_B	EQU H'07A'
BYTE_3_B	EQU H'07B'
BYTE_4_B	EQU H'07C'
    
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
    CLRF    BUTTON_JOYSTICK
    
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
    GOTO    END_RECIVE
    
DATA_RECIVE    
    BTFSC   BUTTON_JOYSTICK, 0	;test if its the joystick or button data byte
    GOTO    BUTTON_BYTES	;save button data
    GOTO    SERVO_BYTES		;save servo data
    GOTO    END_RECIVE
    
ADDRESS_RECIVE
    CLRF    BUTTON_JOYSTICK
    BTFSC   BUFFER_SAVE, 4  ;TEST IF GEN-CALL OR NOT
    BSF	    BUTTON_JOYSTICK,0	;SET IF NOT GEN-CALL
    GOTO    END_RECIVE
    
;------------SERVO DATA BYTE-----------------  
SERVO_BYTES
    MOVF    BUFFER_SAVE, 0
    BTFSC   BYTE_NUMBER, 0
    GOTO    SERVO_BYTE_2
    BSF	    BYTE_NUMBER, 0
    MOVWF   BYTE_1
    GOTO    END_RECIVE
SERVO_BYTE_2	
    BTFSC   BYTE_NUMBER, 1
    GOTO    SERVO_BYTE_3
    BSF	    BYTE_NUMBER, 1
    MOVWF   BYTE_2
    GOTO    END_RECIVE
SERVO_BYTE_3	
    BTFSC   BYTE_NUMBER, 2
    GOTO    SERVO_BYTE_4
    BSF	    BYTE_NUMBER, 2
    MOVWF   BYTE_3
    GOTO    END_RECIVE   
SERVO_BYTE_4	
    BTFSC   BYTE_NUMBER, 3
    GOTO    END_RECIVE
    CLRF    BYTE_NUMBER
    MOVWF   BYTE_4
    GOTO    END_RECIVE     
    
;--------------------------------------------
    
;------------BUTTON DATA BYTE----------------
BUTTON_BYTES
    GOTO    END_RECIVE
;--------------------------------------------
    
;---------------SERVO_DATA-------------------	    
;--------------------------------------------
    
;---------------BUTTON_DATA------------------
;--------------------------------------------
    
END_RECIVE   
    
    BANKSEL PORTB
    MOVF    BYTE_1, 0
    MOVWF   PORTB
    
    BANKSEL SSP1CON1	
    BSF	    SSP1CON1,4	;RELEASE THE CLK LOW
    
    BANKSEL PIR1
    BCF	    PIR1,3
    
    RETFIE