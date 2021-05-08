#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

sbit LED=P1^7;

//Test function definitions

//Main function
void main(void)
{
	unsigned int i;
	unsigned char ch=0;
	unsigned int d_pos = 0;
	unsigned int states[] = {4,4,4,4,4};
	unsigned int offset = 0;
	unsigned int start_state = 4;
	unsigned int up_row = 0x80;
	unsigned int diff = 0x40;
	unsigned char* str = "";
	unsigned int cur_st;
	unsigned int points=0;
	unsigned int max_points=0;
	//Initialize port P1 for output from P1.7-P1.4
	P1 = 0x0F;
	
	//Call initialization functions
	lcd_init();
	uart_init();
	while(1)
	{
			//Receive a character
			unsigned int amt = 0;
			if(rx_complete)
			ch = receive_char();
			if(ch=='s')
				d_pos = 1;
			else if(ch=='w')
				d_pos = 0;
			if((d_pos==0 && states[0]<4) || (d_pos==1 && states[0]>4))
				if((offset*(states[0]%4))%4 >= 2){
					if(max_points<points)
						max_points = points;
					lcd_cmd(0x01);
					lcd_cmd(0x80);
					lcd_write_string("Points: ");
					int_to_string(points, str);
					lcd_write_string(str);
					lcd_cmd(0xC0);
					lcd_write_string("Max points: ");
					int_to_string(max_points, str);
					lcd_write_string(str);
					points = 0;
					start_state = 4;
					offset = 0;
					for(i = 0;i<5;i++)
						states[i]=4;
					continue;
				}
				
			if(offset==3){
				start_state = states[0];
				for(i=0;i<4;i++)
					states[i] = states[i+1];
				states[4] = (states[4]/2) + 4*(int)((states[4]/4) != (states[4]%2));
				offset=0;
				points++;
			}
			
			lcd_cmd(0x01);
			lcd_cmd(up_row + diff*d_pos + 2);
			lcd_write_char('D');
			
			lcd_cmd(up_row + diff*(int)(states[0]>=4));
			cur_st = (states[0]>=4)?(states[0]-4):(states[0]);
			switch(offset){
				case 0:
					lcd_write_char(' ');
				case 1:
					if(cur_st<2)
						lcd_write_char(' ');
					else
						lcd_write_char('*');
				default:
					if(cur_st%2==1)
						lcd_write_char('*');
					else
						lcd_write_char(' ');
			}
			
			for(i=0;i<5;i++){
				lcd_cmd(up_row + diff*(int)(states[i]>=4) + 3*i + 3-offset);
				cur_st = (states[i]>=4)?(states[i]-4):(states[i]);
				lcd_write_char(' ');
				if(cur_st<2)
					lcd_write_char(' ');
				else
					lcd_write_char('*');
				if(cur_st%2==1)
					lcd_write_char('*');
				else
					lcd_write_char(' ');
			}
			offset++;
			msdelay(1000);
		}
}