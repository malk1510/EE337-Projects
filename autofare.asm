; This subroutine writes characters on the LCD
LCD_data equ P2    ;LCD Data port
LCD_rs   equ P0.0  ;LCD Register Select
LCD_rw   equ P0.1  ;LCD Read/Write
LCD_en   equ P0.2  ;LCD Enable
rupee equ 50H
paise equ 51H
dist_dm equ 52H
dist_km equ 53H
waiting equ 54H
hund_rupee equ 55H
rupee_print equ 56H
hund_print equ 57H
power equ 58H

org 0000H
	clr PSW.5
	lcall print_start
	lp1: jnb P1.0, lp3
		ljmp start
	lp3: sjmp lp1
	
org 001BH
	djnz r0, cond1
	clr PSW.7
	mov r0, #100
	
	wt_time:
		jnb P1.3, dist_time
		djnz r1, cond2
		inc waiting
		mov r1, #5
		lcall inc_money
		sjmp cond2
	
	dist_time:
		mov DPTR, #speeds
		mov a, P1
		anl a, #6H
		rr a
		movc a, @a+DPTR
		mov r2, a
		add a, dist_dm
		mov dist_dm, a
		subb a, #10
		jc next
		inc dist_km
		mov dist_dm, a
		
		next: jb PSW.5, dist_money
		clr PSW.7
		mov a, dist_km
		mov b, #10
		mul ab
		add a, dist_dm
		subb a, #15
		jc cond2
		setb PSW.5
		mov r2, a
		jz cond2
		
	dist_money:
		lcall inc_money
		djnz r2, dist_money
			
	cond2:
		lcall print_money
		jnb P1.3, p_sp
			lcall print_wait
			sjmp cond1
		p_sp: lcall print_speeds
		
	cond1: 
		mov th1, #0B1H
		mov tl1, #0E0H
		reti
	
	inc_money:
		clr PSW.7
		mov a, paise
		add a, #42
		mov paise, a
		inc rupee
		subb a, #100
		jc done1
		inc rupee
		mov paise, a
		done1:
		clr PSW.7
		mov a, rupee
		subb a, #100
		jc done2
		inc hund_rupee
		mov rupee, a
		done2: reti
	
org 0120H
	start:
	lcall delay
	lcall delay
		mov rupee, #21
		mov paise, #0
		mov dist_dm, #0
		mov dist_km, #0
		mov waiting, #0
		mov hund_rupee, #0
		lcall print_money
		lcall print_speeds
		mov tmod, #10H
		setb ea
		setb et1
		mov r0, #100
		mov r1, #5H
		mov th1, #0B1H
		mov tl1, #0E0H
		setb tr1
		lp2: jb P1.0, lp2
		clr tr1
		ljmp 0000H

;-------------------------------------------------------------


org 160h
	print_start:
      ;mov P1,#00h
	  ;initial delay for lcd power up

	;here1:setb p1.0
      	  acall delay
	;clr p1.0
	  acall delay
	;sjmp here1


	  acall lcd_init      ;initialise LCD
	
	  acall delay
	  acall delay
	  acall delay
	  mov a,#80h		 ;Put cursor on first row,5 column
	  acall lcd_command	 ;send command to LCD
	  acall delay
	  mov   DPTR,#my_string1   ;Load DPTR with sring1 Addr
	  acall lcd_sendstring	   ;call text strings sending routine
	  acall delay

	  mov a,#0C0h		  ;Put cursor on second row,3 column
	  acall lcd_command
	  acall delay
	  mov   DPTR,#my_string2
	  acall lcd_sendstring
	  ret				//stay here

	print_money:
	acall lcd_init
	acall delay
	acall delay
	acall delay
		clr PSW.7
		mov a, #80H
		acall lcd_command
		mov DPTR, #my_string4
		acall lcd_sendstring
		acall delay
		mov a, paise
		subb a, #50
		mov hund_print, hund_rupee
		mov rupee_print, rupee
		jc dont_inc
		clr PSW.7
		mov a, rupee_print
		add a, #1H
		subb a, #100
		jnz dont_inc_hund
		inc hund_print
		mov rupee_print, #0FFH
		dont_inc_hund: inc rupee_print
		dont_inc: mov a, hund_print
		add a, #30H
		acall lcd_senddata
		mov a, rupee_print
		acall print_digits
		ret
		
	print_digits:
		clr PSW.7
		mov r2, #08H
		mov b, #0H
		mov power, #1H
		decimal_adj:
			mov a, rupee_print
			rrc a
			mov rupee_print, a
			jnc addition
				mov a, power
				add a, b
				da a
				mov b, a
			addition: mov a, power
			add a, acc
			da a
			mov power, a
		djnz r2, decimal_adj
		mov a, b
		anl a, #0F0H
		swap a
		add a, #30H
		acall lcd_senddata 
		mov a, b
		anl a, #0FH
		add a, #30H
		acall lcd_senddata
		ret
		
	print_wait:
		acall delay
		mov a, #0C0H
		acall lcd_command
		acall delay
		mov DPTR, #my_string3
		acall lcd_sendstring
		acall delay
		ret
	
	print_speeds:
		acall delay
		mov a, #0C0H
		acall lcd_command
		acall delay
		mov a, P1
		anl a, #6H
		rr a
		mov DPTR, #func_list
		add a, acc
		jmp @a+DPTR
		func_list:
			sjmp func0
			sjmp func1
			sjmp func2
			sjmp func3
		
		func0:
			mov DPTR, #my_string6
			acall lcd_sendstring
			acall delay
			ret
		func1:
			mov DPTR, #my_string7
			acall lcd_sendstring
			acall delay
			ret
		func2:
			mov DPTR, #my_string8
			acall lcd_sendstring
			acall delay
			ret
		func3:
			mov DPTR, #my_string9
			acall lcd_sendstring
			acall delay
			ret

;------------------------LCD Initialisation routine----------------------------------------------------
lcd_init:
		mov P2, #0H
         mov   LCD_data,#38H  ;Function set: 2 Line, 8-bit, 5x7 dots
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
	     acall delay

         mov   LCD_data,#0CH  ;Display on, Curson off
         clr   LCD_rs         ;Selected instruction register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         
		 acall delay
         mov   LCD_data,#01H  ;Clear LCD
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         
		 acall delay

         mov   LCD_data,#06H  ;Entry mode, auto increment with no shift
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en

		 acall delay
         
         ret                  ;Return from routine

;-----------------------command sending routine-------------------------------------
 lcd_command:
         mov   LCD_data,A     ;Move the command to LCD port
         clr   LCD_rs         ;Selected command register
         clr   LCD_rw         ;We are writing in instruction register
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
		 acall delay
    
         ret  
;-----------------------data sending routine-------------------------------------		     
 lcd_senddata:
         mov   LCD_data,A     ;Move the command to LCD port
         setb  LCD_rs         ;Selected data register
         clr   LCD_rw         ;We are writing
         setb  LCD_en         ;Enable H->L
		 acall delay
         clr   LCD_en
         acall delay
		 acall delay
         ret                  ;Return from busy routine

;-----------------------text strings sending routine-------------------------------------
lcd_sendstring:
	push 0e0h
	lcd_sendstring_loop:
	 	 clr   a                 ;clear Accumulator for any previous data
	         movc  a,@a+DPTR         ;load the first character in accumulator
	         jz    exit              ;go to exit if zero
	         acall lcd_senddata      ;send first char
	         inc   DPTR              ;increment data pointer
	         sjmp  LCD_sendstring_loop    ;jump back to send the next character
exit:    pop 0e0h
         ret                     ;End of routine

;----------------------delay routine-----------------------------------------------------
delay:	 push 0
	 push 1
         mov r2,#1
loop2:	 mov r3,#255
	 loop1:	 djnz r3, loop1
	 djnz r2, loop2
	 pop 1
	 pop 0 
	 ret

;------------- ROM text strings---------------------------------------------------------------
org 300h
my_string1:
         DB   "Mumbai auto", 00H
my_string2:
		 DB   "For hire", 00H
my_string3:
		DB	"Waiting", 00H
my_string4:
		DB "Rs. ", 00H
my_string6:
		DB "360 km/hr", 00H
my_string7:
		DB "720 km/hr", 00H
my_string8:
		DB "1800 km/hr", 00H
my_string9:
		DB "3600 km/hr", 00H
speeds:
		DB 1,2,5,10

end