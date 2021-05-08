#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions


sbit sw_1=P1^0;
sbit sw_2=P1^1;
sbit sw_3=P1^2;
sbit sw_4=P1^3;

void display_shit(char* a_char, char* b_char, char* c_char, char* d_char){
		 lcd_cmd(0x01);                       // clear lcd
	 lcd_cmd(0x80);	                         //Move cursor to first line
	 lcd_write_string("A: ");			//Display "A: " on first line of lcd
	 //int_to_string(a,a_char);					//Converting integer to string of ascii
	 lcd_write_string( a_char );
		 
	 lcd_write_string("   ");
	 lcd_write_string("B: ");
	 //int_to_string(b,b_char);					//Converting integer to string of ascii
	 lcd_write_string( b_char );
		 
	 lcd_cmd(0xC0);                 	 //Move cursor to second line
   lcd_write_string("C: ");			//Display "C: " 
	 //int_to_string(token,c_char);					//Converting integer to string of ascii
	 lcd_write_string( c_char );
		 
	 lcd_write_string("   ");
	 lcd_write_string("D: ");
	 lcd_write_string( d_char );
	msdelay(2000);
}
	
//Main function
void main(void)
{
	unsigned char *string_1 ="A: ";
unsigned char *string_2 ="B: ";
unsigned char *string_3 ="C: ";
unsigned char *string_4 ="D: ";
	unsigned char a_char[] = {' ',' '};
	unsigned char b_char[] = {' ',' '};
	unsigned char c_char[] = {' ',' '};
	unsigned char d_char[] = {' ',' '};
	unsigned int count = 0;
	unsigned int token = 0;
	unsigned int a = 0;
	unsigned int b = 0;
	unsigned int c = 0;
	unsigned int d = 0;
	unsigned char ch=0;
		 unsigned int temp1 = sw_1;
	 unsigned int temp2 = sw_2;
	 unsigned int temp3 = sw_3;
	 unsigned int temp4 = sw_4;
	//Call initialization functions
	lcd_init();
	uart_init();
	
	while(1) {
	//Receive a character
     ch = receive_char();
	 
	 if(ch== 't') {
	 if( count < 100 )  {
	   count = count + 1;}
	  else
	   count = 1;
	   }
	 
	 // display Token: XX
	 
	 
	 if( sw_1 != temp1) {
			token++;
			a = token; 
			if ( token < count) { 
					int_to_string(a, a_char);
			}	 
   		else{
				token--;
				a--;
				a_char[0] = ' ';
				a_char[1] = ' ';
	 }
	 }
	 
	 if( sw_2 != temp2) {
			token++;
			b = token; 
			if ( token < count) { 
					int_to_string(b, b_char);
			}	 
   		else{
				token--;
				b--;
				b_char[0] = ' ';
				b_char[1] = ' ';
	 }
	 }
	 
	 	 if( sw_3 != temp3) {
			token++;
			c = token; 
			if ( token < count) { 
					int_to_string(c, c_char);
			}	 
   		else{
				token--;
				c--;
				c_char[0] = ' ';
				c_char[1] = ' ';
	 }
	 }
		 
	 	 if( sw_4 != temp4) {
			token++;
			d = token; 
			if ( token < count) { 
					int_to_string(d, d_char);
			}	 
   		else{
				token--;
				d--;
				d_char[0] = ' ';
				d_char[1] = ' ';
	 }
	 }
	 
	 display_shit(a_char, b_char, c_char, d_char);
	 }
 }